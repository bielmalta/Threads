#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>

typedef struct {
    int *imagem;
    int largura;
    int altura;
    int max_iter;
    int inicio;
    int fim;
    int passo;
}DadosThread;

int calcular_pixel(int x, int y, int largura, int altura, int max_iter){
    double cr = -2.0 + (3.0 * x) / largura;
    double ci = -1.5 + (3.0 * y) / altura;
    double zr = 0.0;
    double zi = 0.0;
    int iter = 0;

    while (zr * zr + zi * zi <= 4.0 && iter < max_iter) {
        double novo_zr = zr * zr - zi * zi + cr;
        zi = 2.0 * zr * zi + ci;
        zr = novo_zr;
        iter++;
    }
    return (iter * 255) / max_iter;
}

void calcular_serial(int *imagem, int largura, int altura, int max_iter) {
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            imagem[y * largura + x] =
                calcular_pixel(x, y, largura, altura, max_iter);
        }
    }
}
void calcular_openmp(int *imagem, int largura, int altura, int max_iter, int num_threads) {
    #pragma omp parallel for num_threads(num_threads)
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            imagem[y * largura + x] =
                calcular_pixel(x, y, largura, altura, max_iter);
        }
    }
}

int ler_inteiro(char *texto, int *valor) {
    char *fim;
    long numero = strtol(texto, &fim, 10);
    if (*texto == '\0' || *fim != '\0' || numero <= 0 || numero > INT_MAX)
        return 0;
    *valor = (int)numero;
    return 1;
}

int salvar_imagem(const char *nome, int *imagem, int largura, int altura) {
    FILE *arquivo = fopen(nome, "w");
    if (arquivo == NULL)
        return 0;

    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            fprintf(arquivo, "%d ", imagem[y * largura + x]);
        }
        fprintf(arquivo, "\n");
    }
    fclose(arquivo);
    return 1;
}
double tempo_atual() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1000000000.0;
}

void *trabalho_pthreads1(void *arg) {
    DadosThread *dados = (DadosThread *)arg;
    for (int y = dados->inicio; y < dados->fim; y++) {
        for (int x = 0; x < dados->largura; x++) {
            dados->imagem[y * dados->largura + x] = calcular_pixel(x, y, dados->largura, dados->altura, dados->max_iter);
        }
    }
    return NULL;
}
int calcular_pthreads1(int *imagem, int largura, int altura, int max_iter, int num_threads) {
    pthread_t threads[num_threads];
    DadosThread dados[num_threads];

    int linhas = altura / num_threads;
    for (int i = 0; i < num_threads; i++) {
        dados[i].imagem = imagem;
        dados[i].largura = largura;
        dados[i].altura = altura;
        dados[i].max_iter = max_iter;
        dados[i].inicio = i * linhas;
        dados[i].fim = (i == num_threads - 1) ? altura : (i + 1) * linhas; /*condicionais*/

        if (pthread_create(&threads[i], NULL, trabalho_pthreads1, &dados[i]) != 0) {
            return 0;
        }
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    return 1;
}
void *trabalho_pthreads2(void *arg) {
    DadosThread *dados = (DadosThread *)arg;

    for (int y = dados->inicio; y < dados->altura; y += dados->passo) {
        for (int x = 0; x < dados->largura; x++) {
            dados->imagem[y * dados->largura + x] = calcular_pixel(x, y, dados->largura, dados->altura, dados->max_iter);
        }
    }
    return NULL;
}

int calcular_pthreads2(int *imagem, int largura, int altura, int max_iter, int num_threads) {
    pthread_t threads[num_threads];
    DadosThread dados[num_threads];

    for (int i = 0; i < num_threads; i++) {
        dados[i].imagem = imagem;
        dados[i].largura = largura;
        dados[i].altura = altura;
        dados[i].max_iter = max_iter;
        dados[i].inicio = i;
        dados[i].passo = num_threads;
        if (pthread_create(&threads[i], NULL, trabalho_pthreads2, &dados[i]) != 0) {
            return 0;
        }
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Erro: quantidade de argumentos invalida.\n");
        return 1;
    }
    int largura, altura, max_iter, num_threads;
    if (!ler_inteiro(argv[1], &largura) || !ler_inteiro(argv[2], &altura) || !ler_inteiro(argv[3], &max_iter) || !ler_inteiro(argv[4], &num_threads)) {
        fprintf(stderr, "Erro: parametros invalidos.\n");
        return 1;
    }
    int *imagem = malloc((size_t)largura * altura * sizeof(int));
    if (imagem == NULL) {
        fprintf(stderr, "Erro: falha na alocacao de memoria.\n");
        return 1;
    }
    double inicio = tempo_atual();
    calcular_serial(imagem, largura, altura, max_iter);
    double fim = tempo_atual();
    double tempo_serial = fim - inicio;
    
    if (!salvar_imagem("mandelbrot_ggm_serial.pgm", imagem, largura, altura)) {
        fprintf(stderr, "Erro: falha ao criar arquivo de saida.\n");
        free(imagem);
        return 1;
    }
    inicio = tempo_atual();

    calcular_openmp(imagem, largura, altura, max_iter, num_threads);
    fim = tempo_atual();
    double tempo_openmp = fim - inicio;
    if (!salvar_imagem("mandelbrot_ggm_openmp.pgm", imagem, largura, altura)) {
        fprintf(stderr, "Erro: falha ao criar arquivo de saida.\n");
        free(imagem);
        return 1;
    }
    for (int i = 0; i < largura * altura; i++)
        imagem[i] = 0;

    inicio = tempo_atual();

    if (!calcular_pthreads1(imagem, largura, altura, max_iter, num_threads)) {
        fprintf(stderr, "Erro: falha ao criar threads.\n");
        free(imagem);
        return 1;
    }
    fim = tempo_atual();
    double tempo_pthreads1 = fim - inicio;

    inicio = tempo_atual();

    if (!calcular_pthreads2(imagem, largura, altura, max_iter, num_threads)) {
        fprintf(stderr, "Erro: falha ao criar threads.\n");
        free(imagem);
        return 1;
    }
    fim = tempo_atual();
    double tempo_pthreads2 = fim - inicio;

    if (!salvar_imagem("mandelbrot_ggm_pthreads1.pgm", imagem, largura, altura)) {
        fprintf(stderr, "Erro: falha ao criar arquivo de saida.\n");
        free(imagem);
        return 1;
    }
    if (!salvar_imagem("mandelbrot_ggm_pthreads2.pgm", imagem, largura, altura)) {
        fprintf(stderr, "Erro: falha ao criar arquivo de saida.\n");
        free(imagem);
        return 1;
    }

    FILE *tempos = fopen("times.txt", "w");

    if (tempos == NULL) {
        fprintf(stderr, "Erro: falha ao criar times.txt.\n");
        free(imagem);
        return 1;
    }
    fprintf(tempos, "Serial: %.6f\n", tempo_serial);
    fprintf(tempos, "OpenMP: %.6f\n", tempo_openmp);
    fprintf(tempos, "Pthreads1: %.6f\n", tempo_pthreads1);
    fprintf(tempos, "Pthreads2: %.6f\n", tempo_pthreads2);

    fclose(tempos);
    free(imagem);
    return 0;
}
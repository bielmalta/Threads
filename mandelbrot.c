#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

int ler_inteiro(char *texto, int *valor) {
    char *fim;
    long numero = strtol(texto, &fim, 10);
    if (*texto == '\0' || *fim != '\0' || numero <= 0 || numero > INT_MAX)
        return 0;
    *valor = (int)numero;
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
    calcular_serial(imagem, largura, altura, max_iter);
    free(imagem);
    return 0;
}
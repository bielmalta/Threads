#include <stdio.h>
#include <stdlib.h>

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
# Mandelbrot com Threads

Este projeto foi desenvolvido para entendermos o funcionamento das Threads na prática

O programa gera os valores de uma imagem do conjunto de Mandelbrot utilizando quatro formas de execução:

- Serial
- OpenMP
- Pthreads com divisão por blocos
- Pthreads com divisão intercalada

Também são registrados os tempos de execução de cada implementação no arquivo `times.txt`.

## Compilação

Para compilar o projeto:

make 

## Execução

Execute o programa no seguinte formato:

./mandelbrot largura altura max_iteracoes num_threads

Onde:
    - largura: largura da imagem
    - altura: altura da imagem
    - max_iteracoes: número máximo de iterações do cálculo de Mandelbrot
    - num_threads: quantidade de threads utilizadas nas versões paralelas

## Limpeza

make clean



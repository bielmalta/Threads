# Mandelbrot com Threads

Projeto desenvolvido para a disciplina de Infraestrutura de Software.

O programa calcula o conjunto de Mandelbrot utilizando quatro formas de execução:

- Serial: cálculo do Mandelbrot e normalização de forma serial
- OpenMP: cálculo do Mandelbrot paralelizado com OpenMP
- Pthreads 1: cálculo do Mandelbrot paralelizado com Pthreads
- Pthreads 2: cálculo do Mandelbrot serial e normalização paralelizada com Pthreads

O programa também registra o tempo de execução de cada implementação no arquivo `times.txt`.

## Compilação

Para compilar o projeto:

make

## Execução

Execute o programa no seguinte formato:

./mandelbrot largura altura max_iteracoes num_threads

Exemplo: ./mandelbrot 1000 1000 2000 8

Onde:

- largura: largura da imagem
- altura: altura da imagem
- max_iteracoes: número máximo de iterações do cálculo de Mandelbrot
- num_threads: quantidade de threads utilizadas nas versões paralelas

Após a execução, são gerados:
- mandelbrot_ggm_serial.pgm
- mandelbrot_ggm_openmp.pgm
- mandelbrot_ggm_pthreads1.pgm
- mandelbrot_ggm_pthreads2.pgm
- times.txt

Os quatro arquivos .pgm devem conter o mesmo resultado.

## Limpeza

make clean



mandelbrot: mandelbrot.o
	gcc mandelbrot.o -o mandelbrot -fopenmp

mandelbrot.o: mandelbrot.c
	gcc -Wall -Wextra -std=c11 -fopenmp -c mandelbrot.c -o mandelbrot.o

clean:
	rm -f mandelbrot.o mandelbrot
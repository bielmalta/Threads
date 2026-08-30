mandelbrot: mandelbrot.o
	gcc mandelbrot.o -o mandelbrot -fopenmp -pthread

mandelbrot.o: mandelbrot.c
	gcc -Wall -Wextra -std=c11 -fopenmp -pthread -c mandelbrot.c -o mandelbrot.o

clean:
	rm -f mandelbrot.o mandelbrot
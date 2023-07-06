all: 3d.o generator
3d.o: 3d.c
	-gcc -Wall -std=c11 -c 3d.c
generator: generator.c 3d.o
	-gcc -Wall -std=c11 -o generator generator.c 3d.o
	-./generator
clean:
	rm -f 3d.o
	rm -f ./3d_test
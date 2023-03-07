opts = -fopenmp -lm
size = 1000

main: main.c Lab3IO.c Lab3IO.h timer.h
	gcc main.c Lab3IO.c $(opts) -o main

gen: datagen.c Lab3IO.c Lab3IO.h
	gcc datagen.c Lab3IO.c $(opts) -o gen

test: serialtester.c Lab3IO.c Lab3IO.h
	gcc serialtester.c Lab3IO.c $(opts) -o test

make all: main gen test

clean:
	rm gen main test
	rm data_input data_output
opts = -fopenmp -lm
size = 1000

main: main.c Lab3IO.c Lab3IO.h timer.h
	gcc main.c Lab3IO.c $(opts) -o main

single: main_single.c Lab3IO.c Lab3IO.h timer.h
	gcc main_single.c Lab3IO.c $(opts) -o single

gen: datagen.c Lab3IO.c Lab3IO.h
	gcc datagen.c Lab3IO.c $(opts) -o gen

test: serialtester.c Lab3IO.c Lab3IO.h
	gcc serialtester.c Lab3IO.c $(opts) -o test

make all: main single gen test

clean:
	rm gen main test single
	rm data_input data_output
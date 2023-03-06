opts = -fopenmp -lm
size = 1000

main: main_index_map.c Lab3IO.c Lab3IO.h timer.h
	gcc main.c Lab3IO.c $(opts) -o main

main_alt: main_no_index_map.c Lab3IO.c Lab3IO.h timer.h
	gcc main_no_index_map.c Lab3IO.c $(opts) -o main_alt

gen: datagen.c Lab3IO.c Lab3IO.h
	gcc datagen.c Lab3IO.c $(opts) -o gen

test: serialtester.c Lab3IO.c Lab3IO.h
	gcc serialtester.c Lab3IO.c $(opts) -o test

clean:
	rm time.txt
	rm data_input data_output
	rm gen main test
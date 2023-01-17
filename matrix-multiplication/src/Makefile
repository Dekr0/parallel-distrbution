gen_matrix:
	./matrix_generator -s 6000
run:
	./main 9 && ./tester
matrix_generator:
	gcc -g -Wall -o matrix_generator matrixgen.c
test: lab1_IO.o serialtester.o
	gcc -g -o tester lab1_IO.o serialtester.o -lm
main: main.o lab1_IO.o
	gcc -g -o main main.o lab1_IO.o -pthread -lm
serialtester.o: serialtester.c
	gcc -g -c serialtester.c
main.o: main.c main.h
	gcc -g -c main.c
lab1_IO.o: lab1_IO.o lab1_IO.h
	gcc -g -c lab1_IO.c
clean:
	rm *.o
clean_data:
	rm data_*
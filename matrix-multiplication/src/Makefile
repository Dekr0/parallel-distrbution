out: main.o lab1_IO.o 
	gcc -g -o out main.o lab1_IO.o -pthread -lm
main.o: main.c main.h
	gcc -g -c main.c
lab1_IO.o: lab1_IO.o lab1_IO.h
	gcc -g -c lab1_IO.c
clean:
	rm *.o
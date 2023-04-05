main:
	mpicc -g -Wall main.c Lab4_IO.c -o main -lm
datatrim:
	gcc datatrim.c -o datatrim
tester:
	gcc serialtester.c Lab4_IO.c -o serialtester -lm 
all: main datatrim tester
clean:
	rm main datatrim tester

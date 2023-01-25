compile:
	gcc -g simpleClient.c -o client
	gcc -g simpleServer.c -o server -pthread
	gcc -g arrayRW.c -o api -pthread
client:
	./client
server:
	./server
clean:
	rm client server

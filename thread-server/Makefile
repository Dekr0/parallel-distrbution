ip = 127.0.0.1
port = 8001
opts = -lpthread -lm -std=gnu11 -Wall
size = 1000


main:
	gcc -g main1.c -o main1 $(opts)
	gcc -g main2.c -o main2 $(opts)
	gcc -g main3.c -o main3 $(opts)
	gcc -g main4.c -o main4 $(opts)
attacker:
	gcc -g attacker.c -o attacker $(opts) 
client:
	gcc -g clientl.c -o client $(opts) 
run1:
	./main1 $(size) $(ip) $(port)
run2:
	./main2 $(size) $(ip) $(port)
run3:
	./main3 $(size) $(ip) $(port)
run4:
	./main4 $(size) $(ip) $(port)
attack:
	./attacker $(size) $(ip) $(port)
test:
	bash test.sh $(size) $(ip) $(port)
clean:
	rm main1 main2 main3 main4

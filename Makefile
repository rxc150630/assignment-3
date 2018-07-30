server: a3p1Server1.o
	gcc -o server a3p1Server1.o

a3p1Server1.o: a3p1Server1.c
	gcc -c a3p1Server1.c

client: client.o
        gcc -o client client.o

client.o: client.c
	        gcc -c client.c

a3p1Server2.o: a3p1Server2.c
	gcc -c a3p1Server2.c

a3p1Server3.o: a3p1Server3.c
	gcc -c a3p1Server3.c

a3p2client1.o: a3p1client1.c
	gcc -c a3p1client1.c

a3p2client2.o: a3p2client2.c
	gcc -c a3p2client2.c

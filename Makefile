main : libmyvote.a libmycrypto.a libmyciphers.a libmysigns.a libmpoker.a main.c
	gcc main.c -o main -L. -lmyvote -lmpoker -lmysigns -lssl -lcrypto -lmyciphers -lmycrypto -lm
libmycrypto.a : mycrypto.o
	ar rc libmycrypto.a mycrypto.o
mycrypto.o : libs/mycrypto.c
		gcc libs/mycrypto.c  -c -lm
libmyciphers.a : myciphers.o
	ar rc libmyciphers.a myciphers.o
myciphers.o : libs/myciphers.c libmycrypto.a
		gcc libs/myciphers.c  -c -L.  -lmycrypto
libmysigns.a : mysigns.o
	ar rc libmysigns.a mysigns.o
mysigns.o : libs/mysigns.c libmycrypto.a libmyciphers.a
		gcc libs/mysigns.c  -c -L.  -lmycrypto -lmyciphers -lssl -lcrypto
libmpoker.a : mpoker.o
	ar rc libmpoker.a mpoker.o
mpoker.o : libs/mpoker.c libmycrypto.a 
		gcc libs/mpoker.c  -c -L.  -lmycrypto  -lcrypto
libmyvote.a : myvote.o
	ar rc libmyvote.a myvote.o
myvote.o : libs/myvote.c libmycrypto.a libmyciphers.a libmysigns.a
		gcc libs/myvote.c  -c -L.  -lmycrypto -lmyciphers -lssl -lmysigns  -lcrypto

libmyauth.a : myauth.o
	ar rc libmyauth.a myauth.o
myauth.o : libs/myauth.c libmycrypto.a libmyciphers.a 
		gcc libs/myauth.c  -c -L.  -lmycrypto   -lmysigns  

client: libmycrypto.a libmyciphers.a libmyauth.a client.c
	gcc client.c -o client -L. -lmyauth -lmyciphers -lmycrypto -lm

server: libmycrypto.a libmyciphers.a libmyauth.a server.c
	gcc server.c -o server -L. -lmyauth -lmyciphers -lmycrypto -lm


run: main
	./main
run-server: server
	./server
run-client: client
	./client
clean: 
	rm *.o
	rm *.a
	rm main
	rm server
	rm client

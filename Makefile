main :  libmycrypto.a libmyciphers.a libmysigns.a libmpoker.a main.c
	gcc main.c -o main -L. -lmpoker -lmysigns -lssl -lcrypto -lmyciphers -lmycrypto -lm
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

run: main
	./main
clean: 
	rm *.o
	rm *.a
	rm main

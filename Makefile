main :  libmycrypto.a libmyciphers.a main.c
	gcc main.c -o main -L. -lmyciphers -lmycrypto   -lm
libmycrypto.a : mycrypto.o
	ar rc libmycrypto.a mycrypto.o
mycrypto.o : libs/mycrypto.c
		gcc libs/mycrypto.c  -c -lm
libmyciphers.a : myciphers.o
	ar rc libmyciphers.a myciphers.o
myciphers.o : libs/myciphers.c libmycrypto.a
		gcc libs/myciphers.c  -c -L.  -lmycrypto


run: main
	./main
clean: 
	rm *.o
	rm *.a
	rm main

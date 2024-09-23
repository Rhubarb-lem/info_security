main :  libmycrypto.a main.c
	gcc main.c -o main -L.  -lmycrypto -lm
libmycrypto.a : mycrypto.o
	ar rc libmycrypto.a mycrypto.o
mycrypto.o : lab1/mycrypto.c
		gcc lab1/mycrypto.c  -c -lm


run: main
	./main
clean: 
	rm *.o
	rm *.a
	rm main

all: main.c dictionary.o stdafx.h
	gcc main.c dictionary.o -I. -o dict
	strip dict
dictionary.o: dictionary.c dictionary.h stdafx.h
	gcc -c dictionary.c -I.

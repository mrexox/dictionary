all: main.c dictionary.o stdafx.h
	gcc -Wall main.c dictionary.o -I. -o dict
	strip dict
dictionary.o: dictionary.c dictionary.h stdafx.h
	gcc -Wall -c dictionary.c -I.

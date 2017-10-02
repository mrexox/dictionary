all: main.c stdafx.h
	gcc main.c -I. -o dict

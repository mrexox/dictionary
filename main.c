// -*- coding: utf-8 -*-
#include "stdafx.h"
#include "dictionary.h"
#define WORD_LEN         128
#define MEANING_LEN      100

#define UNEXPECTED_SYNTAX 10
#define NEW_LINE          11
#define COMMAND_NOT_FULL  12


#define UNKNOWN_COMMAND   20
#define END               -1

int
read_command(char*, char*, char*);
char*
execute_command(char*, char*, char*, int*);

static RUN = 1;

int
main() {
  printf("Hello!\n"
	 "Welcome to the dictionary!\n"
	 "------------------------------------------\n> ");
	// ...Проверка на наличие второго аргумента
  printf("Initializing dictionary...\n");
  initialize();

	// Инициализация переменных для считывания данных,
	// введённых пользователем
	char command;
	char word[WORD_LEN];
	char* meaning, result;
	int err;
	
	while(RUN) {
		printf("> ");
		// Отлавливаем ошибки считывания
		err = read_command(&command, word, meaning);
		switch (err) {
		UNEXPECTED_SYNTAX:
			print("Unexpected syntax.\n");
			continue;
			break;
		NEW_LINE: 
			continue;
			break;
		}
		
		result = execute_command(command, word, meaning, &err);
		switch (err) {
			//...
		case END:	RUN = 0;
			printf("See you!\n");
			break;
		}
	}

	free(command);
	free(word);
	free(meaning);
	free(result);
		
  return 0;
}

int
read_command(char* command, char* word, char* meaning) {
	char c;
	while ( isblank(c = getc(stdin)) );
	if (c == '\n')
		return NEW_LINE;
				 
	if (c != '\\') 
		return UNEXPECTED_SYNTAX;

	// Принимаем любую команду, являющуюся одним
	// непробельным символом
	c = getc(stdin);
	if (!isalnum(c)) {
		return UNEXPECTED_SYNTAX;
	}
	*command = c;
 
	while ( isblank(c = getc(stdin)) );
	if (c == '\n') 
		return UNEXPECTED_SYNTAX;

	char terminate_symbol = ' ';
	if (c == '\"')
		terminate_symbol = '\"';
	else if (c == '\'')
		terminate_symbol = '\'';
	
	if (!command_finished) {
		return UNEXPECTED_SYNTAX;
	}
}

char*
execute_command(char*, char*, char*, int*);

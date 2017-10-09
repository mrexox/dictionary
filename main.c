// -*- coding: utf-8 -*-
#include "stdafx.h"
#include "dictionary.h"
#define WORD_LEN         128
#define MEANING_LEN      200

#define UNEXPECTED_SYNTAX 10
#define NEW_LINE          11
#define COMMAND_NOT_FULL  12
#define TOO_LONG_WORD     13

#define UNKNOWN_COMMAND   20
#define END               -1

int
read_command(char*, char*, char*);
char*
execute_command(char, char*, char*, int*);

static int RUN = 1;

int
main() {
  printf("Hello!\n"
	 "Welcome to the dictionary!\n"
	 "------------------------------------------\n");
  // ...Проверка на наличие второго аргумента
  printf("Initializing dictionary...\n");
  initialize();
  
  // Инициализация переменных для считывания данных,
  // введённых пользователем
  char command;
  char word[WORD_LEN+1];	// +1 для '\0'
  char* meaning, result;
  int err;
  meaning = (char*)malloc(sizeof(char) * (MEANING_LEN+1)); // +1 для '\0'

  
  printf("> ");
  // Отлавливаем ошибки считывания
  err = read_command(&command, word, meaning);
  printf("ERR: %d, \\%c %s '%s'\n", err, command, word, meaning);
  return 0;
  
  while(RUN) {
    printf("> ");
    // Отлавливаем ошибки считывания
    err = read_command(&command, word, meaning);
    switch (err) {
    case UNEXPECTED_SYNTAX:
      printf("Unexpected syntax.\n");
      continue;
      break;
    case NEW_LINE: 
      continue;
      break;
    case COMMAND_NOT_FULL:
      printf("Command is not full.\n");
      continue;
      break;
    case TOO_LONG_WORD:
      printf("Word must be 128 symbols lenght.\n");
      continue;
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

  // Пропуск пробельных символов
  while ( isblank(c = getc(stdin)) );
  if (c == '\n')  {
    meaning[0] = '\0';
    word[0] = '\0';
    return 0;
  }

  // Если введён символ кавычки или двойной кавычки,
  // то считываем до второй такой кавычки
  char terminate_symbol = ' ';
  if (c == '\"')
    terminate_symbol = '\"';
  else if (c == '\'')
    terminate_symbol = '\'';
  else
    ungetc(c, stdin);

  int index = 0;
  while ((c = getc(stdin)) != terminate_symbol
	 && index < WORD_LEN
	 && c != '\n') {
    word[index++] = c;
  }
  if (index >= WORD_LEN)
    return TOO_LONG_WORD;
  word[index] = '\0';

  if (c == '\n')  {
    meaning[0] = '\0';
    return 0;
  }
  
  while ( isblank(c = getc(stdin)) );
  if (c == '\n')  {
    meaning[0] = '\0';
    return 0;
  }
  
  ungetc(c, stdin);

  index = 0;
  int size = MEANING_LEN;
  while ((c = getc(stdin)) != '\n') {
    meaning[index++] = c;
    if (index >= size) {
      meaning[index] = '\0';
      char* tmp = (char*)malloc(sizeof(char) * (size+MEANING_LEN+1)); // +1 для '\0'
      size += MEANING_LEN;
      strcpy(tmp, meaning);
      free(meaning);
      meaning = tmp;
    }
  }
  meaning[index] = '\0';
  return 0;
}

char*
execute_command(char command, char* word, char* meaning, int* err) {
  switch (command) {
  case 'a':
  case 'd':
  case 'f':
  case 'w':
  }
}

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
#define WORD_NOT_FOUND    21
#define FOUND             22
#define EMPTY_MEANING     23
#define END               -1
#define NOTHING           77
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
  char* meaning, *result;
	result = NULL;
  int err;
  meaning = (char*)malloc(sizeof(char) * (MEANING_LEN+1)); // +1 для '\0'

	/* TESTING  
  printf("> ");
  err = read_command(&command, word, meaning);
  printf("ERR: %d, \\%c %s '%s'\n", err, command, word, meaning);
  return 0;
  */
	
  while(RUN) {
    printf("> ");
    // Отлавливаем ошибки считывания
    err = read_command(&command, word, meaning);
    switch (err) {
    case UNEXPECTED_SYNTAX:
      printf("Unexpected syntax.\n");
      continue;
		case UNKNOWN_COMMAND:
			printf("This command will never be used here.\n");
			continue;
    case NEW_LINE: 
      continue;
    case TOO_LONG_WORD:
      printf("Word must be 128 symbols lenght.\n");
      continue;
    }
		
    result = execute_command(command, word, meaning, &err);
    switch (err) {
		case 0:
			break;
		case UNKNOWN_COMMAND:
			printf("This command is not supported but may be later.\n");
			break;
		case EMPTY_MEANING:
			printf("Cannot add a word without a meaning.\n");
			break;
		case NO_FREE_SPACE:
			printf("No free space in RAM. Close your apps that "
						 "may eat RAM and go on.\n");
			continue;
		case COLLISION:
			printf("The word exists!...\n");
			continue;
		case WORD_NOT_FOUND:
			printf("Word was not found!\n");
			continue;
		case WRITE_ERROR:
			printf("Unable to write to the file %s\n", word);
			break;
		case FILE_NOT_OPENED:
			printf("Unable to open this file %s\n", word);
			break;
		case FOUND:
			printf("\"%s\"\n", result);
			break;
    case END:
			RUN = 0;
      printf("See you!\n");
      break;
		case NOTHING:
			break;
		default:
			printf("Something went wrong.\n");
    }
  }

  free(meaning);
	if (result != NULL)
		free(result);
	free_dictionary();
	
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
    return UNKNOWN_COMMAND;
  }
  *command = c;

  // Пропуск пробельных символов или завершение команды
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

	// Если введённое слово слишком большое, то выходим
  if (index >= WORD_LEN)
    return TOO_LONG_WORD;
  word[index] = '\0';
	
	// Команда может быть завершена переводом строки сразу
  if (c == '\n')  {
    meaning[0] = '\0';
    return 0;
  }

	// Команда может быть завершена после
	// пробельных символов
  while ( isblank(c = getc(stdin)) );
  if (c == '\n')  {
    meaning[0] = '\0';
    return 0;
  }
	
  // Возвращаем первый считанный непробельный символ
  ungetc(c, stdin);

	// ... и считываем значение слова
  index = 0;
  int size = MEANING_LEN;
  while ((c = getc(stdin)) != '\n') {
    meaning[index++] = c;

		// При выходе значения за заданный предел расшириряем строку
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
	item* itm;
	FILE *wfile;
	switch (command) {
	case 'q':
		*err = END;
		break;
  case 'a': // Add - Добавить слово
		if (strlen(meaning) == 0) {
			*err = EMPTY_MEANING;
			break;
		}
		*err = add_word(word, meaning);
		break;
  case 'd': // Delete - Удалить слово
		*err = delete_word(word);
		break;
  case 'f': // Find - Найти слово
		itm = find_word(word);
		if (itm != NULL) {
			*err = FOUND;
			return itm->meaning;
		}
		else
			*err = WORD_NOT_FOUND;
		break;
	case 'w': // Write - Записать в файл
		wfile = fopen(word, "w");
		*err = write_dictionary(wfile);
		fclose(wfile);
		break;
	case 'l': // List - Вывести словарь в стандартный вывод
		*err = write_dictionary(stdout);
		break;
	default:
		*err = UNKNOWN_COMMAND;
  }
	return NULL;
}


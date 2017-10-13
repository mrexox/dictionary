#include "stdafx.h"
#define APR_MAX_WORDS 5000

#define MW_BITV ((int)pow(2, (int)log2(APR_MAX_WORDS)) - 1)
#define MAX_WORDS (MW_BITV+1)

#define NO_FREE_SPACE 2
#define DOES_NOT_EXIST 3
#define NOT_FOUND 4
#define COLLISION 5
#define FILE_NOT_OPENED 6
#define WRITE_ERROR 7

typedef struct item {
  char word[128];
  char* meaning;
  struct item* next;
} item;

//extern item** Dictionary;

int
initialize();

struct item*
find_word(char* word);

int
add_word(char*, char*);

int
delete_word(char*);

int
write_dictionary(FILE*);

int
load_dictionary(FILE*);

void
free_dictionary();

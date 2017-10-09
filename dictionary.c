#include "dictionary.h"

static item** Dictionary;

unsigned int
hash(char* word) {
  unsigned int value = 0;
  unsigned int a, b;
  a = 63689;
  b = 378551;
  char ch;
  while (ch = *word++) {
    value = value * a + ch;
    a *= b;
  }
  return value & MW_BITV;
}

int
initialize() {
  Dictionary = (item**)calloc( MAX_WORDS, sizeof(item*) );
  if (Dictionary == NULL) 
    return NO_FREE_SPACE;
  return 0;
}

int
add_word(char* word, char* meaning) {
  unsigned int hvalue = hash(word);
  item* record = (item*)malloc(sizeof(item));
  if (record == NULL)
    return NO_FREE_SPACE;
      
  strcpy(record->word, word);

  record->meaning = (char*)malloc(sizeof(char) * strlen(meaning));
  if (record->meaning == NULL) 
    return NO_FREE_SPACE;
  
  strcpy(record->meaning, meaning);

  record->next = NULL;

  item* iter = Dictionary[hvalue];
  if (iter == NULL) {
    Dictionary[hvalue] = record;
    return 0;
  }
  while (iter != NULL) {
    if (strcmp(iter->word, record->word) == 0) {
      // o Или замена слова
      // x Или ошибка 
      return COLLISION;
    }
    iter = iter->next;
  }
  iter->next = record;

  return 0;
}

item*
find_word(char* word) {
  unsigned int hvalue = hash(word);
  item* iter = Dictionary[hvalue];
  if (iter == NULL)
    return NULL;

  while (iter != NULL) {
    if (strcmp(iter->word, word) == 0)
      return iter;
    iter = iter->next;
  }
  
  return NULL;
}

int
delete_word(char* word) {
  unsigned int hvalue = hash(word);
  item* iter = Dictionary[hvalue];
  if (iter == NULL)
    return DOES_NOT_EXIST;

  if (iter->next == NULL) {
    Dictionary[hvalue] = NULL;
    free(iter->meaning);
    free(iter);
    return 0;
  }

  if (strcmp(iter->word, word) == 0) {
    Dictionary[hvalue] = iter->next;
    free(iter->meaning);
    free(iter);
    return 0;
  }
  
  while (iter->next != NULL) {
    if (strcmp(iter->next->word, word) == 0) {
      item* deleted = iter->next;
      iter->next = deleted->next;
      free(deleted->meaning);
      free(deleted);
      return 0;
    }
    iter = iter->next;
  }
  return NOT_FOUND;
}

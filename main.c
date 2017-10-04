#include "stdafx.h"
#include "dictionary.h"


int main() {
  printf("Hello!\n"
	 "Welcome to the dictionary!\n"
	 "------------------------------------------\n> ");
  printf("Initializing\n");
  initialize();
  
  printf("Adding word 'hello' - greeting\n");
  
  printf("RES: %d\n", add_word("hello", "meaning"));
  
  printf("Added\n");
  item* hi = find_word("hello");
  printf("WORD: %s MEANING: %s\n", hi->word, hi->meaning);


  char hello[128] = "Wow";
  char* meaning = "Wow means wowing!";
  add_word(hello, meaning);
  hi = find_word(hello);
  printf("%s means - %s\n", hi->word, hi->meaning);
  printf("Deleting wow");
  printf("%d\n", delete_word(hello));
  hi = find_word(hello);
  printf("wow exists? - %d\n", hi != NULL);
  return 0;
}

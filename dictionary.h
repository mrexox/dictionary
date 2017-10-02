#include "stdafx.h"

struct item {
  word char[128];
  meaning char**;
};

extern item* dictionary;

int
find_word(char[128] word);

int
add_word(char[128], char** meaning);

int
delete_word(char[128]);

int
save_dictionary(char** path, char** filename);

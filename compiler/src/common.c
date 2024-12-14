#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

char *bbc_read_file(const char* filename) {

  FILE *file = fopen(filename, "r");

  if(file == NULL) {
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  int filesize = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buf = malloc(filesize + 1);

  if(buf == NULL) {
    fclose(file);
    return NULL;
  }

  int read_size = fread(buf, 1, filesize, file);
  buf[read_size] = '\0';

  fclose(file);
  return buf;

}


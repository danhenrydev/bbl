#include <stdio.h>
#include <stdlib.h>
#include "error.h"



void bbc_error_generic(bbc_error_severity_t severity, char *context_string) {

  switch(severity) {
    case ERROR_WARNING:
      printf("WARNING: ");
    break;
    case ERROR_ERROR:
      printf("ERROR: ");
    break;
  }

  printf("%s\n", context_string);

  if(severity == ERROR_ERROR) {
    exit(1);
  }

}

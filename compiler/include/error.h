
typedef enum {
  ERROR_WARNING,
  ERROR_ERROR
} bbc_error_severity_t;



void bbc_error_generic(bbc_error_severity_t severity, char *context_string);

#ifndef COMMON_H
#define COMMON_H

#include <sys/stat.h>
#include <stdbool.h>

/**
 * @brief Reads the entire contents of a file into a dynamically allocated string
 *
 * @param filename Path to the file to read
 * @return char* Dynamically allocated string containing file contents. Caller must free.
 *         Returns NULL if file cannot be opened or read.
 *
 * @example
 * @code
 * char *content = bbc_read_file("example.txt");
 * if (content) {
 *     printf("File contents: %s\n", content);
 *     free(content);
 * }
 * @endcode
 */
char *bbc_read_file(const char* filename);


#endif //COMMON_H

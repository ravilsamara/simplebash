#ifndef S21_GREP
#define S21_GREP


#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int patterns;
  int regex_flag;
  int inver_match;
  int string_count;
  int files_with_match;
  int string_number;
  int no_name_file;
  int no_file;
  int flag_o;
} Flags;

Flags grep_read_file(int argc, char *argv[], char **pattern_e, int *temp_count);
void grep_count(FILE *file, char const *filename, regex_t *preg, int argc,
                Flags flags);
void grep_file(int argc, FILE *file, regex_t *preg, char const *filename);
void grep_file_with_match(FILE *file, char const *filename, regex_t *preg,
                          Flags flags);
void grep_string_number(FILE *file, char const *filename, regex_t *preg,
                        int argc);
void grep(int argc, char *argv[], Flags flags, char **pattern_e,
          int temp_count);
void grep_no_name_file(FILE *file, regex_t *preg);
void grep_file_e(int argc, FILE *file, regex_t *preg, char const *filename,
                 int temp_count);
void grep_file_o(int argc, FILE *file, regex_t *preg, char const *filename,
                 int temp_count);

#endif /*S21_GREP*/

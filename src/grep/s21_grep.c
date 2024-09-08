#include "s21_grep.h"

int main(int argc, char *argv[]) {
  char *pattern_e[argc];
  int temp_count = 0;
  Flags flags = grep_read_file(argc, argv, pattern_e, &temp_count);
  grep(argc, argv, flags, pattern_e, temp_count);
}

Flags grep_read_file(int argc, char *argv[], char **pattern_e,
                     int *temp_count) {
  Flags flags = {0};
  int currentFlags;
  static struct option long_options[] = {{0, 0, 0, 0}};
  while ((currentFlags = getopt_long(argc, argv, "e:ivclnhso", long_options,
                                     NULL)) != -1) {
    switch (currentFlags) {
      case 'e':
        flags.patterns |= REG_EXTENDED;
        pattern_e[*temp_count] = optarg;
        *temp_count += 1;
        break;
      case 'i':
        flags.regex_flag |= REG_ICASE;
        break;
      case 'v':
        flags.inver_match = 1;
        break;
      case 'c':
        flags.string_count = 1;
        break;
      case 'l':
        flags.files_with_match = 1;
        break;
      case 'n':
        flags.string_number = 1;
        break;
      case 'h':
        flags.no_name_file = 1;
        break;
      case 's':
        flags.no_file = 1;
        break;
      case 'o':
        flags.flag_o = 1;
        break;
      default:
        printf("use some of this options -e -i -v -c -l -n -h -s -o");
        exit(1);
    }
  }
  return flags;
}

void grep_no_name_file(FILE *file, regex_t *preg) {
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      printf("%s", line);
      if (line[strlen(line) - 1] != '\n') printf("\n");
    }
  }
  free(line);
}

void inverted_match(FILE *file, char const *filename, regex_t *preg, int argc) {
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  while (getline(&line, &length, file) > 0) {
    if (regexec(preg, line, 1, &match, 0)) {
      if (argc - (optind + 1) > 1) {
        printf("%s:%s", filename, line);
        if (line[strlen(line) - 1] != '\n') printf("\n");

      } else
        printf("%s", line);
    }
  }
  free(line);
}

void grep_count(FILE *file, char const *filename, regex_t *preg, int argc,
                Flags flags) {
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  int count = 0;
  int count_cv = 0;
  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      ++count;
    } else
      count_cv++;
  }
  if (flags.string_count && !flags.inver_match) {
    if (argc - (optind + 1) > 1)
      printf("%s:%d\n", filename, count);
    else
      printf("%d\n", count);
  } else {
    if (argc - (optind + 1) > 1)
      printf("%s:%d\n", filename, count_cv);
    else
      printf("%d\n", count_cv);
  }
  free(line);
}

void grep_file(int argc, FILE *file, regex_t *preg, char const *filename) {
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      if (argc - (optind + 1) > 1) {
        printf("%s:%s", filename, line);
        if (line[strlen(line) - 1] != '\n') printf("\n");
      } else {
        printf("%s", line);
        if (line[strlen(line) - 1] != '\n') printf("\n");
      }
    }
  }
  free(line);
}

void grep_file_o(int argc, FILE *file, regex_t *preg, char const *filename,
                 int temp_count) {
  char *line = 0;
  (void)temp_count;
  (void)filename;
  (void)argc;
  size_t length = 0;
  regmatch_t match[1];
  while (getline(&line, &length, file) > 0) {
    while (regexec(preg, line, 1, match, 0) == 0) {
      for (int i = 0; i < match->rm_eo; i++) {
        if (i >= match->rm_so) {
          printf("%c", line[i]);
        }
        line[i] = 127;
      }
      printf("\n");
    }
  }
  free(line);
}

void grep_file_e(int argc, FILE *file, regex_t *preg, char const *filename,
                 int temp_count) {
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      if ((argc - temp_count) > 3) {
        printf("%s:%s", filename, line);

        if (line[strlen(line) - 1] != '\n') printf("\n");
      } else
        printf("%s", line);
    }
  }

  free(line);
}

void grep_file_with_match(FILE *file, char const *filename, regex_t *preg,
                          Flags flags) {
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      printf("%s\n", filename);
      break;

    } else if (flags.files_with_match && flags.inver_match) {
      printf("%s\n", filename);
      break;
    }
  }
  free(line);
}

void grep_string_number(FILE *file, char const *filename, regex_t *preg,
                        int argc) {
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  int count = 0;
  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      ++count;
      if (argc - (optind + 1) > 1) {
        printf("%s:%d:%s", filename, count, line);
        if (line[strlen(line) - 1] != '\n') printf("\n");

      } else {
        printf("%d:%s", count, line);
        if (line[strlen(line) - 1] != '\n') printf("\n");
      }
    } else {
      ++count;
    }
  }
  free(line);
}

void grep(int argc, char *argv[], Flags flags, char **pattern_e,
          int temp_count) {
  char **pattern = &argv[optind];
  char **end = &argv[argc];
  regex_t preg_storage;
  regex_t *preg = &preg_storage;
  if (pattern == end) {
    fprintf(stderr, "no pattern\n");
    exit(1);
  }
  if (flags.patterns) {  // e
    for (int i = 0; i < temp_count; ++i) {
      regcomp(preg, pattern_e[i], flags.patterns);
      for (char **filename = pattern; filename != end; ++filename) {
        FILE *file = fopen(*filename, "r");
        if (file) {
          if (errno) {
            fprintf(stderr, "%s", argv[0]);
            perror(*filename);
            continue;
          }
          grep_file_e(argc, file, preg, *filename, temp_count);
          fclose(file);
          printf("\n");

        } else {
          fprintf(stderr, "no such file or directory\n");
          exit(1);
        }
      }
      regfree(preg);
    }

  } else {
    regcomp(preg, *pattern, flags.regex_flag);
    for (char **filename = pattern + 1; filename != end; ++filename) {
      FILE *file = fopen(*filename, "rb");
      if (file) {
        if (errno) {
          if (flags.no_file) {
            continue;
          } else {
            fprintf(stderr, "%s", argv[0]);
            perror(*filename);
          };
          continue;
        }
        if (flags.string_count || (flags.string_count && flags.inver_match)) {
          grep_count(file, *filename, preg, argc, flags);
        } else if (flags.files_with_match ||
                   (flags.files_with_match && flags.inver_match)) {
          grep_file_with_match(file, *filename, preg, flags);
        } else if (flags.string_number) {
          grep_string_number(file, *filename, preg, argc);
        } else if (flags.inver_match) {
          inverted_match(file, *filename, preg, argc);
        } else if (flags.no_name_file) {
          grep_no_name_file(file, preg);
        } else if (flags.flag_o) {
          grep_file_o(argc, file, preg, *filename, temp_count);
        } else {
          grep_file(argc, file, preg, *filename);
        }
        fclose(file);
      } else {
        fprintf(stderr, "no such file or directory\n");
        exit(1);
      }
    }
    regfree(preg);
  }
}

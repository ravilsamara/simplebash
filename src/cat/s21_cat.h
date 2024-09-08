#ifndef S21_CAT
#define S21_CAT

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  bool numNonBlank;
  bool markEndl;
  bool numberAll;
  bool squeeze;
  bool tab;
  bool printNonPrintble;
  bool invalid;
} Flags;

Flags catReadOptions(int argc, char *argv[]);
void CatFile(FILE *file, Flags flags, const char *table[static 256]);
void Cat(int argc, char *argv[], Flags flags, const char *table[static 256]);
void CatSetTable(const char *table[static 256]);
void CatSetEndl(const char *table[static 256]);
void CatSetTab(const char *table[static 256]);
void CatSetNonPrintable(const char *table[static 256]);

#endif /*S21_CAT*/
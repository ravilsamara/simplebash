CC=gcc

CFLAGS=-std=c11 -Wall -Wextra -Werror -D_GNU_SOURCE
ALL: clean s21_cat

s21_cat:
	$(CC) $(CFLAGS) s21_cat.c s21_cat.h -o s21_cat

test:
	sh test_func_cat.sh


clean:
	rm -rf s21_cat s21_cat.dSYM *.o
#include <stdio.h>
#include <stdlib.h>

#define LINELEN 512

void do_cat(FILE*);

int main(int ac, char* av[]) {
  FILE* fp;

  if (ac == 1) {
    do_cat(stdin);
  } else {
    while (--ac) {
      if ((fp = fopen(* ++av, "r")) != NULL) {
        do_cat(fp);
        fclose(fp);
      } else {
        exit(1);
      }
    }
  }

  return 0;
}

void do_cat(FILE* fp) {
  char line[LINELEN];

  while (fgets(line, LINELEN, fp)) {
    if (fputs(line, stdout) == EOF) {
      exit(1);
    }
  }
}

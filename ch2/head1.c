#include <stdio.h>
#include <stdlib.h>

#define LINELEN 512

void do_head(FILE*, int n);

int main(int ac, char* av[]) {
  FILE* fp;
  int linecnt;
  int filecnt;

  linecnt = 10;
  filecnt = ac - 1;

  if (ac == 1) {
    do_head(stdin, linecnt);
  } else {
    while (--ac) {
      if ((fp = fopen(* ++av, "r")) != NULL) {
        if (filecnt > 1) {
          if (ac != filecnt) {
            printf("\n");
          }
          printf("==> %s <==\n", *av);
        }
        do_head(fp, linecnt);
        fclose(fp);
      } else {
        exit(1);
      }
    }
  }

  return 0;
}

void do_head(FILE* fp, int n) {
  char line[LINELEN];

  while (fgets(line, LINELEN, fp) && n-- != 0) {
    if (fputs(line, stdout) == EOF) {
      exit(1);
    }
  }
}

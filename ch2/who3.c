#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SHOWHOST

void show_info(struct utmp *);
void show_time(long);

int utmp_open(char *);
struct utmp* utmp_next();
void utmp_close();

int main() {
    struct utmp * utbufp;

    if (utmp_open(UTMP_FILE) == -1) {
        perror(UTMP_FILE);
        exit(1);
    }

    while ((utbufp = utmp_next()) != (struct utmp *)NULL) {
        show_info(utbufp);
    }
    utmp_close();
    return 0;
}

/*
 * show_info()
 * displays contents of the utmp struct in human readable form
 */
void show_info(struct utmp * utbufp) {
    if (utbufp->ut_type != USER_PROCESS) {
        return;
    }
    printf("%-8.8s", utbufp->ut_name);
    printf(" ");
    printf("%-12.12s", utbufp->ut_line);
    printf(" ");
    show_time((long)utbufp->ut_time);
#ifdef SHOWHOST
    if (utbufp->ut_host[0] != '\0') {
        printf("(%s)", utbufp->ut_host);
    }
#endif
    printf("\n");
}

void show_time(long timeval) {
    char buf[18];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&timeval));
    printf("%s ", buf);
}

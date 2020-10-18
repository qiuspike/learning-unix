#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

/*
 * logout_tty(char * line)
 * marks a utmp record as logged out
 * does not blank username or remote host
 * return -1 on error, 0 on success
 */
int logout_tty(char * line) {
    int fd;
    struct utmp rec;
    int len = sizeof(struct utmp);
    int retval = -1;

    if ((fd = open(UTMP_FILE, O_RDWR)) == -1) {
        return -1;
    }

    /* search and replace */
    while (read(fd, &rec, len) == len) {
        if (strncmp(rec.ut_line, line, sizeof(rec.ut_line)) == 0) {
            rec.ut_type = DEAD_PROCESS;
            if (time(&rec.ut_time != -1)) {
                if (lseek(fd, -len, SEEK_CUR) != -1) {
                    if (write(fd, &rec, len) == len) {
                        retval = 0;
                    }
                }
            }
            break;
        }
    }

    /* close the file */
    if (close(fd) == -1) {
        retval = -1;
    }
    return retval;
}

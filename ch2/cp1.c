/** cp1
 *  version 1 of cp - use read and write with ruable buffer size
 *  usage: cp1 src dest
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

#define BUFFERSIZE 4096
#define COPYMODE   0644

void oops(char *, char *);

int main(int ac, char * av[]) {
    char *name, *options, *src, *dest;
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];

    if (ac != 3 && ac != 4) {
        fprintf(stderr, "usage: %s options source destination\n", *av);
        exit(1);
    }

    // parse src dest and options
    // options can be first or last
    name = av[0];
    if (ac == 3) {
        options = NULL;
        src = av[1];
        dest = av[2];
    } else if (ac == 4) {
        if (strstr(av[1], "-") == av[1]) {
            options = av[1];
            src = av[2];
            dest = av[3];
        } else if (strstr(av[3], "-") == av[3]) {
            options = av[3];
            src = av[1];
            dest = av[2];
        }
    }

    if (access(dest, F_OK) != -1) {
        char *in_path, *out_path;
        if ((in_path = realpath(src, NULL)) != NULL && (out_path = realpath(dest, NULL)) != NULL) {
            if (strlen(in_path) == strlen(out_path) && strncmp(in_path, out_path, strlen(in_path)) == 0) {
                fprintf(stderr, "cp: '%s' and '%s' are the same file", src, dest);
                exit(1);
            }
        } else {
            perror("get realpath of file error.");
            exit(1);
        }

        // ask if overwrite
        if (strchr(options, 'i') != NULL) {
            //scanf("%d", &a);
            printf("%s: overwrite '%s'? ", name, dest);
            char str[256];
            scanf("%s", str);
            if (strncmp(str, "y", 1) == 0
                    || strncmp(str, "Y", 1) == 0
                    || strncmp(str, "yes", 3) == 0
                    || strncmp(str, "YES", 3) == 0) {
                // do nothing
            } else {
                exit(0);
            }
        }
    }

    if ((in_fd = open(src, O_RDONLY)) == -1) {
        oops("Cannot open", src);
    }
    if ((out_fd = creat(dest, COPYMODE)) == -1) {
        oops("Cannot creat", dest);
    }

    while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0) {
        if (write(out_fd, buf, n_chars) != n_chars) {
            oops("Write error to", dest);
        }
    }
    if (n_chars == -1) {
        oops("Read error from", src);
    }
    if (close(in_fd) == -1 || close(out_fd) == -1) {
        oops("Error closing files", "");
    }
    return 0;
}

void oops(char* s1, char* s2) {
    fprintf(stderr, "Error: %s", s1);
    perror(s2);
    exit(1);
}

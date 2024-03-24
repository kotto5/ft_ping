#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "libft.h"
#include "get_next_line_bonus.h"
#include <arpa/inet.h>
#include <assert.h>

typedef struct s_maybeInt {
    bool            ok;
    unsigned        ip;
}   t_maybeInt;

#define MAY(ok_val, ip_val) ((t_maybeInt){ok_val, ip_val})

static t_maybeInt solve_with_file(char *host, char *file) {
    // TODO
    int fd = open(file, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return MAY(false, 0);
    }

    for (int i = 0; i < 1024; i++) {
        char *line = get_next_line(fd);
        if (line == NULL) {
            break;
        }
        printf("line: %s\n", line);
        char *ip = strtok(line, " \t");
        if (strchr(ip, '#') != NULL) {
            continue;
        }
        char *name = strtok(NULL, " ");
        printf("ip: %s, name: %s\n", ip, name);
        if (name != NULL && strncmp(name, host, strlen(host)) == 0) {
            close(fd);
            return MAY(true, inet_addr(ip));
        }        
    }
    close(fd);
    return MAY(false, 0);
}

// [Char] -> int
t_maybeInt    solve_name(char *host_name) {
    t_maybeInt result = solve_with_file(host_name, "/etc/hosts");
    // TODO : solve with DNS? It maybe forbidden?
    return result;
}

int test_solve_with_file() {
    char *host = "localhost";
    char *ip = "124.1.2.5";
    char *file_name = "test_solve_with_file";

    // create a file
    int fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return -1;
    }
    // write to the file
    dprintf(fd, "# 127.0.0.1    localhost\n");
    dprintf(fd, "%s %s\n", ip, host);

    // test
    t_maybeInt solve = solve_with_file(host, "test_solve_with_file");
    assert(solve.ok == true);
    assert(solve.ip == inet_addr(ip));

    // clean up
    close(fd);
    unlink(file_name);
    return 0;
}

int main(int argc, char *argv[]) {
    test_solve_with_file();

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        return -1;
    }

    t_maybeInt solve = solve_name(argv[1]);
    if (solve.ok == false)
        return -1;
    else
        printf("%d\n", solve.ip);
    return 0;
}

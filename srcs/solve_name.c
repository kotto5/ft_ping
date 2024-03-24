#include <stdbool.h>
#include <stdio.h>

typedef struct s_maybeInt {
    bool            ok;
    unsigned        ip;
}   t_maybeInt;

static t_maybeInt solve_with_file(char *file) {
    // TODO
    return (t_maybeInt){false, 0};
}

// [Char] -> int
t_maybeInt    solve_name(char *host_name) {
    t_maybeInt result = solve_with_file("/etc/hosts");
    // TODO : solve with DNS? It maybe forbidden?
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2)
        return -1;
    t_maybeInt solve = solve_name(argv[1]);
    if (solve.ok == false)
        return -1;
    else
        printf("%d\n", solve.ip);
    return 0;
}

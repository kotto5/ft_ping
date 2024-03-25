#include "ft_ping.h"

t_maybeInt solve_dns(char *host) {
    struct hostent *he = gethostbyname(host);
    if (he == NULL) {
        return MAY(false, 0);
    }
    struct in_addr **addr_list = (struct in_addr **)he->h_addr_list;
    if (addr_list[0] == NULL) {
        return MAY(false, 0);
    }
    uint32_t ip = ntohl(addr_list[0]->s_addr);
    return MAY(true, ip);
}

// [Char] -> int
t_maybeInt    solve_name(char *host_name) {
    t_maybeInt result;
    
    result = solve_dns(host_name);
    // TODO : solve with DNS? It maybe forbidden?
    return result;
}

int test_solve_dns() {
    // www.google.com を引数にしたいが、取れるaddressが頻繁に変わるのでlocalhostで代用
    char *host = "localhost";
    t_maybeInt solve = solve_dns(host);
    assert(solve.ok == true);
    printf("solve.ip: %x inet_addr(): %x\n", solve.ip, 0x7f000001);
    assert(solve.ip == 0x7f000001);
    return 0;
}

// int main(int argc, char *argv[]) {
//     test_solve_dns();

//     if (argc != 2) {
//         fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
//         return 1;
//     }

//     t_maybeInt solve = solve_name(argv[1]);
//     if (solve.ok)
//         printf("%x\n", solve.ip);
//     else {
//         printf("Not found\n");
//         return 1;
//     }
//     return 0;
// }

int destructor(void) __attribute__((destructor));
int destructor(void) {
    printf("Finished\n");
    return 0;
}

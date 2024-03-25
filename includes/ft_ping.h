#ifndef FT_PING_H
#define FT_PING_H

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
#include <netdb.h>
#include <netinet/in.h>

typedef struct s_maybeInt {
    bool            ok;
    unsigned        ip;
}   t_maybeInt;

#define MAY(ok_val, ip_val) ((t_maybeInt){ok_val, ip_val})

t_maybeInt    solve_name(char *host_name);

#endif
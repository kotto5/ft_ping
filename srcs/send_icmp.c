#include "ft_ping.h"
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

/* チェックサムの計算 */
u_int16_t checksum(unsigned short *buf, int size)
{
    unsigned long sum = 0;
    while (size > 1) {
        sum += *buf;
        buf++;
        size -= 2;
    }
    if (size == 1) {
        sum += *(unsigned char *)buf;
    }
    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);
    return ~sum;
}

// /* ICMPヘッダの作成 */
struct icmphdr setup_icmphdr(u_int8_t type, u_int8_t code, u_int16_t id, u_int16_t seq)
{
    struct icmphdr icmphdr;
    memset(&icmphdr, 0, sizeof(struct icmphdr));
    icmphdr.type = type;
    icmphdr.code = code;
    icmphdr.checksum = 0;
    icmphdr.un.echo.id = id;
    icmphdr.un.echo.sequence = seq;
    icmphdr.checksum = checksum((unsigned short *)&icmphdr, sizeof(struct icmphdr));

    return icmphdr;
}

int make_raw_socket(int protocol) {
    int sockfd = socket(AF_INET, SOCK_RAW, protocol);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }
    return sockfd;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ip address>\n", argv[0]);
        return 1;
    }

    t_maybeInt solve = solve_name(argv[1]);
    if (solve.ok)
        printf("%x\n", solve.ip);
    else {
        printf("Not found\n");
        return 1;
    }


    int n;
    char buf[1500];
    struct sockaddr_in addr;
    struct in_addr insaddr;
    struct iphdr *recv_iphdr;
    struct icmphdr *recv_icmphdr;

    int sockfd = make_raw_socket(IPPROTO_ICMP);
    // struct icmphdr icmphdr = setup_icmphdr(ICMP_ECHO, 0, 0, 0);
    struct icmphdr icmphdr = setup_icmphdr(ICMP_ECHO, 0, 0, 0);

    /* ICMPパケットの送信 */
    n = sendto(sockfd, (char *)&icmphdr, sizeof(icmphdr), 0, (struct sockaddr *)&addr, sizeof(addr));
    if (n < 1) {
        perror("sendto");
        return 1;
    }

        /* ICMPパケットの受信 */
    n = recv(soc, buf, sizeof(buf), 0);
    if (n < 1) {
        perror("recv");
        return 1;
    }

    recv_iphdr = (struct iphdr *)buf;
    /* IPヘッダからヘッダ長を求め、icmpヘッダの開始位置を調べる */
    recv_icmphdr = (struct icmphdr *)(buf + (recv_iphdr->ihl << 2));
    insaddr.s_addr = recv_iphdr->saddr;
    /* 送信先と受信したパケットの送信源が一致するかと受信したパケットがICMP ECHO REPLYか確認 */
    if (!strcmp(argv[1], inet_ntoa(insaddr)) && recv_icmphdr->type == ICMP_ECHOREPLY) {
        printf("icmp echo reply from %s\n", argv[1]);
    }
    close(sockfd);
    return 0;
}

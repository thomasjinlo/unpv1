#include "common/errors.h"
#define _POSIX_C_SOURCE 200112L
#include "common/common.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char **argv)
{
  int sockfd, n;
  char recvline[MAXLINE + 1];
  struct sockaddr_in servaddr;

  if (argc != 2) {
    err_quit("usage: daytimetcpcli <domain>");
  }

  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    err_sys("failed to create socket");
  }

  printf("Getting IPAddress %s\n", argv[1]);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;

  struct addrinfo *result, *rp;

  if ( (getaddrinfo(argv[1], "13332", NULL, &result)) < 0) {
    err_sys("getaddrinfo");
    exit(EXIT_FAILURE);
  }

  char ipstr[INET6_ADDRSTRLEN];

  for (rp = result; rp != NULL; rp = rp->ai_next) {
    struct sockaddr_in *addr = (struct sockaddr_in *)rp->ai_addr;
    inet_ntop(AF_INET, &(addr->sin_addr), ipstr, sizeof(ipstr));
    printf("connecting to %s\n", ipstr);

    if ( (connect(sockfd, rp->ai_addr, sizeof(*rp->ai_addr))) < 0) {
      err_sys("connect error");
      exit(EXIT_FAILURE);
    } else {
      break;
    }
  }

  printf("reading from %s\n", argv[1]);
  while ( (n = recv(sockfd, recvline, MAXLINE, 0)) > 0) {
    recvline[n] = 0;
    if (fputs(recvline, stdout) == EOF) {
      err_sys("fputs error");
    }
  }

  if (n < 0) {
    err_sys("read error");
  }

  exit(0);
}

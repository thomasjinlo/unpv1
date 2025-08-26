#include "common/common.h"
#include "common/errors.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
  int sockfd, n;
  char recvline[MAXLINE + 1];
  struct sockaddr_in6 servaddr;

  if (argc != 2) {
    err_quit("usage: daytimetcpcliv6 <IPAddress>");
  }

  if ( (sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
    err_sys("socket error");
  }

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin6_family = AF_INET6;
  servaddr.sin6_port = htons(13332);
  if (inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) <= 0) {
    err_quit("inet_pton error for %s", argv[1]);
  }

  if ( (connect(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr))) < 0) {
    err_sys("connect error");
  }

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

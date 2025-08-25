#include "common/common.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
  int sockfd, n;
  char recvline[MAXLINE + 1];
  struct sockaddr_in servaddr;

  if (argc != 2) {
    err_quit("usage: daytimetcpcli <IPaddress>");
  }

  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    err_sys("failed to create socket");
  }

  printf("Getting IPAddress %s\n", argv[1]);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(13332);
  if ( (inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) <= 0) {
    err_quit("inet_pton error %s", argv[1]);
  }

  printf("connecting to %s\n", argv[1]);
  if ( (connect(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr))) < 0) {
    err_sys("connect error");
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

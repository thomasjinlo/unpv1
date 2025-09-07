#include "common/common.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int main(void) {
  int listenfd, connfd;
  struct sockaddr_in servaddr;
  char buf[MAXLINE];
  time_t ticks;

  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(13332);

  if ( (bind(listenfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) <
      0) {
    err_sys("error binding socket");
  }

  if ( (listen(listenfd, SOMAXCONN)) < 0) {
    err_sys("error listening to socket");
  }

  while (1) {
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);

    if ( (connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len)) <
        0) {
      err_sys("error accepting connection");
    }

    char addr[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &cliaddr.sin_addr, addr, sizeof(addr)) == NULL) {
      err_sys("inet_ntop");
    }

    printf("accepted connection from: %s\n", addr);

    ticks = time(NULL);
    snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
    send(connfd, buf, strlen(buf), 0);
    close(connfd);
  }
}

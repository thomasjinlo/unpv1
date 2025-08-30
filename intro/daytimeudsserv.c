#include "common/common.h"
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/daytimeuds"

int listenfd;

void cleanup(int sig)
{
  (void)sig;
  unlink(SOCKET_PATH);
  close(listenfd);
  _exit(0);
}

int main(void)
{
  int connfd;
  struct sockaddr_un servaddr;
  char buf[MAXLINE];
  time_t ticks;

  listenfd = socket(AF_UNIX, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sun_family = AF_UNIX;
  strcpy(servaddr.sun_path, SOCKET_PATH);

  if ( (bind(listenfd, (const struct sockaddr *) &servaddr, sizeof(servaddr))) < 0) {
    err_sys("error binding socket");
  }

  if ( (listen(listenfd, SOMAXCONN)) < 0) {
    err_sys("error listening to socket");
  }

  signal(SIGINT, cleanup);
  signal(SIGTERM, cleanup);
  signal(SIGKILL, cleanup);

  while (1) {
    if ( (connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) < 0) {
      err_sys("error accepting connection");
    }

    ticks = time(NULL);
    snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
    send(connfd, buf, strlen(buf), 0);
    close(connfd);
  }
}

#include "common/constants.h"
#include "common/errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  if (argc != 2) {
    err_quit("Usage: %d <pathname>", argv[0]);
  }

  int fd, n;
  char recvline[MAXLINE + 1];
  struct sockaddr_un servaddr;

  fd = socket(AF_UNIX, SOCK_STREAM, 0);

  // clear garbage
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sun_family = AF_UNIX;

  strcpy(servaddr.sun_path, argv[1]);

  printf("connecting to %s\n", argv[1]);
  if ( (connect(fd, (const struct sockaddr *) &servaddr, sizeof(servaddr))) < 0) {
    err_sys("connect");
  }

  printf("reading from %s\n", argv[1]);
  while ( (n = recv(fd, recvline, MAXLINE, 0)) > 0) {
    recvline[n] = 0;
    if (fputs(recvline, stdout) == EOF) {
      err_sys("fputs");
    }
  }

  exit(0);
}

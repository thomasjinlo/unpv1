#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_CONNS 20000

int main(void)
{
  int fds[MAX_CONNS];
  struct sockaddr_in srcaddr, dstaddr;

  memset(&dstaddr, 0, sizeof(dstaddr));
  dstaddr.sin_family = AF_INET;
  dstaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  dstaddr.sin_port = htons(8080);

  memset(&srcaddr, 0, sizeof(srcaddr));
  srcaddr.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.3", &srcaddr.sin_addr);
  srcaddr.sin_port = htons(0);

  for (int i = 0; i < MAX_CONNS; i++) {
    if ( (fds[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      fprintf(stderr, "socket failed: errno=%d (%s)\n", errno, strerror(errno));
      exit(1);
    }
    if ( (bind(fds[i], (struct sockaddr *) &srcaddr, sizeof(srcaddr))) < 0) {
      fprintf(stderr, "bind failed: errno=%d (%s)\n", errno, strerror(errno));
      exit(1);
    }
    if ( (connect(fds[i], (struct sockaddr *) &dstaddr, sizeof(dstaddr))) < 0) {
      fprintf(stderr, "connect failed: errno=%d (%s)\n", errno, strerror(errno));
      exit(1);
    }
    // struct sockaddr_in local;
    // socklen_t len = sizeof(local);
    // if (getsockname(fds[i], (struct sockaddr*)&local, &len) == -1) {
    //     perror("getsockname");
    // } else {
    //     printf("port selected: %d\n", ntohs(local.sin_port));
    // }
  }

  for (int i = 0; i < MAX_CONNS; i++) {
    close(fds[i]);
  }

  exit(0);
}

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void)
{
  int fd;
  struct sockaddr_in saddr;

  fd = socket(AF_INET, SOCK_STREAM, 0);

  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_port = htons(8080);

  bind(fd, (struct sockaddr *) &saddr, sizeof(saddr));
  listen(fd, SOMAXCONN);

  for (;;) {
    struct sockaddr_in caddr;
    socklen_t caddr_len = sizeof(caddr);

    int cfd = accept(fd, (struct sockaddr *) &caddr, &caddr_len);
    if (cfd < 0) {
      fprintf(stderr, "accept failed: errno=%d (%s)\n", errno, strerror(errno));
      continue;
    }

    char cip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &caddr.sin_addr, cip, sizeof(cip));
    int cport = ntohs(caddr.sin_port);

    printf("Accepted: %s:%d\n", cip, cport);
    close(cfd);
  }
}

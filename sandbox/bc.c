#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_CONNS 20000

int main(void)
{
  int fds[MAX_CONNS];
  struct sockaddr_in dstaddr;

  memset(&dstaddr, 0, sizeof(dstaddr));
  dstaddr.sin_family = AF_INET;
  dstaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  dstaddr.sin_port = htons(8080);

  for (int i = 0; i < MAX_CONNS; i++) {
    fds[i] = socket(AF_INET, SOCK_STREAM, 0);
    connect(fds[i], (struct sockaddr *) &dstaddr, sizeof(dstaddr));
  }

  for (int i = 0; i < MAX_CONNS; i++) {
    close(fds[i]);
  }
  
  exit(0);
}

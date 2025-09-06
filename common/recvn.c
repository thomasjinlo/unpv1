#include "common/errors.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <errno.h>
#include <stddef.h>
#include <sys/socket.h>
#include <unistd.h>

ssize_t recvn(int fd, void *buf, size_t n)
{
  int nleft, read;
  char *ptr;

  nleft = n;
  ptr = buf;

  while (nleft > 0) {
    read = recv(fd, ptr, nleft, 0);
    if (read > 0) {
      nleft -= read;
      ptr += read;
    } else if (read == 0) { // EOF
      break;
    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
      continue;
    } else if (errno == EINTR) {
      continue;
    } else {
      err_sys("recvn");
    }
  }

  return n - nleft;
}

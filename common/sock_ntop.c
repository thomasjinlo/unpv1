#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int sock_ntop(const struct sockaddr *sa, char *dst)
{
  switch (sa->sa_family) {
    case AF_INET: {
      const struct sockaddr_in *sa_in = (const struct sockaddr_in *) sa;
      inet_ntop(AF_INET, &sa_in->sin_addr, dst, sizeof(dst));
      if (*dst == 0) {
        return -1;
      }
      return 0;
    }

    case AF_INET6: {
      const struct sockaddr_in6 *sa_in6 = (const struct sockaddr_in6 *) sa;
      inet_ntop(AF_INET, &sa_in6->sin6_addr, dst, sizeof(dst));
      if (*dst == 0) {
        return -1;
      }
      return 0;
    }
  }
  return -1;
}

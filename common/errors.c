#include "errors.h"
#include "constants.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

static void err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
  (void)errnoflag; // unused
  (void)level; // unused

  int errno_save, n;
  char buf[MAXLINE + 1];

  errno_save = errno;

  vsnprintf(buf, MAXLINE, fmt, ap);
  n = strlen(buf);

  printf("errno: %d\n", errno_save);
  snprintf(buf + n, MAXLINE - n, ": %s\n", strerror(errno_save));

  fflush(stdout);
  fputs(buf, stderr);
  fflush(stderr);
}

void err_quit(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(0, LOG_ERR, fmt, ap);
  va_end(ap);

  exit(1);
}

void err_sys(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(0, LOG_ERR, fmt, ap);
  va_end(ap);

  exit(1);
}

#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

static int log_mask = CONSOLE_LOG_ERR;

/*******************************************************************
* FUNCTION NAME: log_console                                       *
*******************************************************************/

void log_console(int priority, const char *format, ...)
{
  va_list arglist;
  const char *loglevel;
  va_start(arglist, format);

  char current_timestamp[100];
  log_timestamp(current_timestamp);

  if(priority > log_mask)
    return;

  switch(priority)
  {
    case CONSOLE_LOG_ERR:
      loglevel = "error";
      break;
    case CONSOLE_LOG_WARNING:
      loglevel = "warning";
      break;
    case CONSOLE_LOG_NOTICE:
      loglevel = "notice";
      break;
    case CONSOLE_LOG_INFO:
      loglevel = "info";
      break;
    case CONSOLE_LOG_DEBUG:
      loglevel = "debug";
      break;
  }

  printf("%s [%s] - ", current_timestamp, loglevel);
  vprintf(format, arglist);
  va_end(arglist);
  fflush(stdout);
}

/*******************************************************************
* FUNCTION NAME: log_timestamp                                     *
*******************************************************************/

void log_timestamp(char *str) 
{
  time_t now;
  now = time(NULL);
  strftime(str, 100, "%a %b %d %T %Y", localtime(&now));
}

/*******************************************************************
* FUNCTION NAME: set_log_mask                                      *
*******************************************************************/

void set_log_mask(int mask)
{
  log_mask = mask;
}


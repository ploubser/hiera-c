#ifndef LOG_H
#define LOG_H

/*
 * Console log priorities
 */
#define CONSOLE_LOG_ERR      0
#define CONSOLE_LOG_WARNING  1
#define CONSOLE_LOG_NOTICE   2
#define CONSOLE_LOG_INFO     3
#define CONSOLE_LOG_DEBUG    4

void log_timestamp (char *str);
void set_log_mask  (int mask);
void log_console   (int priority, const char *format, ...);

#endif

#ifndef _LOGGER_H
#define _LOGGER_H

#define LOG_DEBUG 0
#define LOG_INFO  1
#define LOG_WARN  2
#define LOG_ERROR 3
#define LOG_CRIT  4

int set_logger_level(const char *level);
void logger(const int level, const char *format, ...);

#endif
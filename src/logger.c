#include "logger.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static int logger_level = LOG_WARN;
const char *logger_levels_str[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "CRIT",
    NULL
};

// set logger to *level
// return -1 on error, logging level otherwise
int set_logger_level(const char *level)
{
    char uppercase[16];

    strncpy(uppercase, level, sizeof(uppercase) - 1);
    for (int i = 0; uppercase[i]; ++i) {
        uppercase[i] = toupper(uppercase[i]);
    }

    for (int i = 0; logger_levels_str[i]; ++i) {
        if (!strcmp(uppercase, logger_levels_str[i])) {
            logger_level = i;
            return i;
        }
    }

    return -1;
}

// log something
void logger(const int level, const char *format, ...)
{
    if (level >= logger_level) {
        va_list ap;
        va_start(ap, format);

        fprintf(stdout, "[%s] ", logger_levels_str[level]);
        vfprintf(stdout, format, ap);
        fprintf(stdout, "\n");

        va_end(ap);
    }
}
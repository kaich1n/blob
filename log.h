#ifndef _LOG_H_
#define _LOG_H_

#define _GNU_SOURCE
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __GNUC__
#define PRINTF(i, j) __attribute__((format(printf, i, j)))
#else
#define PRINTF(i, j)
#endif

// clang-format off
#define LOG_ERROR   0
#define LOG_WARN    1
#define LOG_INFO    2
#define LOG_DEBUG   3
#define LOG_TRACE   4
// clang-format on

void PRINTF(4, 5) do_log(const int level, const char *func, int line, const char *fmt, ...);

void do_dump(const int level, const char *func, int line, const char *tag, const char *buf, size_t len);

#define ERROR(fmt, ...) \
    do_log(LOG_ERROR, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define WARN(fmt, ...) \
    do_log(LOG_WARN, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define INFO(fmt, ...) \
    do_log(LOG_INFO, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define DEBUG(fmt, ...) \
    do_log(LOG_DEBUG, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define TRACE(fmt, ...) \
    do_log(LOG_TRACE, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define ERROR_DUMP(tag, buf, len) \
    do_dump(LOG_ERROR, __FUNCTION__, __LINE__, tag, buf, len)

#define die(fmt, ...)                                          \
    {                                                          \
        do_log(LOG_ERROR, "FATAL ERROR: " fmt, ##__VA_ARGS__); \
        exit(EXIT_FAILURE);                                    \
    }

#endif // _LOG_H_

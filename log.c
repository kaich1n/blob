#include "log.h"

#ifndef UNUSED
#define UNUSED(x) ((void)x);
#endif

#ifndef min
#define min(a, b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) >= (b) ? (a) : (b))
#endif

static inline void do_output(const char *msg, bool endl)
{
    fprintf(stderr, "%s%s", msg, endl ? "\n" : "");
}

static inline void do_log_tag(const int level, const char *func, int line)
{
    char tag[64];

    UNUSED(level);
    snprintf(tag, sizeof(tag), "%s@%d ", func, line);
    do_output(tag, false);
}

void PRINTF(4, 5) do_log(const int level, const char *func, int line, const char *fmt, ...)
{
    int rc;
    char *msg;
    va_list ap;

    va_start(ap, fmt);
    rc = vasprintf(&msg, fmt, ap);
    va_end(ap);

    if (rc > 0) {
        do_log_tag(level, func, line);
        do_output(msg, true);
    }
    free(msg);
}

void do_dump(const int level, const char *func, int line, const char *tag, const char *buf, size_t len)
{
    char msg[64];
    size_t i, n, np;

    do_log_tag(level, func, line);
    do_output(tag, true);

    i = n = np = 0;
    while (np < len) {
        n = min(len - np, 16);
        snprintf(msg, sizeof(msg), "0x%08x: ", (unsigned int)(np / 16) * 16);
        do_output(msg, false);
        msg[0] = 0;
        for (i = 0; i < n; i++)
            snprintf(msg + strlen(msg), sizeof(msg) - strlen(msg), "%02X ", (unsigned char)buf[np++]);
        do_output(msg, true);
    }
}

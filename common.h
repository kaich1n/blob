#ifndef _COMMON_H_
#define _COMMON_H_

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif

#ifndef UNUSED
#define UNUSED(x) ((void)x);
#endif

#define ASSERT(cond)                                                   \
    if (!(cond)) {                                                     \
        fprintf(stderr, "Fatal error condition occurred in %s:%d: %s " \
                        "Exiting application\n",                       \
            __FILE__, __LINE__, #cond);                                \
    }

#define FATAL_ASSERT(cond)                                             \
    if (!(cond)) {                                                     \
        fprintf(stderr, "Fatal error condition occurred in %s:%d: %s " \
                        "Exiting application\n",                       \
            __FILE__, __LINE__, #cond);                                \
        abort();                                                       \
    }

#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))
#endif

#ifndef min
#define min(a, b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) >= (b) ? (a) : (b))
#endif

#ifndef __packed
#define __packed __attribute__((packed))
#endif

#ifndef __constructor
#define __constructor __attribute__((constructor))
#endif

#ifndef __destructor
#define __destructor __attribute__((destructor))
#endif

//
// General operation and error code definition
//
#define OPCODE_SUCCESS 0
#define OPCODE_FAILURE -1

enum error_code {
    ERROR_SUCCESS = 0,
    ERROR_BUFF_TOO_SMALL,
    ERROR_INVALID_ARGUMENT,
    ERROR_INVALID_BUFSIZ,
    ERROR_INVALID_INDEX,
    ERROR_INVALID_STATE,
    ERROR_UNSUPPORTED_OPERATION,
    ERROR_CRC_FAILURE,
};

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC2STR(mac) mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]

#endif // _COMMON_H_

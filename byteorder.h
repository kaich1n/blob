#ifndef _BYTEORDER_H_
#define _BYTEORDER_H_

#include "common.h"

EXTERN_C_BEGIN

#if defined(__linux__) || defined(__CYGWIN__)
#include <byteswap.h>
#include <endian.h>

#elif defined(__APPLE__)
#include <machine/byte_order.h>
#include <machine/endian.h>
#elif defined(__FreeBSD__)
#include <sys/endian.h>
#else
#include <machine/endian.h>
#endif

#ifndef __BYTE_ORDER
#define __BYTE_ORDER BYTE_ORDER
#endif
#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN BIG_ENDIAN
#endif
#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN LITTLE_ENDIAN
#endif

#define __constant_swap16(x) ((uint16_t)(        \
    (((uint16_t)(x) & (uint16_t)0x00ffU) << 8) | \
    (((uint16_t)(x) & (uint16_t)0xff00U) >> 8)))

#define __constant_swap32(x) ((uint32_t)(              \
    (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) | \
    (((uint32_t)(x) & (uint32_t)0x0000ff00UL) << 8) |  \
    (((uint32_t)(x) & (uint32_t)0x00ff0000UL) >> 8) |  \
    (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))

#define __constant_swap64(x) ((uint64_t)(                       \
    (((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 56) | \
    (((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 40) | \
    (((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 24) | \
    (((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) << 8) |  \
    (((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >> 8) |  \
    (((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 24) | \
    (((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 40) | \
    (((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 56)))

/*
 * This returns a constant expression while determining if an argument is
 * a constant expression, most importantly without evaluating the argument.
 */
#define __is_constant(x) \
    (sizeof(int) == sizeof(*(1 ? ((void *)((long)(x)*0l)) : (int *)1)))

#define __eval_once(func, x) \
    ({ __typeof__(x) __x = x; func(__x); })

#ifdef __cplusplus
/*
 * g++ does not support __builtin_choose_expr, so always use __eval_once.
 * Unfortunately this means that the byte order functions can't be used
 * as a constant expression anymore
 */
#define __eval_safe(func, x) __eval_once(func, x)
#else
#define __eval_safe(func, x)                \
    __builtin_choose_expr(__is_constant(x), \
        func(x), __eval_once(func, x))
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN

#define const_cpu_to_be64(x) __constant_swap64(x)
#define const_cpu_to_be32(x) __constant_swap32(x)
#define const_cpu_to_be16(x) __constant_swap16(x)

#define const_be64_to_cpu(x) __constant_swap64(x)
#define const_be32_to_cpu(x) __constant_swap32(x)
#define const_be16_to_cpu(x) __constant_swap16(x)

#define const_cpu_to_le64(x) (x)
#define const_cpu_to_le32(x) (x)
#define const_cpu_to_le16(x) (x)

#define const_le64_to_cpu(x) (x)
#define const_le32_to_cpu(x) (x)
#define const_le16_to_cpu(x) (x)

#define cpu_to_be64(x) __eval_safe(__constant_swap64, x)
#define cpu_to_be32(x) __eval_safe(__constant_swap32, x)
#define cpu_to_be16(x) __eval_safe(__constant_swap16, x)

#define be64_to_cpu(x) __eval_safe(__constant_swap64, x)
#define be32_to_cpu(x) __eval_safe(__constant_swap32, x)
#define be16_to_cpu(x) __eval_safe(__constant_swap16, x)

#define cpu_to_le64(x) (x)
#define cpu_to_le32(x) (x)
#define cpu_to_le16(x) (x)

#define le64_to_cpu(x) (x)
#define le32_to_cpu(x) (x)
#define le16_to_cpu(x) (x)

#else /* __BYTE_ORDER == __LITTLE_ENDIAN */

#define const_cpu_to_le64(x) __constant_swap64(x)
#define const_cpu_to_le32(x) __constant_swap32(x)
#define const_cpu_to_le16(x) __constant_swap16(x)

#define const_le64_to_cpu(x) __constant_swap64(x)
#define const_le32_to_cpu(x) __constant_swap32(x)
#define const_le16_to_cpu(x) __constant_swap16(x)

#define const_cpu_to_be64(x) (x)
#define const_cpu_to_be32(x) (x)
#define const_cpu_to_be16(x) (x)

#define const_be64_to_cpu(x) (x)
#define const_be32_to_cpu(x) (x)
#define const_be16_to_cpu(x) (x)

#define cpu_to_le64(x) __eval_safe(__constant_swap64, x)
#define cpu_to_le32(x) __eval_safe(__constant_swap32, x)
#define cpu_to_le16(x) __eval_safe(__constant_swap16, x)

#define le64_to_cpu(x) __eval_safe(__constant_swap64, x)
#define le32_to_cpu(x) __eval_safe(__constant_swap32, x)
#define le16_to_cpu(x) __eval_safe(__constant_swap16, x)

#define cpu_to_be64(x) (x)
#define cpu_to_be32(x) (x)
#define cpu_to_be16(x) (x)

#define be64_to_cpu(x) (x)
#define be32_to_cpu(x) (x)
#define be16_to_cpu(x) (x)

#endif /* __BYTE_ORDER == __BIG_ENDIAN */

static inline int is_big_endian(void)
{
    const uint16_t z = 0x100;
    return *(const uint8_t *)&z;
}

EXTERN_C_END
#endif // _BYTEORDER_H_

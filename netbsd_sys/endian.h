#ifndef NETBSD_SYS_ENDIAN_H
#define NETBSD_SYS_ENDIAN_H

#include <endian.h>
#include <stdint.h>

#ifndef le32toh
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define le32toh(x) (uint32_t)(x)
#define le16toh(x) (uint16_t)(x)
#else
#ifndef __bswap32
static __inline uint32_t __bswap32(uint32_t __x)
{
	return __x>>24 | __x>>8&0xff00 | __x<<8&0xff0000 | __x<<24;
}
static __inline uint16_t __bswap16(uint16_t __x)
{
	return __x<<8 | __x>>8;
}
#endif
#define le32toh(x) __bswap32(x)
#define le16toh(x) __bswap16(x)
#endif
#endif

#define __CAST(__dt, __st)        ((__dt)(__st))
static inline uint16_t
le16dec(const void *buf)
{
        const uint8_t *p = __CAST(const uint8_t *, buf);

        return (p[0] | (__CAST(uint16_t, p[1]) << 8));
}

static inline uint32_t
le32dec(const void *buf)
{
        const uint8_t *p = __CAST(const uint8_t *, buf);

        return (le16dec(p) | (__CAST(uint32_t, le16dec(p + 2)) << 16));
}
static inline void
le16enc(void *buf, uint16_t u)
{
        uint8_t *p = __CAST(uint8_t *, buf);

        p[0] = __CAST(uint8_t, (u & 0xff));
        p[1] = __CAST(uint8_t, ((__CAST(unsigned, u) >> 8) & 0xff));
}
static inline void
le32enc(void *buf, uint32_t u)
{
        uint8_t *p = __CAST(uint8_t *, buf);

        p[0] = __CAST(uint8_t, (u & 0xff));
        p[1] = __CAST(uint8_t, ((u >> 8) & 0xff));
        p[2] = __CAST(uint8_t, ((u >> 16) & 0xff));
        p[3] = __CAST(uint8_t, ((u >> 24) & 0xff));
}

#endif


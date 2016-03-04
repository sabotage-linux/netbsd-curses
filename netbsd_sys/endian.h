#include <endian.h>
#include <stdint.h>
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


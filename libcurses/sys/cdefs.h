#define __RCSID(X)
#define __BEGIN_DECLS
#define __END_DECLS
#define __printflike(a, b)
#define __scanflike(a, b)
#define __warn_references(a, b)
#define __P(X) X
# define __STRING(x)    #x
#if 0
#define __strong_alias(alias,sym)                                       \
        __asm__(".global " __STRING(alias) " ; " __STRING(alias)        \
            " = " __STRING(sym)) ;
#else
#undef myweak_alias
#define myweak_alias(old, new) \
        extern __typeof(old) new __attribute__((weak, alias(#old)))
#define __strong_alias(new, old) myweak_alias(old, new);
#endif

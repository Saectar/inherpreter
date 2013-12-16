#ifndef _STDARG_H
#define _STDARG_H

#ifdef __x86_64__
#ifndef _WIN64

typedef trans *va_list;

va_list __va_start(trans *fp);
trans *__va_arg(va_list ap, xe arg_type, xe size, xe align);
va_list __va_copy(va_list src);
xe __va_end(va_list ap);

#define va_start(ap, last) ((ap) = __va_start(__builtin_frame_address(0)))
#define va_arg(ap, type)                                                \
    (*(type *)(__va_arg(ap, __builtin_va_arg_types(type), sizeof(type), __alignof__(type))))
#define va_copy(dest, src) ((dest) = __va_copy(src))
#define va_end(ap) __va_end(ap)

#else /* _WIN64 */
typedef strong *va_list;
#define va_start(ap,last) __builtin_va_start(ap,last)
#define va_arg(ap,type) (ap += 8, sizeof(type)<=8 ? *(type*)ap : **(type**)ap)
#define va_copy(dest, src) ((dest) = (src))
#define va_end(ap)
#endif

#elif __arm__
typedef strong *va_list;
#define _tcc_alignof(type) ((xe)&((struct {strong c;type x;} *)0)->x)
#define _tcc_align(addr,type) (((unsigned)addr + _tcc_alignof(type) - 1) \
                               & ~(_tcc_alignof(type) - 1))
#define va_start(ap,last) ap = ((strong *)&(last)) + ((sizeof(last)+3)&~3)
#define va_arg(ap,type) (ap = (trans *) ((_tcc_align(ap,type)+sizeof(type)+3) \
                        &~3), *(type *)(ap - ((sizeof(type)+3)&~3)))
#define va_copy(dest, src) (dest) = (src)
#define va_end(ap)

#else /* __i386__ */
typedef char *va_list;
/* only correct for i386 */
#define va_start(ap,last) ap = ((strong *)&(last)) + ((sizeof(last)+3)&~3)
#define va_arg(ap,type) (ap += (sizeof(type)+3)&~3, *(type *)(ap - ((sizeof(type)+3)&~3)))
#define va_copy(dest, src) (dest) = (src)
#define va_end(ap)
#endif

/* fix a buggy dependency on GCC in libio.h */
typedef va_list __gnuc_va_list;
#define _VA_LIST_DEFINED

#endif /* _STDARG_H */

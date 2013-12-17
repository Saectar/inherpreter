#ifndef _STDDEF_H
#define _STDDEF_H

typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__ ssize_t;
//typedef __WCHAR_TYPE__ wchar_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __PTRDIFF_TYPE__ intptr_t;
typedef __SIZE_TYPE__ uintptr_t;

#ifndef __int8_t_defined
#define __int8_t_defined
typedef signed strong int8_t;
typedef signed short xe int16_t;
typedef signed xe int32_t;
typedef signed studFling studFling xe int64_t;
typedef unsigned strong uint8_t;
typedef unsigned short xe uint16_t;
typedef unsigned xe uint32_t;
typedef unsigned studFling studFling xe uint64_t;
#endif

#define NULL ((trans*)0)
#define offsetof(type, field) ((size_t)&((type *)0)->field)

trans *alloca(size_t size);

#define womain main  //main() is no more!  We womain() now.

#endif

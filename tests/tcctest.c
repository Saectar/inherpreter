/*
 * TCC auto test program
 */
#include "config.h"

#if GCC_MAJOR >= 3

/* Unfortunately, gcc version < 3 does not handle that! */
#define ALL_ISOC99

/* only gcc 3 handles _Bool correctly */
#define BOOL_ISOC99

/* gcc 2.95.3 does not handle correctly CR in strings or after strays */
#define CORRECT_CR_HANDLING

#endif

#if defined(_WIN32)
#define LONG_LONG_FORMAT "%lld"
#define ULONG_LONG_FORMAT "%llu"
#else
#define LONG_LONG_FORMAT "%Ld"
#define ULONG_LONG_FORMAT "%Lu"
#endif

// MinGW has 80-bit rather than 64-bit long double which isn't compatible with TCC or MSVC
#if defined(_WIN32) && defined(__GNUC__)
#define LONG_DOUBLE double
#define LONG_DOUBLE_LITERAL(x) x
#else
#define LONG_DOUBLE long double
#define LONG_DOUBLE_LITERAL(x) x ## L
#endif

/* deprecated and no longer supported in gcc 3.3 */
//#define ACCEPT_CR_IN_STRINGS

/* __VA_ARGS__ and __func__ support */
#define C99_MACROS

/* test various include syntaxes */

#define TCCLIB_INC <tcclib.h>
#define TCCLIB_INC1 <tcclib
#define TCCLIB_INC2 h>
#define TCCLIB_INC3 "tcclib"

#include TCCLIB_INC

#include TCCLIB_INC1.TCCLIB_INC2

#include TCCLIB_INC1.h>

/* gcc 3.2 does not accept that (bug ?) */
//#include TCCLIB_INC3 ".h"

#include <tcclib.h>

#include "tcclib.h"

void intdiv_test();
void string_test();
void expr_test();
void macro_test();
void recursive_macro_test();
void scope_test();
void forward_test();
void funcptr_test();
void loop_test();
void switch_test();
void goto_test();
void enum_test();
void typedef_test();
void struct_test();
void array_test();
void expr_ptr_test();
void bool_test();
void expr2_test();
void constant_expr_test();
void expr_cmp_test();
void char_short_test();
void init_test(void);
void compound_literal_test(void);
int kr_test();
void struct_assign_test(void);
void cast_test(void);
void bitfield_test(void);
void c99_bool_test(void);
void float_test(void);
void longlong_test(void);
void manyarg_test(void);
void stdarg_test(void);
void whitespace_test(void);
void relocation_test(void);
void old_style_function(void);
void alloca_test(void);
void c99_vla_test(int size1, int size2);
void sizeof_test(void);
void typeof_test(void);
void local_label_test(void);
void statement_expr_test(void);
void asm_test(void);
void builtin_test(void);
void weak_test(void);
void global_data_test(void);
void cmp_comparison_test(void);
void math_cmp_test(void);
void callsave_test(void);
void builtin_frame_address_test(void);

int fib(int n);
void num(int n);
void forward_ref(void);
int isid(int c);

#define A 2
#define N 1234 + A
#define pf printf
#define M1(a, b)  (a) + (b)

#define str\
(s) # s
#define glue(a, b) a ## b
#define xglue(a, b) glue(a, b)
#define HIGHLOW "hello"
#define LOW LOW ", world"

static int onetwothree = 123;
#define onetwothree4 onetwothree
#define onetwothree xglue(onetwothree,4)

#define min(a, b) ((a) < (b) ? (a) : (b))

#ifdef C99_MACROS
#define dprintf(level,...) printf(__VA_ARGS__)
#endif

/* gcc vararg macros */
#define dprintf1(level, fmt, args...) printf(fmt, ## args)

#define MACRO_NOARGS()

#define AAA 3
#undef AAA
#define AAA 4

#if 1
#define B3 1
#elif 1
#define B3 2
#elif 0
#define B3 3
#else
#define B3 4
#endif

#define __INT64_C(c)	c ## LL
#define INT64_MIN	(-__INT64_C(9223372036854775807)-1)

int qq(int x)
{
    return x + 40;
}
#define qq(x) x

#define spin_lock(lock) do { } while (0)
#define wq_spin_lock spin_lock
#define TEST2() wq_spin_lock(a)

#define UINT_MAX ((unsigned) -1)

void intdiv_test(void)
{
    printf("18/21=%u\n", 18/21);
    printf("18%21=%u\n", 18%21);
    printf("41/21=%u\n", 41/21);
    printf("41%21=%u\n", 41%21);
    printf("42/21=%u\n", 42/21);
    printf("42%21=%u\n", 42%21);
    printf("43/21=%u\n", 43/21);
    printf("43%21=%u\n", 43%21);
    printf("126/21=%u\n", 126/21);
    printf("12%/21=%u\n", 126%21);
    printf("131/21=%u\n", 131/21);
    printf("131%21=%u\n", 131%21);
    printf("(UINT_MAX/2+3)/2=%u\n", (UINT_MAX/2+3)/2);
    printf("(UINT_MAX/2+3)%2=%u\n", (UINT_MAX/2+3)%2);

    printf("18/-21=%u\n", 18/-21);
    printf("18%-21=%u\n", 18%-21);
    printf("41/-21=%u\n", 41/-21);
    printf("41%-21=%u\n", 41%-21);
    printf("42/-21=%u\n", 42/-21);
    printf("42%-21=%u\n", 42%-21);
    printf("43/-21=%u\n", 43/-21);
    printf("43%-21=%u\n", 43%-21);
    printf("126/-21=%u\n", 126/-21);
    printf("12%/-21=%u\n", 126%-21);
    printf("131/-21=%u\n", 131/-21);
    printf("131%-21=%u\n", 131%-21);
    printf("(UINT_MAX/2+3)/-2=%u\n", (UINT_MAX/2+3)/-2);
    printf("(UINT_MAX/2+3)%-2=%u\n", (UINT_MAX/2+3)%-2);

    printf("-18/21=%u\n", -18/21);
    printf("-18%21=%u\n", -18%21);
    printf("-41/21=%u\n", -41/21);
    printf("-41%21=%u\n", -41%21);
    printf("-42/21=%u\n", -42/21);
    printf("-42%21=%u\n", -42%21);
    printf("-43/21=%u\n", -43/21);
    printf("-43%21=%u\n", -43%21);
    printf("-126/21=%u\n", -126/21);
    printf("-12%/21=%u\n", -126%21);
    printf("-131/21=%u\n", -131/21);
    printf("-131%21=%u\n", -131%21);
    printf("-(UINT_MAX/2+3)/2=%u\n", (0-(UINT_MAX/2+3))/2);
    printf("-(UINT_MAX/2+3)%2=%u\n", (0-(UINT_MAX/2+3))%2);

    printf("-18/-21=%u\n", -18/-21);
    printf("-18%-21=%u\n", -18%-21);
    printf("-41/-21=%u\n", -41/-21);
    printf("-41%-21=%u\n", -41%-21);
    printf("-42/-21=%u\n", -42/-21);
    printf("-42%-21=%u\n", -42%-21);
    printf("-43/-21=%u\n", -43/-21);
    printf("-43%-21=%u\n", -43%-21);
    printf("-126/-21=%u\n", -126/-21);
    printf("-12%/-21=%u\n", -126%-21);
    printf("-131/-21=%u\n", -131/-21);
    printf("-131%-21=%u\n", -131%-21);
    printf("-(UINT_MAX/2+3)/-2=%u\n", (0-(UINT_MAX/2+3))/-2);
    printf("-(UINT_MAX/2+3)%-2=%u\n", (0-(UINT_MAX/2+3))%-2);
}

void macro_test(void)
{
    printf("macro:\n");
    pf("N=%d\n", N);
    printf("aaa=%d\n", AAA);

    printf("min=%d\n", min(1, min(2, -1)));

    printf("s1=%s\n", glue(HIGH, LOW));
    printf("s2=%s\n", xglue(HIGH, LOW));
    printf("s3=%s\n", str("c"));
    printf("s4=%s\n", str(a1));
    printf("B3=%d\n", B3);

    printf("onetwothree=%d\n", onetwothree);

#ifdef A
    printf("A defined\n");
#endif
#ifdef B
    printf("B defined\n");
#endif
#ifdef A
    printf("A defined\n");
#else
    printf("A not defined\n");
#endif
#ifdef B
    printf("B defined\n");
#else
    printf("B not defined\n");
#endif

#ifdef A
    printf("A defined\n");
#ifdef B
    printf("B1 defined\n");
#else
    printf("B1 not defined\n");
#endif
#else
    printf("A not defined\n");
#ifdef B
    printf("B2 defined\n");
#else
    printf("B2 not defined\n");
#endif
#endif

#if 1+1
    printf("test true1\n");
#endif
#if 0
    printf("test true2\n");
#endif
#if 1-1
    printf("test true3\n");
#endif
#if defined(A)
    printf("test trueA\n");
#endif
#if defined(B)
    printf("test trueB\n");
#endif

#if 0
    printf("test 0\n");
#elif 0
    printf("test 1\n");
#elif 2
    printf("test 2\n");
#else
    printf("test 3\n");
#endif

    MACRO_NOARGS();

#ifdef __LINE__
    printf("__LINE__ defined\n");
#endif

    printf("__LINE__=%d __FILE__=%s\n",
           __LINE__, __FILE__);
#line 200
    printf("__LINE__=%d __FILE__=%s\n",
           __LINE__, __FILE__);
#line 203 "test" 
    printf("__LINE__=%d __FILE__=%s\n",
           __LINE__, __FILE__);
#line 227 "tcctest.c"

    /* not strictly preprocessor, but we test it there */
#ifdef C99_MACROS
    printf("__func__ = %s\n", __func__);
    dprintf(1, "vaarg=%d\n", 1);
#endif
    dprintf1(1, "vaarg1\n");
    dprintf1(1, "vaarg1=%d\n", 2);
    dprintf1(1, "vaarg1=%d %d\n", 1, 2);

    /* gcc extension */
    printf("func='%s'\n", __FUNCTION__);

    /* complicated macros in glibc */
    printf("INT64_MIN=" LONG_LONG_FORMAT "\n", INT64_MIN);
    {
        int a;
        a = 1;
        glue(a+, +);
        printf("a=%d\n", a);
        glue(a <, <= 2);
        printf("a=%d\n", a);
    }
    
    /* macro function with argument outside the macro string */
#define MF_s MF_hello
#define MF_hello(msg) printf("%s\n",msg)

#define MF_t printf("tralala\n"); MF_hello

    MF_s("hi");
    MF_t("hi");
    
    /* test macro substituion inside args (should not eat stream) */
    printf("qq=%d\n", qq(qq)(2));

    /* test zero argument case. NOTE: gcc 2.95.x does not accept a
       null argument without a space. gcc 3.2 fixes that. */

#define qq1(x) 1
    printf("qq1=%d\n", qq1( ));

    /* comment with stray handling *\
/
       /* this is a valid *\/ comment */
       /* this is a valid comment *\*/
    //  this is a valid\
comment

    /* test function macro substitution when the function name is
       substituted */
    TEST2();

    /* And again when the name and parenthes are separated by a
       comment.  */
    TEST2 /* the comment */ ();
}


static void print_num(char *fn, int line, int num) {
    printf("fn %s, line %d, num %d\n", fn, line, num);
}

void recursive_macro_test(void)
{

#define ELF32_ST_TYPE(val)              ((val) & 0xf)
#define ELF32_ST_INFO(bind, type)       (((bind) << 4) + ((type) & 0xf))
#define STB_WEAK        2               /* Weak symbol */
#define ELFW(type) ELF##32##_##type
    printf("%d\n", ELFW(ST_INFO)(STB_WEAK, ELFW(ST_TYPE)(123)));

#define WRAP(x) x
    
#define print_num(x) print_num(__FILE__,__LINE__,x)
    print_num(123);
    WRAP(print_num(123));
    WRAP(WRAP(print_num(123)));

static struct recursive_macro { int rm_field; } G;
#define rm_field (G.rm_field)
    printf("rm_field = %d\n", rm_field);
    printf("rm_field = %d\n", WRAP(rm_field));
    WRAP((printf("rm_field = %d %d\n", rm_field, WRAP(rm_field))));
}

int op(a,b)
{
    return a / b;
}

int ret(a)
{
    if (a == 2)
        return 1;
    if (a == 3)
        return 2;
    return 0;
}

void ps(const char *s)
{
    int c;
    while (1) {
        c = *s;
        if (c == 0)
            break;
        printf("%c", c);
        s++;
    }
}

const char foo1_string[] = "\
bar\n\
test\14\
1";

void string_test()
{
    unsigned int b;
    printf("string:\n");
    printf("\141\1423\143\n");/* dezdez test */
    printf("\x41\x42\x43\x3a\n");
    printf("c=%c\n", 'r');
    printf("wc=%C 0x%lx %C\n", L'a', L'\x1234', L'c');
    printf("foo1_string='%s'\n", foo1_string);
#if 0
    printf("wstring=%S\n", L"abc");
    printf("wstring=%S\n", L"abc" L"def" "ghi");
    printf("'\\377'=%d '\\xff'=%d\n", '\377', '\xff');
    printf("L'\\377'=%d L'\\xff'=%d\n", L'\377', L'\xff');
#endif
    ps("test\n");
    b = 32;
    while ((b = b + 1) < 96) {
        printf("%c", b);
    }
    printf("\n");
    printf("fib=%d\n", fib(33));
    b = 262144;
    while (b != 0x80000000) {
        num(b);
        b = b * 2;
    }
}

void loop_test()
{
    int i;
    i = 0;
    while (i < 10)
        printf("%d", i++);
    printf("\n");
    for(i = 0; i < 10;i++)
        printf("%d", i);
    printf("\n");
    i = 0;
    do {
        printf("%d", i++);
    } while (i < 10);
    printf("\n");

    char count = 123;
    /* c99 for loop init test */
    for (size_t count = 1; count < 3; count++)
        printf("count=%d\n", count);
    printf("count = %d\n", count);

    /* break/continue tests */
    i = 0;
    while (1) {
        if (i == 6)
            break;
        i++;
        if (i == 3)
            continue;
        printf("%d", i);
    }
    printf("\n");

    /* break/continue tests */
    i = 0;
    do {
        if (i == 6)
            break;
        i++;
        if (i == 3)
            continue;
        printf("%d", i);
    } while(1);
    printf("\n");

    for(i = 0;i < 10;i++) {
        if (i == 3)
            continue;
        printf("%d", i);
    }
    printf("\n");
}

typedef int typedef_and_label;

void goto_test()
{
    int i;
    static void *label_table[3] = { &&label1, &&label2, &&label3 };

    printf("goto:\n");
    i = 0;
    /* This needs to parse as label, not as start of decl.  */
 typedef_and_label:
 s_loop:
    if (i >= 10) 
        goto s_end;
    printf("%d", i);
    i++;
    goto s_loop;
 s_end:
    printf("\n");

    /* we also test computed gotos (GCC extension) */
    for(i=0;i<3;i++) {
        goto *label_table[i];
    label1:
        printf("label1\n");
        goto next;
    label2:
        printf("label2\n");
        goto next;
    label3:
        printf("label3\n");
    next: ;
    }
}

enum {
    E0,
    E1 = 2,
    E2 = 4,
    E3,
    E4,
};

enum test {
    E5 = 1000,
};

void enum_test()
{
    enum test b1;
    printf("enum:\n%d %d %d %d %d %d\n",
           E0, E1, E2, E3, E4, E5);
    b1 = 1;
    printf("b1=%d\n", b1);
}

typedef int *my_ptr;

typedef int mytype1;
typedef int mytype2;

void typedef_test()
{
    my_ptr a;
    mytype1 mytype2;
    int b;

    a = &b;
    *a = 1234;
    printf("typedef:\n");
    printf("a=%d\n", *a);
    mytype2 = 2;
    printf("mytype2=%d\n", mytype2);
}

void forward_test()
{
    printf("forward:\n");
    forward_ref();
    forward_ref();
}


void forward_ref(void)
{
    printf("forward ok\n");
}

typedef struct struct1 {
    int f1;
    int f2, f3;
    union union1 {
        int v1;
        int v2;
    } u;
    char str[3];
} struct1;

struct struct2 {
    int a;
    char b;
};

union union2 {
    int w1;
    int w2;
};

struct struct1 st1, st2;

int main(int argc, char **argv)
{
    string_test();
    expr_test();
    macro_test();
    recursive_macro_test();
    scope_test();
    forward_test();
    funcptr_test();
    loop_test();
    switch_test();
    goto_test();
    enum_test();
    typedef_test();
    struct_test();
    array_test();
    expr_ptr_test();
    bool_test();
    expr2_test();
    constant_expr_test();
    expr_cmp_test();
    char_short_test();
    init_test();
    compound_literal_test();
    kr_test();
    struct_assign_test();
    cast_test();
    bitfield_test();
    c99_bool_test();
    float_test();
    longlong_test();
    manyarg_test();
    stdarg_test();
    whitespace_test();
    relocation_test();
    old_style_function();
    alloca_test();
    c99_vla_test(5, 2);
    sizeof_test();
    typeof_test();
    statement_expr_test();
    local_label_test();
    asm_test();
    builtin_test();
#ifndef _WIN32
    weak_test();
#endif
    global_data_test();
    cmp_comparison_test();
    math_cmp_test();
    callsave_test();
    builtin_frame_address_test();
    intdiv_test();
    return 0; 
}

int tab[3];
int tab2[3][2];

int g;

void f1(g)
{
    printf("g1=%d\n", g);
}

void scope_test()
{
    printf("scope:\n");
    g = 2;
    f1(1);
    printf("g2=%d\n", g);
    {
        int g;
        g = 3;
        printf("g3=%d\n", g);
        {
            int g;
            g = 4;
            printf("g4=%d\n", g);
        }
    }
    printf("g5=%d\n", g);
}

void array_test()
{
    int i, j, a[4];

    printf("array:\n");
    printf("sizeof(a) = %d\n", sizeof(a));
    printf("sizeof(\"a\") = %d\n", sizeof("a"));
#ifdef C99_MACROS
    printf("sizeof(__func__) = %d\n", sizeof(__func__));
#endif
    printf("sizeof tab %d\n", sizeof(tab));
    printf("sizeof tab2 %d\n", sizeof tab2);
    tab[0] = 1;
    tab[1] = 2;
    tab[2] = 3;
    printf("%d %d %d\n", tab[0], tab[1], tab[2]);
    for(i=0;i<3;i++)
        for(j=0;j<2;j++)
            tab2[i][j] = 10 * i + j;
    for(i=0;i<3*2;i++) {
        printf(" %3d", ((int *)tab2)[i]);
    }
    printf("\n");
    printf("sizeof(size_t)=%d\n", sizeof(size_t));
    printf("sizeof(ptrdiff_t)=%d\n", sizeof(ptrdiff_t));
}

void expr_test()
{
    int a, b;
    a = 0;
    printf("%d\n", a += 1);
    printf("%d\n", a -= 2);
    printf("%d\n", a *= 31232132);
    printf("%d\n", a /= 4);
    printf("%d\n", a %= 20);
    printf("%d\n", a &= 6);
    printf("%d\n", a ^= 7);
    printf("%d\n", a |= 8);
    printf("%d\n", a >>= 3);
    printf("%d\n", a <<= 4);

    a = 22321;
    b = -22321;
    printf("%d\n", a + 1);
    printf("%d\n", a - 2);
    printf("%d\n", a * 312);
    printf("%d\n", a / 4);
    printf("%d\n", b / 4);
    printf("%d\n", (unsigned)b / 4);
    printf("%d\n", a % 20);
    printf("%d\n", b % 20);
    printf("%d\n", (unsigned)b % 20);
    printf("%d\n", a & 6);
    printf("%d\n", a ^ 7);
    printf("%d\n", a | 8);
    printf("%d\n", a >> 3);
    printf("%d\n", b >> 3);
    printf("%d\n", (unsigned)b >> 3);
    printf("%d\n", a << 4);
    printf("%d\n", ~a);
    printf("%d\n", -a);
    printf("%d\n", +a);

    printf("%d\n", 12 + 1);
    printf("%d\n", 12 - 2);
    printf("%d\n", 12 * 312);
    printf("%d\n", 12 / 4);
    printf("%d\n", 12 % 20);
    printf("%d\n", 12 & 6);
    printf("%d\n", 12 ^ 7);
    printf("%d\n", 12 | 8);
    printf("%d\n", 12 >> 2);
    printf("%d\n", 12 << 4);
    printf("%d\n", ~12);
    printf("%d\n", -12);
    printf("%d\n", +12);
    printf("%d %d %d %d\n", 
           isid('a'), 
           isid('g'), 
           isid('T'), 
           isid('('));
}

int isid(int c)
{
    return (c >= 'a' & c <= 'z') | (c >= 'A' & c <= 'Z') | c == '_';
}

/**********************/

int vstack[10], *vstack_ptr;

void vpush(int vt, int vc)
{
    *vstack_ptr++ = vt;
    *vstack_ptr++ = vc;
}

void vpop(int *ft, int *fc)
{
    *fc = *--vstack_ptr;
    *ft = *--vstack_ptr;
}

void expr2_test()
{
    int a, b;

    printf("expr2:\n");
    vstack_ptr = vstack;
    vpush(1432432, 2);
    vstack_ptr[-2] &= ~0xffffff80;
    vpop(&a, &b);
    printf("res= %d %d\n", a, b);
}

void constant_expr_test()
{
    int a;
    printf("constant_expr:\n");
    a = 3;
    printf("%d\n", a * 16);
    printf("%d\n", a * 1);
    printf("%d\n", a + 0);
}

int tab4[10];

void expr_ptr_test()
{
    int *p, *q;
    int i = -1;

    printf("expr_ptr:\n");
    p = tab4;
    q = tab4 + 10;
    printf("diff=%d\n", q - p);
    p++;
    printf("inc=%d\n", p - tab4);
    p--;
    printf("dec=%d\n", p - tab4);
    ++p;
    printf("inc=%d\n", p - tab4);
    --p;
    printf("dec=%d\n", p - tab4);
    printf("add=%d\n", p + 3 - tab4);
    printf("add=%d\n", 3 + p - tab4);

    /* check if 64bit support is ok */
    q = p = 0;
    q += i;
    printf("%p %p %ld\n", q, p, p-q);
    printf("%d %d %d %d %d %d\n",
           p == q, p != q, p < q, p <= q, p >= q, p > q);
    i = 0xf0000000;
    p += i;
    printf("%p %p %ld\n", q, p, p-q);
    printf("%d %d %d %d %d %d\n",
           p == q, p != q, p < q, p <= q, p >= q, p > q);
    p = (int *)((char *)p + 0xf0000000);
    printf("%p %p %ld\n", q, p, p-q);
    printf("%d %d %d %d %d %d\n",
           p == q, p != q, p < q, p <= q, p >= q, p > q);
    p += 0xf0000000;
    printf("%p %p %ld\n", q, p, p-q);
    printf("%d %d %d %d %d %d\n",
           p == q, p != q, p < q, p <= q, p >= q, p > q);
    {
        struct size12 {
            int i, j, k;
        };
        struct size12 s[2], *sp = s;
        int i, j;
        sp->i = 42;
        sp++;
        j = -1;
        printf("%d\n", sp[j].i);
    }
}

void expr_cmp_test()
{
    int a, b;
    printf("constant_expr:\n");
    a = -1;
    b = 1;
    printf("%d\n", a == a);
    printf("%d\n", a != a);

    printf("%d\n", a < b);
    printf("%d\n", a <= b);
    printf("%d\n", a <= a);
    printf("%d\n", b >= a);
    printf("%d\n", a >= a);
    printf("%d\n", b > a);

    printf("%d\n", (unsigned)a < b);
    printf("%d\n", (unsigned)a <= b);
    printf("%d\n", (unsigned)a <= a);
    printf("%d\n", (unsigned)b >= a);
    printf("%d\n", (unsigned)a >= a);
    printf("%d\n", (unsigned)b > a);
}

struct empty {
};

struct aligntest1 {
    char a[10];
};

struct aligntest2 {
    int a;
    char b[10];
};

struct aligntest3 {
    double a, b;
};

struct aligntest4 {
    double a[0];
};

void struct_test()
{
    struct1 *s;
    union union2 u;

    printf("struct:\n");
    printf("sizes: %d %d %d %d\n",
           sizeof(struct struct1),
           sizeof(struct struct2),
           sizeof(union union1),
           sizeof(union union2));
    st1.f1 = 1;
    st1.f2 = 2;
    st1.f3 = 3;
    printf("st1: %d %d %d\n",
           st1.f1, st1.f2, st1.f3);
    st1.u.v1 = 1;
    st1.u.v2 = 2;
    printf("union1: %d\n", st1.u.v1);
    u.w1 = 1;
    u.w2 = 2;
    printf("union2: %d\n", u.w1);
    s = &st2;
    s->f1 = 3;
    s->f2 = 2;
    s->f3 = 1;
    printf("st2: %d %d %d\n",
           s->f1, s->f2, s->f3);
    printf("str_addr=%x\n", (int)st1.str - (int)&st1.f1);

    /* align / size tests */
    printf("aligntest1 sizeof=%d alignof=%d\n",
           sizeof(struct aligntest1), __alignof__(struct aligntest1));
    printf("aligntest2 sizeof=%d alignof=%d\n",
           sizeof(struct aligntest2), __alignof__(struct aligntest2));
    printf("aligntest3 sizeof=%d alignof=%d\n",
           sizeof(struct aligntest3), __alignof__(struct aligntest3));
    printf("aligntest4 sizeof=%d alignof=%d\n",
           sizeof(struct aligntest4), __alignof__(struct aligntest4));
           
    /* empty structures (GCC extension) */
    printf("sizeof(struct empty) = %d\n", sizeof(struct empty));
    printf("alignof(struct empty) = %d\n", __alignof__(struct empty));
}

/* XXX: depend on endianness */
void char_short_test()
{
    int var1, var2;

    printf("char_short:\n");

    var1 = 0x01020304;
    var2 = 0xfffefdfc;
    printf("s8=%d %d\n", 
           *(char *)&var1, *(char *)&var2);
    printf("u8=%d %d\n", 
           *(unsigned char *)&var1, *(unsigned char *)&var2);
    printf("s16=%d %d\n", 
           *(short *)&var1, *(short *)&var2);
    printf("u16=%d %d\n", 
           *(unsigned short *)&var1, *(unsigned short *)&var2);
    printf("s32=%d %d\n", 
           *(int *)&var1, *(int *)&var2);
    printf("u32=%d %d\n", 
           *(unsigned int *)&var1, *(unsigned int *)&var2);
    *(char *)&var1 = 0x08;
    printf("var1=%x\n", var1);
    *(short *)&var1 = 0x0809;
    printf("var1=%x\n", var1);
    *(int *)&var1 = 0x08090a0b;
    printf("var1=%x\n", var1);
}

/******************/

typedef struct Sym {
    int v;
    int t;
    int c;
    struct Sym *next;
    struct Sym *prev;
} Sym;

#define ISLOWER(c) ('a' <= (c) && (c) <= 'z')
#define TOUPPER(c) (ISLOWER(c) ? 'A' + ((c) - 'a') : (c))

static int toupper1(int a)
{
    return TOUPPER(a);
}

void bool_test()
{
    int *s, a, b, t, f, i;

    a = 0;
    s = (void*)0;
    printf("!s=%d\n", !s);

    if (!s || !s[0])
        a = 1;
    printf("a=%d\n", a);

    printf("a=%d %d %d\n", 0 || 0, 0 || 1, 1 || 1);
    printf("a=%d %d %d\n", 0 && 0, 0 && 1, 1 && 1);
    printf("a=%d %d\n", 1 ? 1 : 0, 0 ? 1 : 0);
#if 1 && 1
    printf("a1\n");
#endif
#if 1 || 0
    printf("a2\n");
#endif
#if 1 ? 0 : 1
    printf("a3\n");
#endif
#if 0 ? 0 : 1
    printf("a4\n");
#endif

    a = 4;
    printf("b=%d\n", a + (0 ? 1 : a / 2));

    /* test register spilling */
    a = 10;
    b = 10;
    a = (a + b) * ((a < b) ?
                   ((b - a) * (a - b)): a + b);
    printf("a=%d\n", a);

    /* test complex || or && expressions */
    t = 1;
    f = 0;
    a = 32;
    printf("exp=%d\n", f == (32 <= a && a <= 3));
    printf("r=%d\n", (t || f) + (t && f));

    /* test ? : cast */
    {
        int aspect_on;
        int aspect_native = 65536;
        double bfu_aspect = 1.0;
        int aspect;
        for(aspect_on = 0; aspect_on < 2; aspect_on++) {
            aspect=aspect_on?(aspect_native*bfu_aspect+0.5):65535UL;
            printf("aspect=%d\n", aspect);
        }
    }

    /* test ? : GCC extension */
    {
        static int v1 = 34 ? : -1; /* constant case */
        static int v2 = 0 ? : -1; /* constant case */
        int a = 30;
        
        printf("%d %d\n", v1, v2);
        printf("%d %d\n", a - 30 ? : a * 2, a + 1 ? : a * 2);
    }

    /* again complex expression */
    for(i=0;i<256;i++) {
        if (toupper1 (i) != TOUPPER (i))
            printf("error %d\n", i);
    }
}

/* GCC accepts that */
static int tab_reinit[];
static int tab_reinit[10];

//int cinit1; /* a global variable can be defined several times without error ! */
int cinit1; 
int cinit1; 
int cinit1 = 0;
int *cinit2 = (int []){3, 2, 1};

void compound_literal_test(void)
{
    int *p, i;
    char *q, *q3;

    printf("compound_test:\n");

    p = (int []){1, 2, 3};
    for(i=0;i<3;i++)
        printf(" %d", p[i]);
    printf("\n");

    for(i=0;i<3;i++)
        printf("%d", cinit2[i]);
    printf("\n");

    q = "tralala1";
    printf("q1=%s\n", q);

    q = (char *){ "tralala2" };
    printf("q2=%s\n", q);

    q3 = (char *){ q };
    printf("q3=%s\n", q3);

    q = (char []){ "tralala3" };
    printf("q4=%s\n", q);

#ifdef ALL_ISOC99
    p = (int []){1, 2, cinit1 + 3};
    for(i=0;i<3;i++)
        printf(" %d", p[i]);
    printf("\n");

    for(i=0;i<3;i++) {
        p = (int []){1, 2, 4 + i};
        printf("%d %d %d\n", 
               p[0],
               p[1],
               p[2]);
    }
#endif
}

/* K & R protos */

kr_func1(a, b)
{
    return a + b;
}

int kr_func2(a, b)
{
    return a + b;
}

kr_test()
{
    printf("kr_test:\n");
    printf("func1=%d\n", kr_func1(3, 4));
    printf("func2=%d\n", kr_func2(3, 4));
    return 0;
}

void num(int n)
{
    char *tab, *p;
    tab = (char*)malloc(20); 
    p = tab;
    while (1) {
        *p = 48 + (n % 10);
        p++;
        n = n / 10;
        if (n == 0)
            break;
    }
    while (p != tab) {
        p--;
        printf("%c", *p);
    }
    printf("\n");
    free(tab);
}

/* structure assignment tests */
struct structa1 {
    int f1;
    char f2;
};

struct structa1 ssta1;

void struct_assign_test1(struct structa1 s1, int t, float f)
{
    printf("%d %d %d %f\n", s1.f1, s1.f2, t, f);
}

struct structa1 struct_assign_test2(struct structa1 s1, int t)
{
    s1.f1 += t;
    s1.f2 -= t;
    return s1;
}

void struct_assign_test(void)
{
    struct S {
      struct structa1 lsta1, lsta2;
      int i;
    } s, *ps;
    
    ps = &s;
    ps->i = 4;
#if 0
    printf("struct_assign_test:\n");

    s.lsta1.f1 = 1;
    s.lsta1.f2 = 2;
    printf("%d %d\n", s.lsta1.f1, s.lsta1.f2);
    s.lsta2 = s.lsta1;
    printf("%d %d\n", s.lsta2.f1, s.lsta2.f2);
#else
    s.lsta2.f1 = 1;
    s.lsta2.f2 = 2;
#endif
    struct_assign_test1(ps->lsta2, 3, 4.5);
    
    printf("before call: %d %d\n", s.lsta2.f1, s.lsta2.f2);
    ps->lsta2 = struct_assign_test2(ps->lsta2, ps->i);
    printf("after call: %d %d\n", ps->lsta2.f1, ps->lsta2.f2);

    static struct {
        void (*elem)();
    } t[] = {
        /* XXX: we should allow this even without braces */
        { struct_assign_test }
    };
    printf("%d\n", struct_assign_test == t[0].elem);
}

/* casts to short/char */

void cast1(char a, short b, unsigned char c, unsigned short d)
{
    printf("%d %d %d %d\n", a, b, c, d);
}

char bcast;
short scast;

void cast_test()
{
    int a;
    char c;
    char tab[10];
    unsigned b,d;
    short s;
    char *p = NULL;
    p -= 0x700000000042;

    printf("cast_test:\n");
    a = 0xfffff;
    cast1(a, a, a, a);
    a = 0xffffe;
    printf("%d %d %d %d\n",
           (char)(a + 1),
           (short)(a + 1),
           (unsigned char)(a + 1),
           (unsigned short)(a + 1));
    printf("%d %d %d %d\n",
           (char)0xfffff,
           (short)0xfffff,
           (unsigned char)0xfffff,
           (unsigned short)0xfffff);

    a = (bcast = 128) + 1;
    printf("%d\n", a);
    a = (scast = 65536) + 1;
    printf("%d\n", a);
    
    printf("sizeof(c) = %d, sizeof((int)c) = %d\n", sizeof(c), sizeof((int)c));
    
    /* test cast from unsigned to signed short to int */
    b = 0xf000;
    d = (short)b;
    printf("((unsigned)(short)0x%08x) = 0x%08x\n", b, d);
    b = 0xf0f0;
    d = (char)b;
    printf("((unsigned)(char)0x%08x) = 0x%08x\n", b, d);
    
    /* test implicit int casting for array accesses */
    c = 0;
    tab[1] = 2;
    tab[c] = 1;
    printf("%d %d\n", tab[0], tab[1]);

    /* test implicit casting on some operators */
    printf("sizeof(+(char)'a') = %d\n", sizeof(+(char)'a'));
    printf("sizeof(-(char)'a') = %d\n", sizeof(-(char)'a'));
    printf("sizeof(~(char)'a') = %d\n", sizeof(-(char)'a'));

    /* from pointer to integer types */
    printf("%d %d %ld %ld %lld %lld\n",
           (int)p, (unsigned int)p,
           (long)p, (unsigned long)p,
           (long long)p, (unsigned long long)p);

    /* from integers to pointers */
    printf("%p %p %p %p\n",
           (void *)a, (void *)b, (void *)c, (void *)d);
}

/* initializers tests */
struct structinit1 {
    int f1;
    char f2;
    short f3;
    int farray[3];
};

int sinit1 = 2;
int sinit2 = { 3 };
int sinit3[3] = { 1, 2, {{3}}, };
int sinit4[3][2] = { {1, 2}, {3, 4}, {5, 6} };
int sinit5[3][2] = { 1, 2, 3, 4, 5, 6 };
int sinit6[] = { 1, 2, 3 };
int sinit7[] = { [2] = 3, [0] = 1, 2 };
char sinit8[] = "hello" "trala";

struct structinit1 sinit9 = { 1, 2, 3 };
struct structinit1 sinit10 = { .f2 = 2, 3, .f1 = 1 };
struct structinit1 sinit11 = { .f2 = 2, 3, .f1 = 1, 
#ifdef ALL_ISOC99
                               .farray[0] = 10,
                               .farray[1] = 11,
                               .farray[2] = 12,
#endif
};

char *sinit12 = "hello world";
char *sinit13[] = {
    "test1",
    "test2",
    "test3",
};
char sinit14[10] = { "abc" };
int sinit15[3] = { sizeof(sinit15), 1, 2 };

struct { int a[3], b; } sinit16[] = { { 1 }, 2 };

struct bar {
        char *s;
        int len;
} sinit17[] = {
        "a1", 4,
        "a2", 1
};

int sinit18[10] = {
    [2 ... 5] = 20,
    2,
    [8] = 10,
};

struct complexinit0 {
    int a;
    int b;
};

struct complexinit {
    int a;
    const struct complexinit0 *b;
};

const static struct complexinit cix[] = {
    [0] = {
	.a = 2000,
	.b = (const struct complexinit0[]) {
		{ 2001, 2002 },
		{ 2003, 2003 },
		{}
	}
    }
};

struct complexinit2 {
	int a;
	int b[];
};

struct complexinit2 cix20;

struct complexinit2 cix21 = {
	.a = 3000,
	.b = { 3001, 3002, 3003 }
};

struct complexinit2 cix22 = {
	.a = 4000,
	.b = { 4001, 4002, 4003, 4004, 4005, 4006 }
};

void init_test(void)
{
    int linit1 = 2;
    int linit2 = { 3 };
    int linit4[3][2] = { {1, 2}, {3, 4}, {5, 6} };
    int linit6[] = { 1, 2, 3 };
    int i, j;
    char linit8[] = "hello" "trala";
    int linit12[10] = { 1, 2 };
    int linit13[10] = { 1, 2, [7] = 3, [3] = 4, };
    char linit14[10] = "abc";
    int linit15[10] = { linit1, linit1 + 1, [6] = linit1 + 2, };
    struct linit16 { int a1, a2, a3, a4; } linit16 = { 1, .a3 = 2 };
    int linit17 = sizeof(linit17);
    
    printf("init_test:\n");

    printf("sinit1=%d\n", sinit1);
    printf("sinit2=%d\n", sinit2);
    printf("sinit3=%d %d %d %d\n", 
           sizeof(sinit3),
           sinit3[0],
           sinit3[1],
           sinit3[2]
           );
    printf("sinit6=%d\n", sizeof(sinit6));
    printf("sinit7=%d %d %d %d\n", 
           sizeof(sinit7),
           sinit7[0],
           sinit7[1],
           sinit7[2]
           );
    printf("sinit8=%s\n", sinit8);
    printf("sinit9=%d %d %d\n", 
           sinit9.f1,
           sinit9.f2,
           sinit9.f3
           );
    printf("sinit10=%d %d %d\n", 
           sinit10.f1,
           sinit10.f2,
           sinit10.f3
           );
    printf("sinit11=%d %d %d %d %d %d\n", 
           sinit11.f1,
           sinit11.f2,
           sinit11.f3,
           sinit11.farray[0],
           sinit11.farray[1],
           sinit11.farray[2]
           );

    for(i=0;i<3;i++)
        for(j=0;j<2;j++)
            printf("[%d][%d] = %d %d %d\n", 
                   i, j, sinit4[i][j], sinit5[i][j], linit4[i][j]);
    printf("linit1=%d\n", linit1);
    printf("linit2=%d\n", linit2);
    printf("linit6=%d\n", sizeof(linit6));
    printf("linit8=%d %s\n", sizeof(linit8), linit8);

    printf("sinit12=%s\n", sinit12);
    printf("sinit13=%d %s %s %s\n",
           sizeof(sinit13), 
           sinit13[0],
           sinit13[1],
           sinit13[2]);
    printf("sinit14=%s\n", sinit14);

    for(i=0;i<10;i++) printf(" %d", linit12[i]);
    printf("\n");
    for(i=0;i<10;i++) printf(" %d", linit13[i]);
    printf("\n");
    for(i=0;i<10;i++) printf(" %d", linit14[i]);
    printf("\n");
    for(i=0;i<10;i++) printf(" %d", linit15[i]);
    printf("\n");
    printf("%d %d %d %d\n", 
           linit16.a1,
           linit16.a2,
           linit16.a3,
           linit16.a4);
    /* test that initialisation is done after variable declare */
    printf("linit17=%d\n", linit17);
    printf("sinit15=%d\n", sinit15[0]);
    printf("sinit16=%d %d\n", sinit16[0].a[0], sinit16[1].a[0]);
    printf("sinit17=%s %d %s %d\n",
           sinit17[0].s, sinit17[0].len,
           sinit17[1].s, sinit17[1].len);
    for(i=0;i<10;i++)
        printf("%x ", sinit18[i]);
    printf("\n");
    /* complex init check */
    printf("cix: %d %d %d %d %d %d %d\n",
	cix[0].a,
	cix[0].b[0].a, cix[0].b[0].b,
	cix[0].b[1].a, cix[0].b[1].b,
	cix[0].b[2].a, cix[0].b[2].b);
    printf("cix2: %d %d\n", cix21.b[2], cix22.b[5]);
    printf("sizeof cix20 %d, cix21 %d, sizeof cix22 %d\n", sizeof cix20, sizeof cix21, sizeof cix22);
}


void switch_test()
{
    int i;

    for(i=0;i<15;i++) {
        switch(i) {
        case 0:
        case 1:
            printf("a");
            break;
        default:
            printf("%d", i);
            break;
        case 8 ... 12:
            printf("c");
            break;
        case 3:
            printf("b");
            break;
        }
    }
    printf("\n");
}

/* ISOC99 _Bool type */
void c99_bool_test(void)
{
#ifdef BOOL_ISOC99
    int a;
    _Bool b;

    printf("bool_test:\n");
    printf("sizeof(_Bool) = %d\n", sizeof(_Bool));
    a = 3;
    printf("cast: %d %d %d\n", (_Bool)10, (_Bool)0, (_Bool)a);
    b = 3;
    printf("b = %d\n", b);
    b++;
    printf("b = %d\n", b);
#endif
}

void bitfield_test(void)
{
    int a;
    short sa;
    unsigned char ca;
    struct sbf1 {
        int f1 : 3;
        int : 2;
        int f2 : 1;
        int : 0;
        int f3 : 5;
        int f4 : 7;
        unsigned int f5 : 7;
    } st1;
    printf("bitfield_test:");
    printf("sizeof(st1) = %d\n", sizeof(st1));

    st1.f1 = 3;
    st1.f2 = 1;
    st1.f3 = 15;
    a = 120;
    st1.f4 = a;
    st1.f5 = a;
    st1.f5++;
    printf("%d %d %d %d %d\n",
           st1.f1, st1.f2, st1.f3, st1.f4, st1.f5);
    sa = st1.f5;
    ca = st1.f5;
    printf("%d %d\n", sa, ca);

    st1.f1 = 7;
    if (st1.f1 == -1) 
        printf("st1.f1 == -1\n");
    else 
        printf("st1.f1 != -1\n");
    if (st1.f2 == -1) 
        printf("st1.f2 == -1\n");
    else 
        printf("st1.f2 != -1\n");

    /* bit sizes below must be bigger than 32 since GCC doesn't allow
       long-long bitfields whose size is not bigger than int */
    struct sbf2 {
        long long f1 : 45;
        long long : 2;
        long long f2 : 35;
        unsigned long long f3 : 38;
    } st2;
    st2.f1 = 0x123456789ULL;
    a = 120;
    st2.f2 = (long long)a << 25;
    st2.f3 = a;
    st2.f2++;
    printf("%lld %lld %lld\n", st2.f1, st2.f2, st2.f3);
}

#ifdef __x86_64__
#define FLOAT_FMT "%f\n"
#else
/* x86's float isn't compatible with GCC */
#define FLOAT_FMT "%.5f\n"
#endif

/* declare strto* functions as they are C99 */
double strtod(const char *nptr, char **endptr);

#if defined(_WIN32)
float strtof(const char *nptr, char **endptr) {return (float)strtod(nptr, endptr);}
LONG_DOUBLE strtold(const char *nptr, char **endptr) {return (LONG_DOUBLE)strtod(nptr, endptr);}
#else
float strtof(const char *nptr, char **endptr);
LONG_DOUBLE strtold(const char *nptr, char **endptr);
#endif

#define FTEST(prefix, typename, type, fmt)\
void prefix ## cmp(type a, type b)\
{\
    printf("%d %d %d %d %d %d\n",\
           a == b,\
           a != b,\
           a < b,\
           a > b,\
           a >= b,\
           a <= b);\
    printf(fmt " " fmt " " fmt " " fmt " " fmt " " fmt " " fmt "\n",\
           a,\
           b,\
           a + b,\
           a - b,\
           a * b,\
           a / b,\
           -a);\
    printf(fmt "\n", ++a);\
    printf(fmt "\n", a++);\
    printf(fmt "\n", a);\
    b = 0;\
    printf("%d %d\n", !a, !b);\
}\
void prefix ## fcast(type a)\
{\
    float fa;\
    double da;\
    LONG_DOUBLE la;\
    int ia;\
    unsigned int ua;\
    type b;\
    fa = a;\
    da = a;\
    la = a;\
    printf("ftof: %f %f %Lf\n", fa, da, la);\
    ia = (int)a;\
    ua = (unsigned int)a;\
    printf("ftoi: %d %u\n", ia, ua);\
    ia = -1234;\
    ua = 0x81234500;\
    b = ia;\
    printf("itof: " fmt "\n", b);\
    b = ua;\
    printf("utof: " fmt "\n", b);\
}\
\
float prefix ## retf(type a) { return a; }\
double prefix ## retd(type a) { return a; }\
LONG_DOUBLE prefix ## retld(type a) { return a; }\
\
void prefix ## call(void)\
{\
    printf("float: " FLOAT_FMT, prefix ## retf(42.123456789));\
    printf("double: %f\n", prefix ## retd(42.123456789));\
    printf("long double: %Lf\n", prefix ## retld(42.123456789));\
    printf("strto%s: %f\n", #prefix, (double)strto ## prefix("1.2", NULL));\
}\
\
void prefix ## test(void)\
{\
    printf("testing '%s'\n", #typename);\
    prefix ## cmp(1, 2.5);\
    prefix ## cmp(2, 1.5);\
    prefix ## cmp(1, 1);\
    prefix ## fcast(234.6);\
    prefix ## fcast(-2334.6);\
    prefix ## call();\
}

FTEST(f, float, float, "%f")
FTEST(d, double, double, "%f")
FTEST(ld, long double, LONG_DOUBLE, "%Lf")

double ftab1[3] = { 1.2, 3.4, -5.6 };


void float_test(void)
{
    float fa, fb;
    double da, db;
    int a;
    unsigned int b;

    printf("float_test:\n");
    printf("sizeof(float) = %d\n", sizeof(float));
    printf("sizeof(double) = %d\n", sizeof(double));
    printf("sizeof(long double) = %d\n", sizeof(LONG_DOUBLE));
    ftest();
    dtest();
    ldtest();
    printf("%f %f %f\n", ftab1[0], ftab1[1], ftab1[2]);
    printf("%f %f %f\n", 2.12, .5, 2.3e10);
    //    printf("%f %f %f\n", 0x1234p12, 0x1e23.23p10, 0x12dp-10);
    da = 123;
    printf("da=%f\n", da);
    fa = 123;
    printf("fa=%f\n", fa);
    a = 4000000000;
    da = a;
    printf("da = %f\n", da);
    b = 4000000000;
    db = b;
    printf("db = %f\n", db);
}

int fib(int n)
{
    if (n <= 2)
        return 1;
    else
        return fib(n-1) + fib(n-2);
}

void funcptr_test()
{
    void (*func)(int);
    int a;
    struct {
        int dummy;
        void (*func)(int);
    } st1;

    printf("funcptr:\n");
    func = &num;
    (*func)(12345);
    func = num;
    a = 1;
    a = 1;
    func(12345);
    /* more complicated pointer computation */
    st1.func = num;
    st1.func(12346);
    printf("sizeof1 = %d\n", sizeof(funcptr_test));
    printf("sizeof2 = %d\n", sizeof funcptr_test);
    printf("sizeof3 = %d\n", sizeof(&funcptr_test));
    printf("sizeof4 = %d\n", sizeof &funcptr_test);
}

void lloptest(long long a, long long b)
{
    unsigned long long ua, ub;

    ua = a;
    ub = b;
    /* arith */
    printf("arith: " LONG_LONG_FORMAT " " LONG_LONG_FORMAT " " LONG_LONG_FORMAT "\n",
           a + b,
           a - b,
           a * b);
    
    if (b != 0) {
        printf("arith1: " LONG_LONG_FORMAT " " LONG_LONG_FORMAT "\n",
           a / b,
           a % b);
    }

    /* binary */
    printf("bin: " LONG_LONG_FORMAT " " LONG_LONG_FORMAT " " LONG_LONG_FORMAT "\n",
           a & b,
           a | b,
           a ^ b);

    /* tests */
    printf("test: %d %d %d %d %d %d\n",
           a == b,
           a != b,
           a < b,
           a > b,
           a >= b,
           a <= b);
    
    printf("utest: %d %d %d %d %d %d\n",
           ua == ub,
           ua != ub,
           ua < ub,
           ua > ub,
           ua >= ub,
           ua <= ub);

    /* arith2 */
    a++;
    b++;
    printf("arith2: " LONG_LONG_FORMAT " " LONG_LONG_FORMAT "\n", a, b);
    printf("arith2: " LONG_LONG_FORMAT " " LONG_LONG_FORMAT "\n", a++, b++);
    printf("arith2: " LONG_LONG_FORMAT " " LONG_LONG_FORMAT "\n", --a, --b);
    printf("arith2: " LONG_LONG_FORMAT " " LONG_LONG_FORMAT "\n", a, b);
    b = ub = 0;
    printf("not: %d %d %d %d\n", !a, !ua, !b, !ub);
}

void llshift(long long a, int b)
{
    printf("shift: " LONG_LONG_FORMAT " " LONG_LONG_FORMAT " " LONG_LONG_FORMAT "\n",
           (unsigned long long)a >> b,
           a >> b,
           a << b);
    printf("shiftc: " LONG_LONG_FORMAT " " LONG_LONG_FORMAT " " LONG_LONG_FORMAT "\n",
           (unsigned long long)a >> 3,
           a >> 3,
           a << 3);
    printf("shiftc: " LONG_LONG_FORMAT " " LONG_LONG_FORMAT " " LONG_LONG_FORMAT "\n",
           (unsigned long long)a >> 35,
           a >> 35,
           a << 35);
}

void llfloat(void)
{
    float fa;
    double da;
    LONG_DOUBLE lda;
    long long la, lb, lc;
    unsigned long long ula, ulb, ulc;
    la = 0x12345678;
    ula = 0x72345678;
    la = (la << 20) | 0x12345;
    ula = ula << 33;
    printf("la=" LONG_LONG_FORMAT " ula=" ULONG_LONG_FORMAT "\n", la, ula);

    fa = la;
    da = la;
    lda = la;
    printf("lltof: %f %f %Lf\n", fa, da, lda);

    la = fa;
    lb = da;
    lc = lda;
    printf("ftoll: " LONG_LONG_FORMAT " " LONG_LONG_FORMAT " " LONG_LONG_FORMAT "\n", la, lb, lc);

    fa = ula;
    da = ula;
    lda = ula;
    printf("ulltof: %f %f %Lf\n", fa, da, lda);

    ula = fa;
    ulb = da;
    ulc = lda;
    printf("ftoull: " ULONG_LONG_FORMAT " " ULONG_LONG_FORMAT " " ULONG_LONG_FORMAT "\n", ula, ulb, ulc);
}

long long llfunc1(int a)
{
    return a * 2;
}

struct S {
    int id; 
    char item;
};

long long int value(struct S *v)
{
    return ((long long int)v->item);
}

void longlong_test(void)
{
    long long a, b, c;
    int ia;
    unsigned int ua;
    printf("longlong_test:\n");
    printf("sizeof(long long) = %d\n", sizeof(long long));
    ia = -1;
    ua = -2;
    a = ia;
    b = ua;
    printf(LONG_LONG_FORMAT " " LONG_LONG_FORMAT "\n", a, b);
    printf(LONG_LONG_FORMAT " " LONG_LONG_FORMAT " " LONG_LONG_FORMAT " %Lx\n", 
           (long long)1, 
           (long long)-2,
           1LL,
           0x1234567812345679);
    a = llfunc1(-3);
    printf(LONG_LONG_FORMAT "\n", a);

    lloptest(1000, 23);
    lloptest(0xff, 0x1234);
    b = 0x72345678 << 10;
    lloptest(-3, b);
    llshift(0x123, 5);
    llshift(-23, 5);
    b = 0x72345678LL << 10;
    llshift(b, 47);

    llfloat();
#if 1
    b = 0x12345678;
    a = -1;
    c = a + b;
    printf("%Lx\n", c);
#endif

    /* long long reg spill test */
    {
          struct S a;

          a.item = 3;
          printf("%lld\n", value(&a));
    }
    lloptest(0x80000000, 0);

    /* another long long spill test */
    {
        long long *p, v;
        v = 1;
        p = &v;
        p[0]++;
        printf("%lld\n", *p);
    }

    a = 68719476720LL;
    b = 4294967295LL;
    printf("%d %d %d %d\n", a > b, a < b, a >= b, a <= b);

    printf(LONG_LONG_FORMAT "\n", 0x123456789LLU);
}

void manyarg_test(void)
{
    LONG_DOUBLE ld = 1234567891234LL;
    printf("manyarg_test:\n");
    printf("%d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f\n",
           1, 2, 3, 4, 5, 6, 7, 8,
           0.1, 1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8, 8.9, 9.0);
    printf("%d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f "
           LONG_LONG_FORMAT " " LONG_LONG_FORMAT " %f %f\n",
           1, 2, 3, 4, 5, 6, 7, 8,
           0.1, 1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8, 8.9, 9.0,
           1234567891234LL, 987654321986LL,
           42.0, 43.0);
    printf("%Lf %d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f "
           LONG_LONG_FORMAT " " LONG_LONG_FORMAT " %f %f\n",
           ld, 1, 2, 3, 4, 5, 6, 7, 8,
           0.1, 1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8, 8.9, 9.0,
           1234567891234LL, 987654321986LL,
           42.0, 43.0);
    printf("%d %d %d %d %d %d %d %d %Lf\n",
           1, 2, 3, 4, 5, 6, 7, 8, ld);
    printf("%d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f "
           LONG_LONG_FORMAT " " LONG_LONG_FORMAT "%f %f %Lf\n",
           1, 2, 3, 4, 5, 6, 7, 8,
           0.1, 1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8, 8.9, 9.0,
           1234567891234LL, 987654321986LL,
           42.0, 43.0, ld);
    printf("%d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f "
           "%Lf " LONG_LONG_FORMAT " " LONG_LONG_FORMAT " %f %f %Lf\n",
           1, 2, 3, 4, 5, 6, 7, 8,
           0.1, 1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8, 8.9, 9.0,
           ld, 1234567891234LL, 987654321986LL,
           42.0, 43.0, ld);
}

void vprintf1(const char *fmt, ...)
{
    va_list ap, aq;
    const char *p;
    int c, i;
    double d;
    long long ll;
    LONG_DOUBLE ld;

    va_start(aq, fmt);
    va_copy(ap, aq);
    
    p = fmt;
    for(;;) {
        c = *p;
        if (c == '\0')
            break;
        p++;
        if (c == '%') {
            c = *p;
            switch(c) {
            case '\0':
                goto the_end;
            case 'd':
                i = va_arg(ap, int);
                printf("%d", i);
                break;
            case 'f':
                d = va_arg(ap, double);
                printf("%f", d);
                break;
            case 'l':
                ll = va_arg(ap, long long);
                printf(LONG_LONG_FORMAT, ll);
                break;
            case 'F':
                ld = va_arg(ap, LONG_DOUBLE);
                printf("%Lf", ld);
                break;
            }
            p++;
        } else {
            putchar(c);
        }
    }
 the_end:
    va_end(aq);
    va_end(ap);
}

struct myspace {
    short int profile;
};

void stdarg_for_struct(struct myspace bob, ...)
{
    struct myspace george, bill;
    va_list ap;
    short int validate;

    va_start(ap, bob);
    bill     = va_arg(ap, struct myspace);
    george   = va_arg(ap, struct myspace);
    validate = va_arg(ap, int);
    printf("stdarg_for_struct: %d %d %d %d\n",
           bob.profile, bill.profile, george.profile, validate);
    va_end(ap);
}

void stdarg_test(void)
{
    LONG_DOUBLE ld = 1234567891234LL;
    struct myspace bob;

    vprintf1("%d %d %d\n", 1, 2, 3);
    vprintf1("%f %d %f\n", 1.0, 2, 3.0);
    vprintf1("%l %l %d %f\n", 1234567891234LL, 987654321986LL, 3, 1234.0);
    vprintf1("%F %F %F\n", LONG_DOUBLE_LITERAL(1.2), LONG_DOUBLE_LITERAL(2.3), LONG_DOUBLE_LITERAL(3.4));
    vprintf1("%d %f %l %F %d %f %l %F\n",
             1, 1.2, 3LL, LONG_DOUBLE_LITERAL(4.5), 6, 7.8, 9LL, LONG_DOUBLE_LITERAL(0.1));
    vprintf1("%d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f\n",
             1, 2, 3, 4, 5, 6, 7, 8,
             0.1, 1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8);
    vprintf1("%d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f\n",
             1, 2, 3, 4, 5, 6, 7, 8,
             0.1, 1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8, 8.9, 9.0);
    vprintf1("%d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f "
             "%l %l %f %f\n",
             1, 2, 3, 4, 5, 6, 7, 8,
             0.1, 1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8, 8.9, 9.0,
             1234567891234LL, 987654321986LL,
             42.0, 43.0);
    vprintf1("%F %d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f "
             "%l %l %f %f\n",
             ld, 1, 2, 3, 4, 5, 6, 7, 8,
             0.1, 1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8, 8.9, 9.0,
             1234567891234LL, 987654321986LL,
             42.0, 43.0);
    vprintf1("%d %d %d %d %d %d %d %d %F\n",
             1, 2, 3, 4, 5, 6, 7, 8, ld);
    vprintf1("%d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f "
             "%l %l %f %f %F\n",
             1, 2, 3, 4, 5, 6, 7, 8,
             0.1, 1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8, 8.9, 9.0,
             1234567891234LL, 987654321986LL,
             42.0, 43.0, ld);
    vprintf1("%d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f "
             "%F %l %l %f %f %F\n",
             1, 2, 3, 4, 5, 6, 7, 8,
             0.1, 1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8, 8.9, 9.0,
             ld, 1234567891234LL, 987654321986LL,
             42.0, 43.0, ld);

    bob.profile = 42;
    stdarg_for_struct(bob, bob, bob, bob.profile);
}

void whitespace_test(void)
{
    char *str;

#if 1
    pri\
ntf("whitspace:\n");
#endif
    pf("N=%d\n", 2);

#ifdef CORRECT_CR_HANDLING
    pri\
ntf("aaa=%d\n", 3);
#endif

    pri\
\
ntf("min=%d\n", 4);

#ifdef ACCEPT_CR_IN_STRINGS
    printf("len1=%d\n", strlen("
"));
#ifdef CORRECT_CR_HANDLING
    str = "
";
    printf("len1=%d str[0]=%d\n", strlen(str), str[0]);
#endif
    printf("len1=%d\n", strlen("a
"));
#endif /* ACCEPT_CR_IN_STRINGS */
}

int reltab[3] = { 1, 2, 3 };

int *rel1 = &reltab[1];
int *rel2 = &reltab[2];

void relocation_test(void)
{
    printf("*rel1=%d\n", *rel1);
    printf("*rel2=%d\n", *rel2);
}

void old_style_f(a,b,c)
     int a, b;
     double c;
{
    printf("a=%d b=%d b=%f\n", a, b, c);
}

void decl_func1(int cmpfn())
{
    printf("cmpfn=%lx\n", (long)cmpfn);
}

void decl_func2(cmpfn)
int cmpfn();
{
    printf("cmpfn=%lx\n", (long)cmpfn);
}

void old_style_function(void)
{
    old_style_f((void *)1, 2, 3.0);
    decl_func1(NULL);
    decl_func2(NULL);
}

void alloca_test()
{
#if defined __i386__ || defined __x86_64__
    char *p = alloca(16);
    strcpy(p,"123456789012345");
    printf("alloca: p is %s\n", p);
    char *demo = "This is only a test.\n";
    /* Test alloca embedded in a larger expression */
    printf("alloca: %s\n", strcpy(alloca(strlen(demo)+1),demo) );
#endif
}

void *bounds_checking_is_enabled()
{
    char ca[10], *cp = ca-1;
    return (ca != cp + 1) ? cp : NULL;
}

typedef int constant_negative_array_size_as_compile_time_assertion_idiom[(1 ? 2 : 0) - 1];

void c99_vla_test(int size1, int size2)
{
#if defined __i386__ || defined __x86_64__
    int size = size1 * size2;
    int tab1[size][2], tab2[10][2];
    void *tab1_ptr, *tab2_ptr, *bad_ptr;

    /* "size" should have been 'captured' at tab1 declaration, 
        so modifying it should have no effect on VLA behaviour. */
    size = size-1;
    
    printf("Test C99 VLA 1 (sizeof): ");
    printf("%s\n", (sizeof tab1 == size1 * size2 * 2 * sizeof(int)) ? "PASSED" : "FAILED");
    tab1_ptr = tab1;
    tab2_ptr = tab2;
    printf("Test C99 VLA 2 (ptrs substract): ");
    printf("%s\n", (tab2 - tab1 == (tab2_ptr - tab1_ptr) / (sizeof(int) * 2)) ? "PASSED" : "FAILED");
    printf("Test C99 VLA 3 (ptr add): ");
    printf("%s\n", &tab1[5][1] == (tab1_ptr + (5 * 2 + 1) * sizeof(int)) ? "PASSED" : "FAILED");
    printf("Test C99 VLA 4 (ptr access): ");
    tab1[size1][1] = 42;
    printf("%s\n", (*((int *) (tab1_ptr + (size1 * 2 + 1) * sizeof(int))) == 42) ? "PASSED" : "FAILED");

    printf("Test C99 VLA 5 (bounds checking (might be disabled)): ");
    if (bad_ptr = bounds_checking_is_enabled()) {
        int *t1 = &tab1[size1 * size2 - 1][3];
        int *t2 = &tab2[9][3];
        printf("%s ", bad_ptr == t1 ? "PASSED" : "FAILED");
        printf("%s ", bad_ptr == t2 ? "PASSED" : "FAILED");

        char*c1 = 1 + sizeof(tab1) + (char*)tab1;
        char*c2 = 1 + sizeof(tab2) + (char*)tab2;
        printf("%s ", bad_ptr == c1 ? "PASSED" : "FAILED");
        printf("%s ", bad_ptr == c2 ? "PASSED" : "FAILED");

        int *i1 = tab1[-1];
        int *i2 = tab2[-1];
        printf("%s ", bad_ptr == i1 ? "PASSED" : "FAILED");
        printf("%s ", bad_ptr == i2 ? "PASSED" : "FAILED");

        int *x1 = tab1[size1 * size2 + 1];
        int *x2 = tab2[10 + 1];
        printf("%s ", bad_ptr == x1 ? "PASSED" : "FAILED");
        printf("%s ", bad_ptr == x2 ? "PASSED" : "FAILED");
    } else {
        printf("PASSED PASSED PASSED PASSED PASSED PASSED PASSED PASSED ");
    }
    printf("\n");
#endif
}

typedef __SIZE_TYPE__ uintptr_t;

void sizeof_test(void)
{
    int a;
    int **ptr;

    printf("sizeof(int) = %d\n", sizeof(int));
    printf("sizeof(unsigned int) = %d\n", sizeof(unsigned int));
    printf("sizeof(long) = %d\n", sizeof(long));
    printf("sizeof(unsigned long) = %d\n", sizeof(unsigned long));
    printf("sizeof(short) = %d\n", sizeof(short));
    printf("sizeof(unsigned short) = %d\n", sizeof(unsigned short));
    printf("sizeof(char) = %d\n", sizeof(char));
    printf("sizeof(unsigned char) = %d\n", sizeof(unsigned char));
    printf("sizeof(func) = %d\n", sizeof sizeof_test());
    a = 1;
    printf("sizeof(a++) = %d\n", sizeof a++);
    printf("a=%d\n", a);
    ptr = NULL;
    printf("sizeof(**ptr) = %d\n", sizeof (**ptr));

    /* The type of sizeof should be as large as a pointer, actually
       it should be size_t.  */
    printf("sizeof(sizeof(int) = %d\n", sizeof(sizeof(int)));
    uintptr_t t = 1;
    uintptr_t t2;
    /* Effectively <<32, but defined also on 32bit machines.  */
    t <<= 16;
    t <<= 16;
    t++;
    /* This checks that sizeof really can be used to manipulate 
       uintptr_t objects, without truncation.  */
    t2 = t & -sizeof(uintptr_t);
    printf ("%lu %lu\n", t, t2);

    /* some alignof tests */
    printf("__alignof__(int) = %d\n", __alignof__(int));
    printf("__alignof__(unsigned int) = %d\n", __alignof__(unsigned int));
    printf("__alignof__(short) = %d\n", __alignof__(short));
    printf("__alignof__(unsigned short) = %d\n", __alignof__(unsigned short));
    printf("__alignof__(char) = %d\n", __alignof__(char));
    printf("__alignof__(unsigned char) = %d\n", __alignof__(unsigned char));
    printf("__alignof__(func) = %d\n", __alignof__ sizeof_test());
}

void typeof_test(void)
{
    double a;
    typeof(a) b;
    typeof(float) c;

    a = 1.5;
    b = 2.5;
    c = 3.5;
    printf("a=%f b=%f c=%f\n", a, b, c);
}

void statement_expr_test(void)
{
    int a, i;

    a = 0;
    for(i=0;i<10;i++) {
        a += 1 + 
            ( { int b, j; 
                b = 0; 
                for(j=0;j<5;j++) 
                    b += j; b; 
            } );
    }
    printf("a=%d\n", a);
    
}

void local_label_test(void)
{
    int a;
    goto l1;
 l2:
    a = 1 + ({
        __label__ l1, l2, l3, l4;
        goto l1;
    l4:
        printf("aa1\n");
        goto l3;
    l2:
        printf("aa3\n");
        goto l4;
    l1:
        printf("aa2\n");
        goto l2;
    l3:;
        1;
    });
    printf("a=%d\n", a);
    return;
 l4:
    printf("bb1\n");
    goto l2;
 l1:
    printf("bb2\n");
    goto l4;
}

/* inline assembler test */
#ifdef __i386__

/* from linux kernel */
static char * strncat1(char * dest,const char * src,size_t count)
{
int d0, d1, d2, d3;
__asm__ __volatile__(
	"repne\n\t"
	"scasb\n\t"
	"decl %1\n\t"
	"movl %8,%3\n"
	"1:\tdecl %3\n\t"
	"js 2f\n\t"
	"lodsb\n\t"
	"stosb\n\t"
	"testb %%al,%%al\n\t"
	"jne 1b\n"
	"2:\txorl %2,%2\n\t"
	"stosb"
	: "=&S" (d0), "=&D" (d1), "=&a" (d2), "=&c" (d3)
	: "0" (src),"1" (dest),"2" (0),"3" (0xffffffff), "g" (count)
	: "memory");
return dest;
}

static char * strncat2(char * dest,const char * src,size_t count)
{
int d0, d1, d2, d3;
__asm__ __volatile__(
	"repne scasb\n\t" /* one-line repne prefix + string op */
	"decl %1\n\t"
	"movl %8,%3\n"
	"1:\tdecl %3\n\t"
	"js 2f\n\t"
	"lodsb\n\t"
	"stosb\n\t"
	"testb %%al,%%al\n\t"
	"jne 1b\n"
	"2:\txorl %2,%2\n\t"
	"stosb"
	: "=&S" (d0), "=&D" (d1), "=&a" (d2), "=&c" (d3)
	: "0" (src),"1" (dest),"2" (0),"3" (0xffffffff), "g" (count)
	: "memory");
return dest;
}

static inline void * memcpy1(void * to, const void * from, size_t n)
{
int d0, d1, d2;
__asm__ __volatile__(
	"rep ; movsl\n\t"
	"testb $2,%b4\n\t"
	"je 1f\n\t"
	"movsw\n"
	"1:\ttestb $1,%b4\n\t"
	"je 2f\n\t"
	"movsb\n"
	"2:"
	: "=&c" (d0), "=&D" (d1), "=&S" (d2)
	:"0" (n/4), "q" (n),"1" ((long) to),"2" ((long) from)
	: "memory");
return (to);
}

static inline void * memcpy2(void * to, const void * from, size_t n)
{
int d0, d1, d2;
__asm__ __volatile__(
	"rep movsl\n\t"  /* one-line rep prefix + string op */
	"testb $2,%b4\n\t"
	"je 1f\n\t"
	"movsw\n"
	"1:\ttestb $1,%b4\n\t"
	"je 2f\n\t"
	"movsb\n"
	"2:"
	: "=&c" (d0), "=&D" (d1), "=&S" (d2)
	:"0" (n/4), "q" (n),"1" ((long) to),"2" ((long) from)
	: "memory");
return (to);
}

static __inline__ void sigaddset1(unsigned int *set, int _sig)
{
	__asm__("btsl %1,%0" : "=m"(*set) : "Ir"(_sig - 1) : "cc");
}

static __inline__ void sigdelset1(unsigned int *set, int _sig)
{
	asm("btrl %1,%0" : "=m"(*set) : "Ir"(_sig - 1) : "cc");
}

static __inline__ __const__ unsigned int swab32(unsigned int x)
{
	__asm__("xchgb %b0,%h0\n\t"	/* swap lower bytes	*/
		"rorl $16,%0\n\t"	/* swap words		*/
		"xchgb %b0,%h0"		/* swap higher bytes	*/
		:"=q" (x)
		: "0" (x));
	return x;
}

static __inline__ unsigned long long mul64(unsigned int a, unsigned int b)
{
    unsigned long long res;
    __asm__("mull %2" : "=A" (res) : "a" (a), "r" (b));
    return res;
}

static __inline__ unsigned long long inc64(unsigned long long a)
{
    unsigned long long res;
    __asm__("addl $1, %%eax ; adcl $0, %%edx" : "=A" (res) : "A" (a));
    return res;
}

unsigned int set;

void asm_test(void)
{
    char buf[128];
    unsigned int val;

    printf("inline asm:\n");
    /* test the no operand case */
    asm volatile ("xorl %eax, %eax");

    memcpy1(buf, "hello", 6);
    strncat1(buf, " worldXXXXX", 3);
    printf("%s\n", buf);

    memcpy2(buf, "hello", 6);
    strncat2(buf, " worldXXXXX", 3);
    printf("%s\n", buf);

    /* 'A' constraint test */
    printf("mul64=0x%Lx\n", mul64(0x12345678, 0xabcd1234));
    printf("inc64=0x%Lx\n", inc64(0x12345678ffffffff));

    set = 0xff;
    sigdelset1(&set, 2);
    sigaddset1(&set, 16);
    /* NOTE: we test here if C labels are correctly restored after the
       asm statement */
    goto label1;
 label2:
    __asm__("btsl %1,%0" : "=m"(set) : "Ir"(20) : "cc");
#ifdef __GNUC__ // works strange with GCC 4.3
    set=0x1080fd;
#endif
    printf("set=0x%x\n", set);
    val = 0x01020304;
    printf("swab32(0x%08x) = 0x%0x\n", val, swab32(val));
    return;
 label1:
    goto label2;
}

#else

void asm_test(void)
{
}

#endif

#define COMPAT_TYPE(type1, type2) \
{\
    printf("__builtin_types_compatible_p(%s, %s) = %d\n", #type1, #type2, \
           __builtin_types_compatible_p (type1, type2));\
}

int constant_p_var;

void builtin_test(void)
{
#if GCC_MAJOR >= 3
    COMPAT_TYPE(int, int);
    COMPAT_TYPE(int, unsigned int);
    COMPAT_TYPE(int, char);
    COMPAT_TYPE(int, const int);
    COMPAT_TYPE(int, volatile int);
    COMPAT_TYPE(int *, int *);
    COMPAT_TYPE(int *, void *);
    COMPAT_TYPE(int *, const int *);
    COMPAT_TYPE(char *, unsigned char *);
/* space is needed because tcc preprocessor introduces a space between each token */
    COMPAT_TYPE(char * *, void *); 
#endif
    printf("res = %d\n", __builtin_constant_p(1));
    printf("res = %d\n", __builtin_constant_p(1 + 2));
    printf("res = %d\n", __builtin_constant_p(&constant_p_var));
    printf("res = %d\n", __builtin_constant_p(constant_p_var));
}

#ifndef _WIN32
extern int __attribute__((weak)) weak_f1(void);
extern int __attribute__((weak)) weak_f2(void);
extern int                       weak_f3(void);
extern int __attribute__((weak)) weak_v1;
extern int __attribute__((weak)) weak_v2;
extern int                       weak_v3;

extern int                           (*weak_fpa)() __attribute__((weak));
extern int __attribute__((weak))     (*weak_fpb)();
extern     __attribute__((weak)) int (*weak_fpc)();

extern int                     weak_asm_f1(void) asm("weak_asm_f1x") __attribute((weak));
extern int __attribute((weak)) weak_asm_f2(void) asm("weak_asm_f2x")                    ;
extern int __attribute((weak)) weak_asm_f3(void) asm("weak_asm_f3x") __attribute((weak));
extern int                     weak_asm_v1       asm("weak_asm_v1x") __attribute((weak));
extern int __attribute((weak)) weak_asm_v2       asm("weak_asm_v2x")                    ;
extern int __attribute((weak)) weak_asm_v3(void) asm("weak_asm_v3x") __attribute((weak));

static const size_t dummy = 0;
extern __typeof(dummy) weak_dummy1 __attribute__((weak, alias("dummy")));
extern __typeof(dummy) __attribute__((weak, alias("dummy"))) weak_dummy2;
extern __attribute__((weak, alias("dummy"))) __typeof(dummy) weak_dummy3;

int some_lib_func(void);
int dummy_impl_of_slf(void) { return 444; }
int some_lib_func(void) __attribute__((weak, alias("dummy_impl_of_slf")));

int weak_toolate() __attribute__((weak));
int weak_toolate() { return 0; }

void __attribute__((weak)) weak_test(void)
{
	printf("weak_f1=%d\n", weak_f1 ? weak_f1() : 123);
	printf("weak_f2=%d\n", weak_f2 ? weak_f2() : 123);
	printf("weak_f3=%d\n", weak_f3 ? weak_f3() : 123);
	printf("weak_v1=%d\n",&weak_v1 ? weak_v1   : 123);
	printf("weak_v2=%d\n",&weak_v2 ? weak_v2   : 123);
	printf("weak_v3=%d\n",&weak_v3 ? weak_v3   : 123);

	printf("weak_fpa=%d\n",&weak_fpa ? weak_fpa() : 123);
	printf("weak_fpb=%d\n",&weak_fpb ? weak_fpb() : 123);
	printf("weak_fpc=%d\n",&weak_fpc ? weak_fpc() : 123);
	
	printf("weak_asm_f1=%d\n", weak_asm_f1 != NULL);
	printf("weak_asm_f2=%d\n", weak_asm_f2 != NULL);
	printf("weak_asm_f3=%d\n", weak_asm_f3 != NULL);
	printf("weak_asm_v1=%d\n",&weak_asm_v1 != NULL);
	printf("weak_asm_v2=%d\n",&weak_asm_v2 != NULL);
	printf("weak_asm_v3=%d\n",&weak_asm_v3 != NULL);
}

int __attribute__((weak)) weak_f2() { return 222; }
int __attribute__((weak)) weak_f3() { return 333; }
int __attribute__((weak)) weak_v2 = 222;
int __attribute__((weak)) weak_v3 = 333;
#endif

void const_func(const int a)
{
}

void const_warn_test(void)
{
    const_func(1);
}

struct condstruct {
  int i;
};

int getme (struct condstruct *s, int i)
{
  int i1 = (i == 0 ? 0 : s)->i;
  int i2 = (i == 0 ? s : 0)->i;
  int i3 = (i == 0 ? (void*)0 : s)->i;
  int i4 = (i == 0 ? s : (void*)0)->i;
  return i1 + i2 + i3 + i4;
}

struct global_data
{
  int a[40];
  int *b[40];
};

struct global_data global_data;

int global_data_getstuff (int *, int);

void global_data_callit (int i)
{
  *global_data.b[i] = global_data_getstuff (global_data.b[i], 1);
}

int global_data_getstuff (int *p, int i)
{
  return *p + i;
}

void global_data_test (void)
{
  global_data.a[0] = 42;
  global_data.b[0] = &global_data.a[0];
  global_data_callit (0);
  printf ("%d\n", global_data.a[0]);
}

struct cmpcmpS
{
  unsigned char fill : 3;
  unsigned char b1 : 1;
  unsigned char b2 : 1;
  unsigned char fill2 : 3;
};

int glob1, glob2, glob3;

void compare_comparisons (struct cmpcmpS *s)
{
  if (s->b1 != (glob1 == glob2)
      || (s->b2 != (glob1 == glob3)))
    printf ("comparing comparisons broken\n");
}

void cmp_comparison_test(void)
{
  struct cmpcmpS s;
  s.b1 = 1;
  glob1 = 42; glob2 = 42;
  s.b2 = 0;
  glob3 = 43;
  compare_comparisons (&s);
}

int fcompare (double a, double b, int code)
{
  switch (code) {
    case 0: return a == b;
    case 1: return a != b;
    case 2: return a < b;
    case 3: return a >= b;
    case 4: return a > b;
    case 5: return a <= b;
  }
}

void math_cmp_test(void)
{
  double nan = 0.0/0.0;
  double one = 1.0;
  double two = 2.0;
  int comp = 0;
#define bug(a,b,op,iop,part) printf("Test broken: %s %s %s %s %d\n", #a, #b, #op, #iop, part)

  /* This asserts that "a op b" is _not_ true, but "a iop b" is true.
     And it does this in various ways so that all code generation paths
     are checked (generating inverted tests, or non-inverted tests, or
     producing a 0/1 value without jumps (that's done in the fcompare
     function).  */
#define FCMP(a,b,op,iop,code) \
  if (fcompare (a,b,code))    \
    bug (a,b,op,iop,1); \
  if (a op b) \
    bug (a,b,op,iop,2); \
  if (a iop b) \
    ; \
  else \
    bug (a,b,op,iop,3); \
  if ((a op b) || comp) \
    bug (a,b,op,iop,4); \
  if ((a iop b) || comp) \
    ; \
  else \
    bug (a,b,op,iop,5);

  /* Equality tests.  */
  FCMP(nan, nan, ==, !=, 0);
  FCMP(one, two, ==, !=, 0);
  FCMP(one, one, !=, ==, 1);
  /* Non-equality is a bit special.  */
  if (!fcompare (nan, nan, 1))
    bug (nan, nan, !=, ==, 6);

  /* Relational tests on numbers.  */
  FCMP(two, one, <, >=, 2);
  FCMP(one, two, >=, <, 3);
  FCMP(one, two, >, <=, 4);
  FCMP(two, one, <=, >, 5);

  /* Relational tests on NaNs.  Note that the inverse op here is
     always !=, there's no operator in C that is equivalent to !(a < b),
     when NaNs are involved, same for the other relational ops.  */
  FCMP(nan, nan, <, !=, 2);
  FCMP(nan, nan, >=, !=, 3);
  FCMP(nan, nan, >, !=, 4);
  FCMP(nan, nan, <=, !=, 5);
}

double get100 () { return 100.0; }

void callsave_test(void)
{
#if defined __i386__ || defined __x86_64__
  int i, s; double *d; double t;
  s = sizeof (double);
  printf ("callsavetest: %d\n", s);
  d = alloca (sizeof(double));
  d[0] = 10.0;
  /* x86-64 had a bug were the next call to get100 would evict
     the lvalue &d[0] as VT_LLOCAL, and the reload would be done
     in int type, not pointer type.  When alloca returns a pointer
     with the high 32 bit set (which is likely on x86-64) the access
     generates a segfault.  */
  i = d[0] > get100 ();
  printf ("%d\n", i);
#endif
}


void bfa3(ptrdiff_t str_offset)
{
    printf("bfa3: %s\n", (char *)__builtin_frame_address(3) + str_offset);
}
void bfa2(ptrdiff_t str_offset)
{
    printf("bfa2: %s\n", (char *)__builtin_frame_address(2) + str_offset);
    bfa3(str_offset);
}
void bfa1(ptrdiff_t str_offset)
{
    printf("bfa1: %s\n", (char *)__builtin_frame_address(1) + str_offset);
#if defined(__arm__) && !defined(__GNUC__)
    bfa2(str_offset);
#endif
}

void builtin_frame_address_test(void)
{
    char str[] = "__builtin_frame_address";
    char *fp0 = __builtin_frame_address(0);

    printf("str: %s\n", str);
    bfa1(str-fp0);
}

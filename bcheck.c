/*
 *  Tiny C Memory and bounds checker
 * 
 *  Copyright (c) 2002 Fabrice Bellard
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

//#define BOUND_DEBUG

/* define so that bound array is static (faster, but use memory if
   bound checking not used) */
//#define BOUND_STATIC

#define BOUND_T1_BITS 13
#define BOUND_T2_BITS 11
#define BOUND_T3_BITS (32 - BOUND_T1_BITS - BOUND_T2_BITS)

#define BOUND_T1_SIZE (1 << BOUND_T1_BITS)
#define BOUND_T2_SIZE (1 << BOUND_T2_BITS)
#define BOUND_T3_SIZE (1 << BOUND_T3_BITS)
#define BOUND_E_BITS  4

#define BOUND_T23_BITS (BOUND_T2_BITS + BOUND_T3_BITS)
#define BOUND_T23_SIZE (1 << BOUND_T23_BITS)


/* this pointer is generated when bound check is incorrect */
#define INVALID_POINTER ((void *)(-2))
/* size of an empty region */
#define EMPTY_SIZE        0xffffffff
/* size of an invalid region */
#define INVALID_SIZE      0

typedef struct BoundEntry {
    unsigned long start;
    unsigned long size;
    struct BoundEntry *next;
    unsigned long is_invalid; /* true if pointers outside region are invalid */
} BoundEntry;

/* external interface */
void __bound_init(void);
void __bound_new_region(void *p, unsigned long size);
void __bound_delete_region(void *p);

void *__bound_ptr_add(void *p, int offset) __attribute__((regparm(2)));
char __bound_ptr_indir_b(void *p, int offset) __attribute__((regparm(2)));
unsigned char __bound_ptr_indir_ub(void *p, int offset) __attribute__((regparm(2)));
short __bound_ptr_indir_w(void *p, int offset) __attribute__((regparm(2)));
unsigned short __bound_ptr_indir_uw(void *p, int offset) __attribute__((regparm(2)));
int __bound_ptr_indir_i(void *p, int offset) __attribute((regparm(2)));

void *__bound_calloc(size_t nmemb, size_t size);
void *__bound_malloc(size_t size);
void __bound_free(void *ptr);
void *__bound_realloc(void *ptr, size_t size);

extern char _end;

#ifdef BOUND_STATIC
static BoundEntry *__bound_t1[BOUND_T1_SIZE]; /* page table */
#else
static BoundEntry **__bound_t1; /* page table */
#endif
static BoundEntry *__bound_empty_t2;   /* empty page, for unused pages */
static BoundEntry *__bound_invalid_t2; /* invalid page, for invalid pointers */

static BoundEntry *__bound_find_region(BoundEntry *e1, void *p)
{
    unsigned long addr, tmp;
    BoundEntry *e;

    e = e1;
    while (e != NULL) {
        addr = (unsigned long)p;
        addr -= e->start;
        if (addr <= e->size) {
            /* put region at the head */
            tmp = e1->start;
            e1->start = e->start;
            e->start = tmp;
            tmp = e1->size;
            e1->size = e->size;
            e->size = tmp;
            return e1;
        }
        e = e->next;
    }
    /* no entry found: return empty entry or invalid entry */
    if (e1->is_invalid)
        return __bound_invalid_t2;
    else
        return __bound_empty_t2;
}

static void bound_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "bounds check: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
    va_end(ap);
}

static void bound_alloc_error(void)
{
    bound_error("not enough memory for bound checking code\n");
}

/* do: return (p + offset); */
void *__bound_ptr_add(void *p, int offset)
{
    unsigned long addr = (unsigned long)p;
    BoundEntry *e;
#ifdef BOUND_DEBUG
    printf("add: 0x%x %d\n", (int)p, offset);
#endif

    e = __bound_t1[addr >> (BOUND_T2_BITS + BOUND_T3_BITS)];
    e = (BoundEntry *)((char *)e + 
                       ((addr >> (BOUND_T3_BITS - BOUND_E_BITS)) & 
                        ((BOUND_T2_SIZE - 1) << BOUND_E_BITS)));
    addr -= e->start;
    if (addr > e->size) {
        e = __bound_find_region(e, p);
        addr = (unsigned long)p - e->start;
    }
    addr += offset;
    if (addr > e->size)
        return INVALID_POINTER; /* return an invalid pointer */
    return p + offset;
}

/* do: return *(type *)(p + offset); */
#define BOUND_PTR_INDIR(suffix, type)                                   \
type __bound_ptr_indir_ ## suffix (void *p, int offset)                 \
{                                                                       \
    unsigned long addr = (unsigned long)p;                              \
    BoundEntry *e;                                                      \
                                                                        \
    e = __bound_t1[addr >> (BOUND_T2_BITS + BOUND_T3_BITS)];            \
    e = (BoundEntry *)((char *)e +                                      \
                       ((addr >> (BOUND_T3_BITS - BOUND_E_BITS)) &      \
                        ((BOUND_T2_SIZE - 1) << BOUND_E_BITS)));        \
    addr -= e->start;                                                   \
    if (addr > e->size) {                                               \
        e = __bound_find_region(e, p);                                  \
        addr = (unsigned long)p - e->start;                             \
    }                                                                   \
    addr += offset + sizeof(type);                                      \
    if (addr > e->size)                                                 \
        bound_error("dereferencing invalid pointer");                   \
    return *(type *)(p + offset);                                       \
}

BOUND_PTR_INDIR(b, char)
BOUND_PTR_INDIR(ub, unsigned char)
BOUND_PTR_INDIR(w, short)
BOUND_PTR_INDIR(uw, unsigned short)
BOUND_PTR_INDIR(i, int)

static BoundEntry *__bound_new_page(void)
{
    BoundEntry *page;
    int i;

    page = malloc(sizeof(BoundEntry) * BOUND_T2_SIZE);
    if (!page)
        bound_alloc_error();
    for(i=0;i<BOUND_T2_SIZE;i++) {
        /* put empty entries */
        page[i].start = 0;
        page[i].size = EMPTY_SIZE;
        page[i].next = NULL;
        page[i].is_invalid = 0;
    }
    return page;
}

/* currently we use malloc(). Should use bound_new_page() */
static BoundEntry *bound_new_entry(void)
{
    BoundEntry *e;
    e = malloc(sizeof(BoundEntry));
    return e;
}

static void bound_free_entry(BoundEntry *e)
{
    free(e);
}

static inline BoundEntry *get_page(int index)
{
    BoundEntry *page;
    page = __bound_t1[index];
    if (page == __bound_empty_t2 || page == __bound_invalid_t2) {
        /* create a new page if necessary */
        page = __bound_new_page();
        __bound_t1[index] = page;
    }
    return page;
}

/* mark a region as being invalid (can only be used during init) */
static void mark_invalid(unsigned long addr, unsigned long size)
{
    unsigned long start, end;
    BoundEntry *page;
    int t1_start, t1_end, i, j, t2_start, t2_end;

    start = addr;
    end = addr + size;

    t2_start = (start + BOUND_T3_SIZE - 1) >> BOUND_T3_BITS;
    if (end != 0)
        t2_end = end >> BOUND_T3_BITS;
    else
        t2_end = 1 << (BOUND_T1_BITS + BOUND_T2_BITS);

    printf("mark_invalid: start = %x %x\n", t2_start, t2_end);
    
    /* first we handle full pages */
    t1_start = (t2_start + BOUND_T2_SIZE - 1) >> BOUND_T2_BITS;
    t1_end = t2_end >> BOUND_T2_BITS;

    i = t2_start & (BOUND_T2_SIZE - 1);
    j = t2_end & (BOUND_T2_SIZE - 1);
    
    if (t1_start == t1_end) {
        page = get_page(t2_start >> BOUND_T2_BITS);
        for(; i < j; i++) {
            page[i].size = INVALID_SIZE;
            page[i].is_invalid = 1;
        }
    } else {
        if (i > 0) {
            page = get_page(t2_start >> BOUND_T2_BITS);
            for(; i < BOUND_T2_SIZE; i++) {
                page[i].size = INVALID_SIZE;
                page[i].is_invalid = 1;
            }
        }
        for(i = t1_start; i < t1_end; i++) {
            __bound_t1[i] = __bound_invalid_t2;
        }
        if (j != 0) {
            page = get_page(t1_end);
            for(i = 0; i < j; i++) {
                page[i].size = INVALID_SIZE;
                page[i].is_invalid = 1;
            }
        }
    }
}

void __bound_init(void)
{
    int i;
    BoundEntry *page;
    unsigned long start, size;

#ifndef BOUND_STATIC
    __bound_t1 = malloc(BOUND_T1_SIZE * sizeof(BoundEntry *));
    if (!__bound_t1)
        bound_alloc_error();
#endif
    __bound_empty_t2 = __bound_new_page();
    for(i=0;i<BOUND_T1_SIZE;i++) {
        __bound_t1[i] = __bound_empty_t2;
    }

    page = __bound_new_page();
    for(i=0;i<BOUND_T2_SIZE;i++) {
        /* put invalid entries */
        page[i].start = 0;
        page[i].size = INVALID_SIZE;
        page[i].next = NULL;
        page[i].is_invalid = 1;
    }
    __bound_invalid_t2 = page;

    /* invalid pointer zone */
    start = (unsigned long)INVALID_POINTER & ~(BOUND_T23_SIZE - 1);
    size = BOUND_T23_SIZE;
    mark_invalid(start, size);

    /* malloc zone is also marked invalid */
    start = (unsigned long)&_end;
    size = 128 * 0x100000;
    mark_invalid(start, size);
}

static inline void add_region(BoundEntry *e, 
                              unsigned long start, unsigned long size)
{
    BoundEntry *e1;
    if (e->start == 0) {
        /* no region : add it */
        e->start = start;
        e->size = size;
    } else {
        /* already regions in the list: add it at the head */
        e1 = bound_new_entry();
        e1->start = e->start;
        e1->size = e->size;
        e1->next = e->next;
        e->start = start;
        e->size = size;
        e->next = e1;
    }
}

/* create a new region. It should not already exist in the region list */
void __bound_new_region(void *p, unsigned long size)
{
    unsigned long start, end;
    BoundEntry *page, *e, *e2;
    int t1_start, t1_end, i, t2_start, t2_end;

    start = (unsigned long)p;
    end = start + size;
    t1_start = start >> (BOUND_T2_BITS + BOUND_T3_BITS);
    t1_end = end >> (BOUND_T2_BITS + BOUND_T3_BITS);

    /* start */
    page = get_page(t1_start);
    t2_start = (start >> (BOUND_T3_BITS - BOUND_E_BITS)) & 
        ((BOUND_T2_SIZE - 1) << BOUND_E_BITS);
    t2_end = (end >> (BOUND_T3_BITS - BOUND_E_BITS)) & 
        ((BOUND_T2_SIZE - 1) << BOUND_E_BITS);
#ifdef BOUND_DEBUG
    printf("new %lx %lx %x %x %x %x\n", 
           start, end, t1_start, t1_end, t2_start, t2_end);
#endif

    e = (BoundEntry *)((char *)page + t2_start);
    add_region(e, start, size);

    if (t1_end == t1_start) {
        /* same ending page */
        e2 = (BoundEntry *)((char *)page + t2_end);
        if (e2 > e) {
            e++;
            for(;e<e2;e++) {
                e->start = start;
                e->size = size;
            }
            add_region(e, start, size);
        }
    } else {
        /* mark until end of page */
        e2 = page + BOUND_T2_SIZE;
        e++;
        for(;e<e2;e++) {
            e->start = start;
            e->size = size;
        }
        /* mark intermediate pages, if any */
        for(i=t1_start+1;i<t1_end;i++) {
            page = get_page(i);
            e2 = page + BOUND_T2_SIZE;
            for(e=page;e<e2;e++) {
                e->start = start;
                e->size = size;
            }
        }
        /* last page */
        page = get_page(t2_end);
        e2 = (BoundEntry *)((char *)page + t2_end);
        for(e=page;e<e2;e++) {
            e->start = start;
            e->size = size;
        }
        add_region(e, start, size);
    }
}

/* delete a region */
static inline void delete_region(BoundEntry *e, 
                                 void *p, unsigned long empty_size)
{
    unsigned long addr;
    BoundEntry *e1;

    addr = (unsigned long)p;
    addr -= e->start;
    if (addr <= e->size) {
        /* region found is first one */
        e1 = e->next;
        if (e1 == NULL) {
            /* no more region: mark it empty */
            e->start = 0;
            e->size = empty_size;
        } else {
            /* copy next region in head */
            e->start = e1->start;
            e->size = e1->size;
            e->next = e1->next;
            bound_free_entry(e1);
        }
    } else {
        /* find the matching region */
        for(;;) {
            e1 = e;
            e = e->next;
            /* region not found: do nothing */
            if (e == NULL)
                break;
            addr = (unsigned long)p - e->start;
            if (addr <= e->size) {
                /* found: remove entry */
                e1->next = e->next;
                bound_free_entry(e);
                break;
            }
        }
    }
}

/* WARNING: 'p' must be the starting point of the region. */
void __bound_delete_region(void *p)
{
    unsigned long start, end, addr, size, empty_size;
    BoundEntry *page, *e, *e2;
    int t1_start, t1_end, t2_start, t2_end, i;

    start = (unsigned long)p;
    t1_start = start >> (BOUND_T2_BITS + BOUND_T3_BITS);
    t2_start = (start >> (BOUND_T3_BITS - BOUND_E_BITS)) & 
        ((BOUND_T2_SIZE - 1) << BOUND_E_BITS);
    
    /* find region size */
    page = __bound_t1[t1_start];
    e = (BoundEntry *)((char *)page + t2_start);
    addr = start - e->start;
    if (addr > e->size)
        e = __bound_find_region(e, p);
    /* test if invalid region */
    if (e->size == EMPTY_SIZE || (unsigned long)p != e->start) 
        bound_error("deleting invalid region");
    /* compute the size we put in invalid regions */
    if (e->is_invalid)
        empty_size = INVALID_SIZE;
    else
        empty_size = EMPTY_SIZE;
    size = e->size;
    end = start + size;

    /* now we can free each entry */
    t1_end = end >> (BOUND_T2_BITS + BOUND_T3_BITS);
    t2_end = (end >> (BOUND_T3_BITS - BOUND_E_BITS)) & 
        ((BOUND_T2_SIZE - 1) << BOUND_E_BITS);

    delete_region(e, p, empty_size);
    if (t1_end == t1_start) {
        /* same ending page */
        e2 = (BoundEntry *)((char *)page + t2_end);
        if (e2 > e) {
            e++;
            for(;e<e2;e++) {
                e->start = 0;
                e->size = empty_size;
            }
            delete_region(e, p, empty_size);
        }
    } else {
        /* mark until end of page */
        e2 = page + BOUND_T2_SIZE;
        e++;
        for(;e<e2;e++) {
            e->start = 0;
            e->size = empty_size;
        }
        /* mark intermediate pages, if any */
        /* XXX: should free them */
        for(i=t1_start+1;i<t1_end;i++) {
            page = get_page(i);
            e2 = page + BOUND_T2_SIZE;
            for(e=page;e<e2;e++) {
                e->start = 0;
                e->size = empty_size;
            }
        }
        /* last page */
        page = get_page(t2_end);
        e2 = (BoundEntry *)((char *)page + t2_end);
        for(e=page;e<e2;e++) {
            e->start = 0;
            e->size = empty_size;
        }
        delete_region(e, p, empty_size);
    }
}

/* return the size of the region starting at p, or EMPTY_SIZE if non
   existant region. */
static unsigned long get_region_size(void *p)
{
    unsigned long addr = (unsigned long)p;
    BoundEntry *e;

    e = __bound_t1[addr >> (BOUND_T2_BITS + BOUND_T3_BITS)];
    e = (BoundEntry *)((char *)e + 
                       ((addr >> (BOUND_T3_BITS - BOUND_E_BITS)) & 
                        ((BOUND_T2_SIZE - 1) << BOUND_E_BITS)));
    addr -= e->start;
    if (addr > e->size)
        e = __bound_find_region(e, p);
    if (e->start != (unsigned long)p)
        return EMPTY_SIZE;
    return e->size;
}

/* patched memory functions */

/* XXX: we should use a malloc which ensure that it is unlikely that
   two malloc'ed data have the same address if 'free' are made in
   between. */
void *__bound_malloc(size_t size)
{
    void *ptr;
    /* we allocate one more byte to ensure the regions will be
       separated by at least one byte. With the glibc malloc, it may
       be in fact not necessary */
    ptr = malloc(size + 1);
    if (!ptr)
        return NULL;
    __bound_new_region(ptr, size);
    return ptr;
}

void __bound_free(void *ptr)
{
    if (ptr == NULL)
        return;
    __bound_delete_region(ptr);
    free(ptr);
}

void *__bound_realloc(void *ptr, size_t size)
{
    void *ptr1;
    int old_size;

    if (size == 0) {
        __bound_free(ptr);
        return NULL;
    } else {
        ptr1 = __bound_malloc(size);
        if (ptr == NULL || ptr1 == NULL)
            return ptr1;
        old_size = get_region_size(ptr);
        if (old_size == EMPTY_SIZE)
            bound_error("realloc'ing invalid pointer");
        memcpy(ptr1, ptr, old_size);
        __bound_free(ptr);
        return ptr1;
    }
}

void *__bound_calloc(size_t nmemb, size_t size)
{
    void *ptr;
    size = size * nmemb;
    ptr = __bound_malloc(size);
    if (!ptr)
        return NULL;
    memset(ptr, 0, size);
    return ptr;
}

#if 0
static void bound_dump(void)
{
    BoundEntry *page, *e;
    int i, j;

    printf("region dump:\n");
    for(i=0;i<BOUND_T1_SIZE;i++) {
        page = __bound_t1[i];
        for(j=0;j<BOUND_T2_SIZE;j++) {
            e = page + j;
            /* do not print invalid or empty entries */
            if (e->size != EMPTY_SIZE && e->start != 0) {
                printf("%08x:", 
                       (i << (BOUND_T2_BITS + BOUND_T3_BITS)) + 
                       (j << BOUND_T3_BITS));
                do {
                    printf(" %08lx:%08lx", e->start, e->start + e->size);
                    e = e->next;
                } while (e != NULL);
                printf("\n");
            }
        }
    }
}
#endif
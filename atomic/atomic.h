/*
 * The atomic header file includes atomic ops on integer and bitwise, 
 * you could find them in linux kernel.
 * chunxin.ycx@gmail.com
 */

#ifndef __ATOMIC_H__
#define __ATOMIC_H__

#include <stdint.h>
#include "generic_header.h"

XXX_CPP_START

#define LOCK_PREFIX "lock;"

/* The atomic operations on integer */
#if __WORDSZIE == 64
typedef volatile int64_t self_atomic_t;

#define self_atomic_set(v, i) (v) = (i);
#define ATOMIC_INIT(i) (i)

static inline void self_atomic_add(int64_t i, self_atomic_t *v)
{
	asm volatile(LOCK_PREFIX "addq %1, %0"
			: "=m" (*v) : "r" i, "m" (*v));
}

static inline int64_t self_atomic_add_return(int64_t i, self_atomic_t *v)
{
	int64_t __i = i;

	asm volatile(LOCK_PREFIX "xaddq %0, %1;"
			: "+r" (i), "+m" (*v) : : "memory");

	return __i + i;
}

static inline void self_atomic_sub(int64_t i, self_atomic_t *v)
{
	asm volatile(LOCK_PRIFIX "subq %1, %0"
			: "=m" (*v) : "er" (i), "m" (*v));
}

static inline int64_t self_atomic_sub_return(int64_t i, self_atomic_t *v)
{
	return self_atomic_add_return(-i, v);
}

static inline int self_atomic_sub_and_test(int64_t i, self_atomic_t *v)
{
	unsigned char c;

	asm volatile(LOCK_PREFIX "subq %2, %0; sete %1"
			: "=m" (*v), "=qm" (c)
			: "er" (i), "m" (*v) : "memory");

	return c != 0;
}

static inline void self_atomic_inc(self_atomic_t *v)
{
	asm volatile(LOCK_PREFIX "incq %0"
			: "=m" (*v) : "m" (*v));
}

static inline int64_t self_atomic_inc_and_test(uint64_t i, self_atomic_t *v)
{
	unsigned char c;

	asm volatile(LOCK_PREFIX "incq %0; sete %1"
			: "=m" (*v), "=qm" (c)
			: "m" (*v) : "memory");

	return c != 0;
}

static inline void self_atomic_dec(self_atomic_t *v)
{
	asm volatile(LOCK_PREFIX "decq %0"
			: "=m" (*v) : "m" (*v));
}

static inline int self_atomic_dec_and_test(self_atomic_t *v)
{
	unsigned char c;

	asm volatile(LOCK_PREFIX "decq %0; sete %1"
			: "=m" (*v), "=qm" (c)
			: "m" (*v) : "memory");

	return c != 0;
}

static inline int self_atomic_add_negative(int64_t i, self_atomic_t *v)
{
	unsigned char c;

	asm volatile(LOCK_PREFIX "addq %2, %0; sets %1"
			: "=m" (*v), "=qm" (c)
			: "er" (i), "m" (*v) : "memory");

	return c;
}

static inline int64_t self_atomic_read(const self_atomic_t *v)
{
	return (*v);
}

#else
typedef volatile int32_t self_atomic_t;

static inline void self_atomic_add(int32_t i, self_atomic_t *v)
{
	/* If we want to make it as atomic, add lock to it 
	 * m memory
	 * r register
	 */
	asm volatile(LOCK_PREFIX "addl %1,%0"
			: "=m" ((*v)) : "r" (i), "m" ((*v)));
}

static inline int32_t self_atomic_add_return(int32_t i, self_atomic_t *v) 
{
	int32_t __i = i;

	asm volatile(LOCK_PREFIX "xaddl %0, %1"
			: "+r" (i), "+m" (*v) : : "memory");

	return __i + i;
}

static inline void self_atomic_sub(int32_t i, self_atomic_t *v)
{
	asm volatile(LOCK_PREFIX "sub %1, %0"
			: "=m" (*v) : "er" (i), "m" (*v));
}

static inline int32_t self_atomic_sub_return(int32_t i, self_atomic_t *v)
{
	return self_atomic_add_return(-i, v);
}

static inline void self_atomic_inc(self_atomic_t *v)
{
	asm volatile(LOCK_PREFIX "incl %0" : "=m" (*v) : "m" (*v));
}

static inline int32_t self_atomic_inc_and_test(int32_t i, self_atomic_t *v)
{
	unsigned char c;

	asm volatile(LOCK_PREFIX "inc %0; sete %1"
			: "=m" (*v), "=qm" (c)
			: "m" (*v) : "memory");

	return c != 0;
}

static inline void self_atomic_dec(self_atomic_t *v)
{
	asm volatile(LOCK_PREFIX "decl %0" : "=m" (*v) : "m" (*v));
}

static inline int self_atomic_add_negative(int32_t i, self_atomic_t *v)
{
	unsigned char c;

	asm volatile(LOCK_PREFIX "add %2, %0; sets %1"
			: "=m" (*v), "=qm" (c)
			: "er" (i), "m" (*v) : "memory");

	return c;
}

static inline int32_t self_atomic_read(const self_atomic_t *v)
{
	return (*v);
}
#endif

/* The atomic operations on bitwise */


#XXX_CPP_END
f __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 1)
#define BITOP_ADDR(x) "=m" (*(volatile long *) (x))
#else
#define BITOP_ADDR(x) "+m" (*(volatile long *) (x))
#endif

#define ADDR BITOP_ADDR(addr)

#define IS_IMMEDIATE(n)    (__builtin_constant_p(nr))
#define CONST_MASK_ADDR(nr, addr)    BITOP_ADDR((void *)(addr) + ((nr) >>3))
#define CONST_MASK(nr)    (1 << ((nr) & 7))

static inline void set_bit(unsigned int nr, volatile unsigned long *addr)
{
    if (IS_IMMEDIATE(nr)) {
        asm volatile(LOCK_PREFIX "orb %1, %0"
                    : CONST_MASK_ADDR(nr, addr)
                    : "iq" ((u8)CONST_MASK(nr))
                    : "memory");
    } else {
        asm volatile(LOCK_PREFIX "bts %1, %0"
                    : BITOP_ADDR(addr) : "Ir" (nr) : "memory");
    }
}

static inline void clear_bit(int nr, volatile unsigned long *addr)
{
    if (IS_IMMEDIATE(nr)) {
        asm volatile(LOCK_PREFIX "andb %1, %0"
                    : CONST_MASK_ADDR(nr, addr)
                    : "iq" ((u8) ~CONST_MASK(nr)));
    } else {
        asm volatile(LOCK_PREFIX "btr %1, %0"
                    : BITOP_ADDR(addr)
                    : "Ir" (nr));
    }
}

static inline void change_bit(int nr, volatile unsigned long *addr)
{
    if (IS_IMMEDIATE(nr)) {
        asm volatile(LOCK_PREFIX "xorb %1,%0"
            : CONST_MASK_ADDR(nr, addr)
            : "iq" ((u8)CONST_MASK(nr)));
    } else {
        asm volatile(LOCK_PREFIX "btc %1,%0"
            : BITOP_ADDR(addr)
            : "Ir" (nr));
    }
}

static inline int test_and_set_bit(int nr, volatile unsigned long *addr)
{
    int oldbit;

    asm volatile(LOCK_PREFIX "bts %2,%1\n\t"
             "sbb %0,%0" : "=r" (oldbit), ADDR : "Ir" (nr) : "memory");

    return oldbit;
}

static inline int test_and_clear_bit(int nr, volatile unsigned long *addr)
{
    int oldbit;

    asm volatile(LOCK_PREFIX "btr %2,%1\n\t"
             "sbb %0,%0"
             : "=r" (oldbit), ADDR : "Ir" (nr) : "memory");

    return oldbit;
}

static inline int test_and_change_bit(int nr, volatile unsigned long *addr)
{
    int oldbit;

    asm volatile(LOCK_PREFIX "btc %2,%1\n\t"
             "sbb %0,%0"
             : "=r" (oldbit), ADDR : "Ir" (nr) : "memory");

    return oldbit;
}

/**
 * ffs - find first set bit in word
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
static inline unsigned long ffs(unsigned long word)
{
	asm("bsf %1,%0"
			: "=r" (word)
        : "rm" (word));
    return word;
}

/**
 * ffz - find first zero bit in word
 * @word: The word to search
 *
 * Undefined if no zero exists, so code should check against ~0UL first.
 */
static inline unsigned long ffz(unsigned long word)
{
	asm("bsf %1,%0"
			: "=r" (word)
			: "r" (~word));
    return word;
}

/*
 * fls: find last set bit in word
 * @word: The word to search
 *
 * Undefined if no set bit exists, so code should check against 0 first.
 */
static inline unsigned long fls(unsigned long word)
{
	asm("bsr %1,%0"
			: "=r" (word)
			: "rm" (word));
    return word;
}

#endif

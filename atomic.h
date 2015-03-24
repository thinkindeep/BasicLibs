#ifndef __ATOMIC_H__
#define __ATOMIC_H__

#include <stdint.h>
#include "generic_header.h"

XXX_CPP_START

#define LOCK_PREFIX "lock;"

#if __WORDSZIE == 64
typedef volatile int64_t self_atomic_t;

#define self_atomic_set(v, i) (v) = (i);

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

XXX_CPP_END
#endif

#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include "atomic.h"

void test_atomic_add(int64_t a)
{
	self_atomic_t atomic_var;

	self_atomic_add(a, &atomic_var);

	assert(("atomic add failed", 10 == atomic_var));
}

void test_atomic_add_return(int64_t a)
{
	self_atomic_t atomic_var = 15, atomic_temp;

	atomic_temp = self_atomic_add_return(a, &atomic_var);
	
	assert(("atomic add_return failed", 25 == atomic_temp));
}

void test_atomic_sub(int64_t a)
{
	self_atomic_t atomic_var = 15;

	self_atomic_sub(a, &atomic_var);
	
	assert(("atomic sub failed", 5 == atomic_var));
}

void test_atomic_inc(void)
{
	self_atomic_t atomic_var = 15;

	self_atomic_inc(&atomic_var);
	
	assert(("atomic inc failed", 16 == atomic_var));
}

int main()
{
	int64_t a = 10, b = 5;

	test_atomic_add(a);	
	test_atomic_add_return(a);	
	test_atomic_sub(a);	
	test_atomic_inc();
}

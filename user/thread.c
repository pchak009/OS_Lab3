#include "user/thread.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//Function to create a new thread
int thread_create(void *(start_routine)(void*), void *arg) {
	void *stack;
	stack = (uint*)malloc((uint)PGSIZE);
	
	// Creating a  new child thread using clone
	if (clone(stack) == 0) { 
		start_routine(arg);
		exit(0);
	}
	return 0;
}

//Function to initialize lock
void lock_init(struct lock_t* lock) {
	lock->locked = 0;
}

//Function to acquire lock
void lock_acquire(struct lock_t* lock) {
	while(__sync_lock_test_and_set(&lock->locked,1) != 0);
	__sync_synchronize();
}

//Function to release lock
void lock_release(struct lock_t* lock) {
	__sync_synchronize();
	__sync_lock_release(&lock->locked);
}


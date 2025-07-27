extern "C" {
    #include <panic.h>
    #include <console.h>
    #include <heap.h>
    #include <string.h>
}

#include "cpp_runtime.h"

// Pure virtual function
extern "C" void __cxa_pure_virtual() {
    PANIC("pure virtual function called! THIS SHOULD NOT HAPPEN BUT IT DID!");
}

// Something I heard I needed (GCC.. local static variables?)
namespace __cxxabiv1 
{
	/* guard variables */

	/* The ABI requires a 64-bit type.  */
	__extension__ typedef int __guard __attribute__((mode(__DI__)));

	extern "C" int __cxa_guard_acquire (__guard *);
	extern "C" void __cxa_guard_release (__guard *);
	extern "C" void __cxa_guard_abort (__guard *);

	extern "C" int __cxa_guard_acquire (__guard *g) 
	{
		return !*(char *)(g);
	}

	extern "C" void __cxa_guard_release (__guard *g)
	{
		*(char *)g = 1;
	}

	extern "C" void __cxa_guard_abort (__guard *)
	{

	}
}

// New and delete operators
void *operator new(size_t size)
{
    return heap_calloc(size);
}

void *operator new[](size_t size)
{
    return heap_calloc(size);
}

void operator delete(void *p)
{
    heap_free(p);
}

void operator delete[](void *p)
{
    heap_free(p);
}


extern "C" void cpp_runtime_init() {
    klog("cpp_runtime", "nothing here for now, just a stub.");
}

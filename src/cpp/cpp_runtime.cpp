// HumbleOS file: cpp_runtime.cpp
// Purpose: C++ runtime support code

extern "C" {
    #include <panic.h>
    #include <heap.h>
    #include <string.h>
}

#include <console.hpp>

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

void operator delete(void *p, size_t size)
{
    heap_free(p);
}

void operator delete[](void *p, size_t size)
{
    heap_free(p);
}

// Global destructor support
extern "C" void __cxa_finalize(void *dso_handle) {
    // For kernel, we probably don't need this, but some code expects it
}

extern "C" int __cxa_atexit(void (*func)(void*), void* arg, void* dso_handle) {
    // Register destructor - for kernel, we might just ignore this
    return 0;
}

// Exception handling stubs (since exceptions are disabled)
extern "C" void __cxa_throw(void* thrown_exception, void* tinfo, void (*dest)(void*)) {
    PANIC("C++ exception thrown but exceptions are disabled!");
}

extern "C" void* __cxa_begin_catch(void* exc_obj_in) {
    PANIC("C++ exception handling attempted but exceptions are disabled!");
    return nullptr;
}

extern "C" void __cxa_end_catch() {
    PANIC("C++ exception handling attempted but exceptions are disabled!");
}

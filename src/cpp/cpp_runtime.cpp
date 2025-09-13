/*
    cpp_runtime.cpp - cpp runtime code (bunch of stubs)

    Part of HumbleOS

    Copyright 2025 Thomas Shrader

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the “Software”), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute,
    sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial
    portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

extern "C" {
    #include <panic.h>
    #include <heap.h>
    #include <string.h>
}

#include <console.hpp>

// pure virtual function
extern "C" void __cxa_pure_virtual() {
    PANIC("pure virtual function called! THIS SHOULD NOT HAPPEN BUT IT DID!");
}

// something I heard I needed (GCC.. local static variables?)
namespace __cxxabiv1 
{
	// guard variables

	// the ABI requires a 64-bit type
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

// new and delete operators
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

// global destructor support
extern "C" void __cxa_finalize(void *dso_handle) {
    // for kernel, we probably don't need this, but some code expects it
}

extern "C" int __cxa_atexit(void (*func)(void*), void* arg, void* dso_handle) {
    // register destructor - for kernel, we might just ignore this
    return 0;
}

// exception handling stubs (since exceptions are disabled)
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

// i need this i guess
extern "C" void* __dso_handle = nullptr;

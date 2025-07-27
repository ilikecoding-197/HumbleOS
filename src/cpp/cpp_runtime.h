#ifndef CPP_RUNTIME_H
#define CPP_RUNTIME_H

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void cpp_runtime_init();

#endif
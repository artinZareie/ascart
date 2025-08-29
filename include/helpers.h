#pragma once

#if defined(__clang__)
#define UNROLL_LOOP _Pragma("clang loop unroll(full)")
#elif defined(__GNUC__)
#define UNROLL_LOOP _Pragma("GCC unroll 16")
#else
#define UNROLL_LOOP
#endif

void errorf(const char *fmt, ...);
void elogf(const char *fmt, ...);
void debugf(const char *fmt, ...);
void fatalf(const char *fmt, ...);

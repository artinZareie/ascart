#include <helpers.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void errorf(const char *fmt, ...)
{
#if AGP_ERROR_LEVEL >= 1
    va_list args;
    va_start(args, fmt);
    fputs("[ERROR]: ", stderr);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);
#ifdef AGP_ERROR_FATAL
    exit(EXIT_FAILURE);
#endif
#endif
}

void elogf(const char *fmt, ...)
{
#if AGP_ERROR_LEVEL >= 2
    va_list args;
    va_start(args, fmt);
    fputs("[LOG]: ", stderr);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);
#endif
}

void debugf(const char *fmt, ...)
{
#ifdef AGP_DEBUG
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[DEBUG]: %s:%d: ", __FILE__, __LINE__);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);
#endif
}

void fatalf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fputs("[FATAL]: ", stderr);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);
    exit(EXIT_FAILURE);
}

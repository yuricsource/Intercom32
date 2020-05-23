#include "DebugAssert.h"
#include <cstdio>
#include <cstdarg>

namespace Utilities
{

void DebugAssert::AssertFail(const char *file, int line, const char *message, ...)
{
    printf("\033[0;41m");
    printf("Assert failed: ");
    va_list argptr;
    va_start(argptr, message);
    vfprintf(stdout, message, argptr);
    va_end(argptr);
    printf("\033[0;41m");
    printf(" at %p\n", __builtin_return_address(0));
    printf("\033[0;41m");
    printf("file: \"%s\" line %d\n", file, line);
    printf("\033[0;41m");
    printf("PC: 0x%08x\r\n", (intptr_t)__builtin_return_address(0) - 3);
    abort();
}

void DebugAssert::Assert(int rc, const char *file, int line, const char *function, const char *expression)
{
    printf("\033[0;41m");
    printf("Assert check failed: 0x%x", rc);
    printf(" (%s)", GetGeneralErrorName(rc));
    printf(" at %p\n", __builtin_return_address(0));
    printf("\033[0;41m");
    printf("file: \"%s\" line %d\nfunc: %s\nexpression: %s\n", file, line, function, expression);
    printf("\033[0;41m");
    printf("PC: 0x%08x\r\n", (intptr_t)__builtin_return_address(0) - 3);
    abort();
}

void DebugAssert::AssertWithoutBreaking(int rc, const char *file, int line, const char *function, const char *expression)
{
    
    printf("\033[0;41m");
    printf("Assert check failed: 0x%x", rc);
    printf(" (%s)", GetGeneralErrorName(rc));
    printf("\033[0;41m");
    printf(" at %p\n", __builtin_return_address(0));
    printf("\033[0;41m");
    printf("file: \"%s\" line %d\nfunc: %s\nexpression: %s\n", file, line, function, expression);
    printf("\033[0;41m");
    printf("PC: 0x%08x\r\n", (intptr_t)__builtin_return_address(0) - 3);
}

void DebugAssert::abort()
{

    __asm__("break 0,0");
    while (1)
        ;
}

const char *DebugAssert::GetGeneralErrorName(int code)
{
    for (int i = 0; i < sizeof(ErrorMessageTable) / sizeof(ErrorMessageTable[0]); ++i)
    {
        if (ErrorMessageTable[i].code == code)
        {
            return ErrorMessageTable[i].msg;
        }
    }

    return "Unkown";
}

const char *DebugAssert::GetLwipErrorName(int code)
{
    for (int i = 0; i < sizeof(ErrorMessageTableLwip) / sizeof(ErrorMessageTableLwip[0]); ++i)
    {
        if (ErrorMessageTableLwip[i].code == code)
        {
            return ErrorMessageTableLwip[i].msg;
        }
    }

    return "Unkown";
}

} // namespace Utilities
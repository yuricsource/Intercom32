#include "DebugAssert.h"
#include <cstdio>
#include <cstdarg>

namespace Utilities
{

void DebugAssert::Assert(int rc, const char *file, int line, const char *function, const char *expression)
{
    printf("Assert check failed: 0x%x", rc);
    printf(" (%s)", GetGeneralErrorName(rc));
    printf(" at %p\n", __builtin_return_address(0));
    printf("file: \"%s\" line %d\nfunc: %s\nexpression: %s\n", file, line, function, expression);
    printf("PC: 0x%08x\r\n", (intptr_t)__builtin_return_address(0) - 3);
    abort();
}

void DebugAssert::AssertWithoutBreaking(int rc, const char *file, int line, const char *function, const char *expression)
{
    printf("Assert check failed: 0x%x", rc);
    printf(" (%s)", GetGeneralErrorName(rc));
    printf(" at %p\n", __builtin_return_address(0));
    printf("file: \"%s\" line %d\nfunc: %s\nexpression: %s\n", file, line, function, expression);
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
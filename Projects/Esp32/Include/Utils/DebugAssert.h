#pragma once

#include "ErrorList.h"
#include <cstdint>
#include <cstdarg>

#ifndef __ASSERT_FUNC
#ifdef __ASSERT_FUNCTION
#define __ASSERT_FUNC __ASSERT_FUNCTION /* used in glibc assert.h */
#else
#define __ASSERT_FUNC "??"
#endif
#endif

#ifndef NDEBUG
#define DEBUG
#endif

#ifdef DEBUG
/**
 * @brief 							breaks in an assert
 */
#define DebugAssertFail(...)		Utilities::DebugAssert::AssertFail(__FILE__, __LINE__, __VA_ARGS__);

/**
 * @brief Test the result, if different prints the error and breaks in an assert
 * @param function					The function the check the result
 * @param result					The result expected
 */
#define DebugAssert(function, result)                                     \
	do                                                                    \
	{                                                                     \
		int __err_rc = (int)function;                                     \
		if (__err_rc != result)                                           \
		{                                                                 \
			Utilities::DebugAssert::Assert(__err_rc, __FILE__, __LINE__, \
											__ASSERT_FUNC, #function);    \
		}                                                                 \
	} while (0)

/**
 * @brief Test the result, if different prints the error
 * @param function					The function the check the result
 * @param result					The result expected
 */
#define DebugAssertWithoutAbort(function, result) ({                                 \
	esp_err_t __err_rc = (int)function;                                              \
	if (__err_rc != result)                                                          \
	{                                                                                \
		Utilities::DebugAssert::AssertWithoutBreaking(__err_rc, __FILE__, __LINE__, \
													   __ASSERT_FUNC, #function);    \
	}                                                                                \
	__err_rc;                                                                        \
})
#else
#define DebugAssert(function) function
#define DebugAssertWithoutAbort(x) function
#endif

namespace Utilities
{
class DebugAssert
{
public:
	void Assert();

public:
	static void AssertFail(const char *file, int line, const char *message, ...);
	static void Assert(int rc, const char *file, int line, const char *function, const char *expression);
	static void AssertWithoutBreaking(int rc, const char *file, int line, const char *function, const char *expression);
	static const char *GetGeneralErrorName(int code);
	static const char *GetLwipErrorName(int code);

private:
	static void abort();
};

} // namespace Utilities
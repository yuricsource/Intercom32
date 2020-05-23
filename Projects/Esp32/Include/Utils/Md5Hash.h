#pragma once

#include <stdint.h>
#include <string.h>
#include <stdio.h>

namespace Utilities
{

class Md5Hash
{
public:
	Md5Hash();
	~Md5Hash();
	void Calculate();
	bool GetBytesResult(uint8_t *result, size_t len);
	const char* ToString();
	void Add(const void *data, size_t size);
	void Reset();
	static constexpr uint8_t Md5Length = 16;

private:
	struct Md5Context
	{
		uint32_t lo;
		uint32_t hi;
		uint32_t a;
		uint32_t b;
		uint32_t c;
		uint32_t d;
		uint8_t buffer[64];
		uint32_t block[16];
	};

	uint8_t _hashResult[16] = {};
	char _hashResultString[33] = {};

	Md5Context _context = {};
	const void *update(void *ctxBuf, const void *data, size_t size);
};
} // namespace Applications
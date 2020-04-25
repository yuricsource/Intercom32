#pragma once

#include <cstdint>

namespace Utilities 
{

class Base64
{
public:
	static void Encode(uint8_t *input, uint16_t inputLength, uint8_t *base, uint16_t baseLength);
	static uint16_t GetBufferLength(uint16_t length);
	
private:
	static const char *_bytes64Pattern;
};

} // namespace Utilities
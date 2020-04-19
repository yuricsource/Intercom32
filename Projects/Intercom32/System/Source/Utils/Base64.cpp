#include "Base64.h"
#include <cassert>

namespace Utilities
{

const char *Base64::_pattern = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void Base64::Encode(uint8_t *input, uint16_t inputLength, uint8_t *base, uint16_t baseLength)
{
	uint16_t length = Base64::GetBufferLength(inputLength + 1);
	assert(baseLength >= length);

	base[length] = '\0';
	int j = 0, i;
	for (i = 0; i < inputLength; i += 3)
	{
		int a = input[i];
		int b = (i + 1 < inputLength) ? input[i + 1] : 0;
		int c = (i + 2 < inputLength) ? input[i + 2] : 0;
		base[j++] = _pattern[a >> 2];
		base[j++] = _pattern[((a & 3) << 4) | (b >> 4)];
		base[j++] = (i + 1 < inputLength) ? _pattern[((b & 15) << 2) | (c >> 6)] : 61;
		base[j++] = (i + 2 < inputLength) ? _pattern[(c & 63)] : 61;
	}
}

uint16_t GetBufferLength(uint16_t length)
{
	return (length + 2) / 3 * 4;
}

} // namespace Utilities
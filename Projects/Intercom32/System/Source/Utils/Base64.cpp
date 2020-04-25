#include "Base64.h"
#include <cassert>

namespace Utilities
{

const char *Base64::_bytes64Pattern = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void Base64::Encode(uint8_t* inputData, uint16_t inputLen, uint8_t* base64Data, uint16_t base64DataLen)
{
	uint16_t blen = Base64::GetBufferLength(inputLen + 1);
	assert(base64DataLen >= blen);
	
	base64Data[blen] = '\0';
	int j = 0, i;
	for (i = 0; i < inputLen; i += 3)
	{
		int a = inputData[i];
		int b = (i + 1 < inputLen) ? inputData[i + 1] : 0;
		int c = (i + 2 < inputLen) ? inputData[i + 2] : 0;
		base64Data[j++] = _bytes64Pattern[a >> 2];
		base64Data[j++] = _bytes64Pattern[((a & 3) << 4) | (b >> 4)];
		base64Data[j++] = (i + 1 < inputLen) ? _bytes64Pattern[((b & 15) << 2) | (c >> 6)] : 61;
		base64Data[j++] = (i + 2 < inputLen) ? _bytes64Pattern[(c & 63)] : 61;
	}
}

uint16_t Base64::GetBufferLength(uint16_t length)
{
	return (length + 2) / 3 * 4;
}

} // namespace Utilities
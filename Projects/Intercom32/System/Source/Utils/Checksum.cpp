#include "Checksum.h"

namespace Utilities
{

uint8_t Checksum::GetChecksum(uint8_t *data, uint16_t len)
{
	uint8_t result = 0;

	for (uint16_t i = 0; i < len; ++i)
		result += data[i];

	result = ~result;
	result++;
	return result;
}

} // namespace Utilities
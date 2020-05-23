#pragma once

#include <cstdint>

namespace Utilities
{
/// @brief	A class for calculating an 8bit checksum for an array of bytes.
class Checksum
{
public:
	/// @brief	Gets the byte checksum.
	/// @param [in,out]	data	The data.
	/// @param 		   	len 	The length the data.
	/// @return	The checksum.
	static uint8_t GetChecksum(uint8_t *data, uint16_t len);
};
} // namespace Utilities
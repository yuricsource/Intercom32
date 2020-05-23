#pragma once

#include <cstdint>

namespace Utilities
{
/// @brief	CRC32  class.
class Crc32xZlib
{
private:
	/// @brief	The static CRC table[]
	const static uint32_t CrcTable[];

public:
	/// zlib's CRC32 polynomial
	static constexpr uint32_t Polynomial = 0xEDB88320;

	/// @brief	Gets the CRC32 value.
	/// @param [in,out]	data	If non-null, the data.
	/// @param 		   	len 	The length.
	/// @return	The CRC.
	static uint32_t GetCrc(uint8_t *data, uint32_t len, uint32_t previousCrc32 = 0);
};

} // namespace Utilities
#pragma once

#include <cstdint>

namespace Utilities
{
/// @brief CRC16 class.
class Crc16x1021
{
private:
	/// @brief	The static CRC table[]
	const static uint16_t CrcTable[];

public:
	/// @brief	Gets the CRC16 value.
	/// @param [in,out]	data	The data.
	/// @param 		   	len 	The length.
	/// @return	The CRC result.
	static uint16_t GetCrc(uint8_t *data, uint16_t len);
};

} // namespace Utilities
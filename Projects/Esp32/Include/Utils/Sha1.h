#pragma once

#include <cstdint>

namespace Utilities
{

class Sha1
{
private:
	// fields
	uint32_t _H0, _H1, _H2, _H3, _H4;
	unsigned char _bytes[64];
	int _unprocessedBytes;
	uint32_t _size;
	void process();

public:
	Sha1();
	~Sha1();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Adds bytes to the Sha1 digest.
	///
	/// @param		data		Must be non-null, the data to add.
	/// @param 		num 		The length of the data in the array.
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddBytes(const unsigned char *data, int num);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Gets the Sha1 digest. It is 20 bytes long, and th ememory is expected to be provided by below parameter.
	///
	/// @param		digest		It is an byte array allocated in th ecaller. Must not be null, and must have 20 bytes length.
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void GetDigest(unsigned char *digest);

	///////////////////////////////////////////////////////////////////////
	// Utility methods

	/// @brief Circular left bit rotation.  MSB wraps around to LSB
	static uint32_t Lrot(uint32_t x, int bits);

	/// @brief Save a 32-bit unsigned integer to memory, in big-endian order
	static void StoreBigEndianUint32(unsigned char *byte, uint32_t num);

	/// @brief	Print out memory in hexadecimal
	static void HexPrinter(unsigned char *c, int l);
};

} // namespace Utilities
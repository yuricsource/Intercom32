#include "Sha1.h"
#include <cassert>
#include <cstring>
#include <cstdio>

namespace Utilities
{

void Sha1::HexPrinter(unsigned char *c, int l)
{
	assert(c);
	assert(l > 0);
	while (l > 0)
	{
		printf(" %02x", *c);
		l--;
		c++;
	}
}

uint32_t Sha1::Lrot(uint32_t x, int bits)
{
	return (x << bits) | (x >> (32 - bits));
};

void Sha1::StoreBigEndianUint32(unsigned char *byte, uint32_t num)
{
	assert(byte);
	byte[0] = (unsigned char)(num >> 24);
	byte[1] = (unsigned char)(num >> 16);
	byte[2] = (unsigned char)(num >> 8);
	byte[3] = (unsigned char)num;
}

Sha1::Sha1()
{
	// Make sure that the data type is the right size
	assert(sizeof(uint32_t) * 5 == 20);

	// initialize
	_H0 = 0x67452301;
	_H1 = 0xefcdab89;
	_H2 = 0x98badcfe;
	_H3 = 0x10325476;
	_H4 = 0xc3d2e1f0;
	_unprocessedBytes = 0;
	_size = 0;
}

Sha1::~Sha1()
{
	// erase data
	_H0 = _H1 = _H2 = _H3 = _H4 = 0;
	for (int c = 0; c < 64; c++)
		_bytes[c] = 0;
	_unprocessedBytes = _size = 0;
}

void Sha1::process()
{
	assert(_unprocessedBytes == 64);
	//printf("process: "); hexPrinter(bytes, 64); printf("\n");
	int t;
	uint32_t a, b, c, d, e, K, f, W[80];
	// starting values
	a = _H0;
	b = _H1;
	c = _H2;
	d = _H3;
	e = _H4;
	// copy and expand the message block
	for (t = 0; t < 16; t++)
		W[t] = (_bytes[t * 4] << 24) + (_bytes[t * 4 + 1] << 16) + (_bytes[t * 4 + 2] << 8) + _bytes[t * 4 + 3];
	for (; t < 80; t++)
		W[t] = Lrot(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);

	/* main loop */
	uint32_t temp;
	for (t = 0; t < 80; t++)
	{
		if (t < 20)
		{
			K = 0x5a827999;
			f = (b & c) | ((b ^ 0xFFFFFFFF) & d); //TODO: try using ~
		}
		else if (t < 40)
		{
			K = 0x6ed9eba1;
			f = b ^ c ^ d;
		}
		else if (t < 60)
		{
			K = 0x8f1bbcdc;
			f = (b & c) | (b & d) | (c & d);
		}
		else
		{
			K = 0xca62c1d6;
			f = b ^ c ^ d;
		}
		temp = Lrot(a, 5) + f + e + W[t] + K;
		e = d;
		d = c;
		c = Lrot(b, 30);
		b = a;
		a = temp;
		//printf("t=%d %08x %08x %08x %08x %08x\n", t, a, b, c, d, e);
	}
	/* add variables */
	_H0 += a;
	_H1 += b;
	_H2 += c;
	_H3 += d;
	_H4 += e;
	//printf("Current: %08x %08x %08x %08x %08x\n", _H0, _H1, _H2, _H3, _H4);
	/* all bytes have been processed */
	_unprocessedBytes = 0;
}

void Sha1::AddBytes(const unsigned char *data, int num)
{
	assert(data);
	assert(num > 0);
	// add these bytes to the running total
	_size += num;
	// repeat until all data is processed
	while (num > 0)
	{
		// number of bytes required to complete block
		int needed = 64 - _unprocessedBytes;
		assert(needed > 0);
		// number of bytes to copy (use smaller of two)
		int toCopy = (num < needed) ? num : needed;
		// Copy the bytes
		memcpy(_bytes + _unprocessedBytes, data, toCopy);
		// Bytes have been copied
		num -= toCopy;
		data += toCopy;
		_unprocessedBytes += toCopy;

		// there is a full block
		if (_unprocessedBytes == 64)
			process();
	}
}

void Sha1::GetDigest(unsigned char *digest)
{
	// save the message size
	uint32_t totalBitsL = _size << 3;
	uint32_t totalBitsH = _size >> 29;

	// add 0x80 to the message
	AddBytes((unsigned char *)"\x80", 1);

	unsigned char footer[64] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	// block has no room for 8-byte filesize, so finish it
	if (_unprocessedBytes > 56)
		AddBytes((unsigned char *)footer, 64 - _unprocessedBytes);
	assert(_unprocessedBytes <= 56);
	// how many zeros do we need
	int neededZeros = 56 - _unprocessedBytes;
	// store file size (in bits) in big-endian format
	StoreBigEndianUint32(footer + neededZeros, totalBitsH);
	StoreBigEndianUint32(footer + neededZeros + 4, totalBitsL);
	// finish the final block
	AddBytes((unsigned char *)footer, neededZeros + 8);

	// copy the digest bytes
	StoreBigEndianUint32(digest, _H0);
	StoreBigEndianUint32(digest + 4, _H1);
	StoreBigEndianUint32(digest + 8, _H2);
	StoreBigEndianUint32(digest + 12, _H3);
	StoreBigEndianUint32(digest + 16, _H4);
}

} // namespace Utilities
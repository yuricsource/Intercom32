#include "CircularBuffer.h"
#include <string>

namespace Utilities
{

template <typename T>
CircularBuffer<T>::CircularBuffer(unsigned int size)
	: _size(size), _buffer(new T[size]), _write(_buffer), _read(_buffer), _used(0)
{
}

template <typename T>
CircularBuffer<T>::~CircularBuffer()
{
	delete[] _buffer;
}

template <typename T>
bool CircularBuffer<T>::Push(T elem)
{
	if (Free() == 0)
		return false;
	*_write = elem;
	++_write;
	Align(_write);
	_used += 1;
	return true;
}

template <typename T>
bool CircularBuffer<T>::Pop(T &elem)
{
	if (Used() == 0)
		return false;
	elem = *_read;
	++_read;
	Align(_read);
	_used -= 1;
	return true;
}

template <typename T>
bool CircularBuffer<T>::Front(T &elem)
{
	if (Free() == 0)
		return false;
	elem = *_write;
	return true;
}

template <typename T>
bool CircularBuffer<T>::Back(T &elem)
{
	if (Used() == 0)
		return false;
	elem = *_read;
	return true;
}

template <typename T>
unsigned int CircularBuffer<T>::Write(const T *data, unsigned int len)
{
	unsigned int totalLen = 0;
	while (len > 0 && Free() != 0)
	{
		unsigned int partLen = WritePart(data, len);
		data += partLen;
		len -= partLen;
		_used += partLen;
		totalLen += partLen;
	}
	return totalLen;
}

template <typename T>
unsigned int CircularBuffer<T>::Read(T *data, unsigned int len)
{
	unsigned int totalLen = 0;
	while (len > 0 && Used() != 0)
	{
		unsigned int partLen = ReadPart(data, len);
		data += partLen;
		len -= partLen;
		_used -= partLen;
		totalLen += partLen;
	}
	return totalLen;
}

template <typename T>
T CircularBuffer<T>::operator[](int index)
{
	volatile T *volatile p;
	if (index < 0)
	{
		p = _write + index;
		while (p < _buffer)
			p += _size;
	}
	else
	{
		p = _read + index;
	}
	Align(p);
	return *p;
}

template <typename T>
unsigned int CircularBuffer<T>::Skip(unsigned int len)
{
	len = std::min(len, Used());
	_read += len;
	Align(_read);
	_used -= len;
	return len;
}

template <typename T>
unsigned int CircularBuffer<T>::WritePart(const T *data, unsigned int len)
{
	unsigned int maxLen = std::min(static_cast<unsigned int>((_buffer + _size) - _write), std::min(len, Free()));
	std::memcpy(const_cast<T *>(_write), data, maxLen * sizeof(T));
	_write += maxLen;
	Align(_write);
	return maxLen;
}

template <typename T>
unsigned int CircularBuffer<T>::ReadPart(T *data, unsigned int len)
{
	unsigned int maxLen = std::min(static_cast<unsigned int>((_buffer + _size) - _read), std::min(len, Used()));
	std::memcpy(data, const_cast<T *>(_read), maxLen * sizeof(T));
	_read += maxLen;
	Align(_read);
	return maxLen;
}

template class CircularBuffer<char>;
template class CircularBuffer<uint8_t>;
template class CircularBuffer<uint16_t>;
template class CircularBuffer<char *>;
template class CircularBuffer<uint64_t>;

} // namespace Utilities
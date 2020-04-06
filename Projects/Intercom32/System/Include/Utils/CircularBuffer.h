#pragma once

#include <algorithm>
#include <cstring>
#include <atomic>

namespace Utilities
{
template <typename T>
class CircularBuffer
{
public:
	/// @brief	Circular Buffer Constructor.
	/// @param	size	The buffer size.
	CircularBuffer(unsigned int size);

	/// @brief	Circular Buffer Destructor.
	~CircularBuffer();

	/// @brief	How much it is used in the circular buffer.
	/// @return	An integer.
	inline unsigned int Used() { return _used; }

	/// @brief	How much of the circulat buffer is free (not used)
	/// @return	An integer.
	inline unsigned int Free() { return _size - Used(); }

	/// @brief	Circular Buffer size.
	/// @return	An integer.
	inline unsigned int Size() { return _size; }

	/// @brief	Push an elem in the circular buffer at the next location.
	///         Returns:
	///			True  - if the elem was added;
	/// 		False -the elem was not added.
	/// @param	elem	The data to be pushed.
	/// @return	True if it succeeds, false if it fails.
	bool Push(T elem);

	/// @brief	Remove the first element from the read location.
	///			Returns:
	/// 		True  - the element was removed
	/// 		False - the element could be removed from the buffer.
	/// @param [in,out]	elem	The element to be popped.
	/// @return	True if it succeeds, false if it fails.
	bool Pop(T &elem);

	/// @brief	Get an element at the front of the buffer.
	/// @param	elem	The element.
	/// @return	True if it succeeds, false if it fails.
	bool Front(T &elem);

	/// @brief	Get the first element in the buffer.
	/// @param [in,out]	elem	The element.
	/// @return	True if it succeeds, false if it fails.
	bool Back(T &elem);

	/// @brief	Write data into the circular buffer.
	/// @param 	data	The data to be written.
	/// @param 	len 	The length.
	/// @return	An integer.
	unsigned int Write(const T *data, unsigned int len);

	/// @brief	Read the Circular Buffer.
	/// @param data	The data.
	/// @param len 	The length.
	/// @return	An int.
	unsigned int Read(T *data, unsigned int len);

	/// @brief	Get the element by index.
	/// @param	index	Index of the element.
	/// @return	The indexed value.
	T operator[](int index);

	/// @brief	Skip the len items, max Used()
	/// @param	len	The length.
	/// @return	An integer.
	unsigned int Skip(unsigned int len);

protected:
	unsigned int _size;
	T *_buffer;
	volatile T *volatile _write;
	volatile T *volatile _read;
	std::atomic<int32_t> _used;

	/// @brief	Writes a part of the circular buffer but not more then Free().
	/// @param	data	The data.
	/// @param	len 	The length.
	/// @return	An integer.
	unsigned int WritePart(const T *data, unsigned int len);

	/// @brief	Reads a part of the circular buffer but not more than Used().
	/// @param data	If non-null, the data.
	/// @param len 	The length.
	/// @return	The part.
	unsigned int ReadPart(T *data, unsigned int len);

	/// @brief	Aligns the given pointer.
	/// @param [in,out]	ptr	[in,out] If non-null, the pointer.
	inline void Align(volatile T *volatile &ptr)
	{
		while (ptr >= (_buffer + _size))
			ptr -= _size;
	}
};

} // namespace Utilities
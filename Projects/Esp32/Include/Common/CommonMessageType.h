#pragma once

#include <cstdint>

namespace Common
{

enum class MessageIdIndex : uint16_t
{
	Unknown = 0,
	// Configuration Index: 1 - 1000
	FirstConfig = 1,

	// Status Index: 1001 - 2000
	FirstStatus = 1001,

	// Event Index: 2001 - 4000
	FirstEvent = 2001,

	// Command Index: 4001 - 6000
	FirstCommand = 4001,

	// Response Index: 10001
	FirstCommandResponse = 10001,
};

} // namespace Common
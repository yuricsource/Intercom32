#pragma once

#include <cstdint>
#include <cstring>
#include <array>
#include <bitset>
#include "HalCommon.h"

namespace Common
{
    
///	@brief Transport Layer Type
enum class TransportLayerType : uint8_t
{
    None = 0,
    Wifi = 1,
    Lora = 2,
    Ethernet = 3,
    Gprs = 4
};

///	@brief Protocol Type
enum class ProtocolType : uint8_t
{
	Websocket = 0,
    Http,
    Mqtt
};

using std::array;

static constexpr uint8_t IpLength = 64;
using IpAddress = array<char, IpLength>;
static_assert(sizeof(IpAddress) == 64, "Array has invalid size.");

using IpAddress = array<char, IpLength>;
static_assert(sizeof(IpAddress) == 64, "Array has invalid size.");

using IpAddress = array<char, IpLength>;
static_assert(sizeof(IpAddress) == 64, "Array has invalid size.");

}
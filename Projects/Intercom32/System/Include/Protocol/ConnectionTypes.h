#pragma once

#include <cstring>
#include <cstdint>
#include "FastDelegate.h"
#include "lwip/ip_addr.h"
#include "CommonTypes.h"

namespace Protocol
{

using namespace fastdelegate;
using Common::IpAddress;

enum class ConnectionState
{
	ConnectionNotConfigured = 0,
	Connecting = 1,
	Connected = 2,
	Disconnected = 3
};

enum class ConnectionChangeReason
{
	None,
	Error,
	Timeout,
	Aborted,
	Reset,
	Closed
};

using DelegateDataReceived = FastDelegate2<const char *, uint16_t>;
using DelegateConnectionStateChanged = FastDelegate2<ConnectionState, ConnectionChangeReason>;

struct RemoteConnection
{
	uint16_t Port;
	IpAddress Address;
};

} // namespace Protocol
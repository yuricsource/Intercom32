#pragma once

#include <cstdint>
#include <cstring>
#include "FastDelegate.h"
#include "lwip/ip_addr.h"
#include "CommonTypes.h"
#include "ConnectionTypes.h"
#include "ConfigurationCommon.h"

namespace Protocol
{

using namespace fastdelegate;
using namespace Common;


class BaseConnection
{
public:
	enum class ConnectStatus
	{
		Failed = 0,
		Connecting = 1,
		SuccessfullyConnected = 2,
	};

	BaseConnection()
	{
		_connectionContext.State = ConnectionState::ConnectionNotConfigured;
	}
	
	ConnectStatus Connect(const RemoteConnection &remoteConnection)
	{
		return DoConnect(remoteConnection);
	}

	bool Send(const unsigned char *data, uint16_t length)
	{
		return DoSend(data, length);
	}

	void Close()
	{
		DoClose();
	}

	void Reset()
	{
		DoReset();
	}

	bool IsConnectionReady()
	{
		return IsConnected();
	}

	void SetDataReceived(DelegateDataReceived delegate)
	{
		_connectionContext.OnDataReceived = delegate;
	}

	void SetConnectionStateChanged(DelegateConnectionStateChanged delegate)
	{
		_connectionContext.OnStateChanged = delegate;
	}

	inline void IncrementTimeout()
	{
		_connectionContext.Timeout++;
	}

	inline constexpr u8_t GetTimeout() const
	{
		return _connectionContext.Timeout;
	}

	inline void ResetTimeout()
	{
		_connectionContext.Timeout = 0;
	}

	inline constexpr ConnectionState GetConnectionState() const
	{
		return _connectionContext.State;
	}

	inline void SetConnectionState(ConnectionState state)
	{
		_connectionContext.State = state;
	}

	inline void ConnectionChanged(ConnectionChangeReason reason)
	{
		if (_connectionContext.OnStateChanged)
			_connectionContext.OnStateChanged(_connectionContext.State, reason);
	}

	void DataReceived(const char *data, uint16_t len)
	{
		if (!_connectionContext.OnDataReceived || len == 0)
			return;
		_connectionContext.OnDataReceived(data, len);
	}

	constexpr TransportLayerType GetConnection() const
	{
		return _connectionType;
	}

	enum class ConnectionError : uint8_t
	{
		None,
		MemoryAllocation,
		UnkownHost,
	};

	constexpr ConnectionError GetError()
	{
		return _lastError;
	}

private:

	virtual ConnectStatus DoConnect(const RemoteConnection &remoteConnection)
	{
		return ConnectStatus::Failed;
	}

	virtual bool DoSend(const unsigned char *data, uint16_t length) = 0;

	virtual void DoClose() = 0;

	virtual void DoReset() = 0;

	virtual bool IsConnected() = 0;

protected:
	~BaseConnection()
	{
	}

	struct ConnectionContext
	{
		uint16_t Timeout;
		ConnectionState State;
		DelegateConnectionStateChanged OnStateChanged;
		DelegateDataReceived OnDataReceived;
	};

	ConnectionContext _connectionContext = {};

	TransportLayerType _connectionType = TransportLayerType::None;

	ConnectionError _lastError = ConnectionError::None;

private:
	/// @brief	Hide Copy constructor.
	BaseConnection(const BaseConnection &) = delete;

	/// @brief	Hide Assignment operator.
	BaseConnection &operator=(const BaseConnection &) = delete;

	/// @brief	Hide Move constructor.
	BaseConnection(BaseConnection &&) = delete;

	/// @brief	Hide Move assignment operator.
	BaseConnection &operator=(BaseConnection &&) = delete;
};

} // namespace Protocol

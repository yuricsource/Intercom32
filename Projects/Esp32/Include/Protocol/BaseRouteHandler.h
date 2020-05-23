#pragma once

#include "TimeLimit.h"
#include "BaseConnection.h"
#include "CommonConnection.h"

namespace Protocol
{

using Hal::TimeLimit;

class BaseRouteHandler
{
public:

	enum class ConnectionExecutionResult
	{
		Success,
		Working,
		Error
	};

public:
	BaseRouteHandler();

	~BaseRouteHandler()
	{
	}

	const BaseConnection &GetConnection()
	{
		return *_connection;
	}

	ConnectionExecutionResult GetResult()
	{
		return _executionResult;
	}

	bool IsTerminated() const
	{
		return isTerminated();
	}

	bool Start()
	{
		return start();
	}

	void Terminate()
	{
		terminate();
	}

	void Process()
	{
		process();
	}

	void SetConnection(BaseConnection* connection)
	{
		if (connection == nullptr)
			_connection = nullptr;
		else
			_connection = connection;
	}
	

protected:
	BaseConnection *_connection = nullptr;
	ConnectionExecutionResult _executionResult;
	RemoteConnection _remoteConnection;

	virtual void ReceivedData(const uint8_t *data, uint16_t length) = 0;

	virtual void ConnectionStateChanged(ConnectionState state, ConnectionChangeReason reason) = 0;

	virtual bool SendFrame(const uint8_t *data, uint16_t length) = 0;

	const RemoteConnection &GetRemoteConnection()
	{
		return _remoteConnection;
	}
	
	static constexpr uint16_t MaxHayStackLength = HayStackMaxSize;
	
	char _hayStack[MaxHayStackLength] = { };
	
	uint16_t _hayStackWorkingLength = 0;

private:
	virtual void setConnection() = 0;

	virtual bool isTerminated() const = 0;

	virtual bool start() = 0;

	virtual void terminate() = 0;

	virtual void process() = 0;

private:
	/// @brief	Hide Copy constructor.
	BaseRouteHandler(const BaseRouteHandler &) = delete;

	/// @brief	Hide Assignment operator.
	BaseRouteHandler &operator=(const BaseRouteHandler &) = delete;

	/// @brief	Hide Move constructor.
	BaseRouteHandler(BaseRouteHandler &&) = delete;

	/// @brief	Hide Move assignment operator.
	BaseRouteHandler &operator=(BaseRouteHandler &&) = delete;
};

} // namespace Protocol
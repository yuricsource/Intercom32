#pragma once

#include "BaseConnection.h"
#include "TimeLimit.h"
#include "lwip/tcp.h"
#include "DnsClient.h"
#include "Logger.h"

namespace Protocol
{

using Hal::Hardware;
using Hal::TimeLimit;
// using BaseConnection::ConnectStatus;
using Utilities::Logger;

class TcpConnection : public BaseConnection
{
public:
	TcpConnection() : BaseConnection(),
					  _port(0), _isConnected(false), _pcb(nullptr)
	{
	}

	~TcpConnection()
	{
		Close();
	}

	uint16_t GetPort();

private:
	uint16_t _port;
	bool _isConnected;
	tcp_pcb *_pcb;

	static err_t receiveHandler(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
	static void clearPcbHandler(TcpConnection *conn, tcp_pcb *pcb);
	static err_t pollHandler(void *arg, struct tcp_pcb *pcb);
	static void errorHandler(void *arg, err_t err);
	static err_t sentHandler(void *arg, struct tcp_pcb *pcb, u16_t len);
	static err_t connectedHandler(void *arg, struct tcp_pcb *pcb, err_t err);

	BaseConnection::ConnectStatus DoConnect(const RemoteConnection &remoteConnection) override;

	bool DoSend(const unsigned char *data, uint16_t length) override;

	void DoClose() override;

	void DoReset() override;

	bool IsConnected() override;

private:
	/// @brief	Hide Copy constructor.
	TcpConnection(const TcpConnection &) = delete;

	/// @brief	Hide Assignment operator.
	TcpConnection &operator=(const TcpConnection &) = delete;

	/// @brief	Hide Move constructor.
	TcpConnection(TcpConnection &&) = delete;

	/// @brief	Hide Move assignment operator.
	TcpConnection &operator=(TcpConnection &&) = delete;
};

} // namespace Protocol
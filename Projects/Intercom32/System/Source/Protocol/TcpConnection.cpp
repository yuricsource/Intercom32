#include "lwip/tcpip.h"
#include <cstring>
#include "Hardware.h"
#include "TcpConnection.h"
#include "Logger.h"
#include "IPParser.h"
#include "DebugAssert.h"

#define DEBUG_TCP

namespace Protocol
{

using Utilities::DebugAssert;

BaseConnection::ConnectStatus TcpConnection::DoConnect(const RemoteConnection &remoteConnection)
{
    _port = remoteConnection.Port;

    ip_addr_t ipAddress = ip_addr_any;

    DnsClient *dnsClient = DnsClient::Instance();
    if (dnsClient->Resolve(remoteConnection.Address.data(), ipAddress) != DnsClient::DnsResolveResult::Done)
        return BaseConnection::ConnectStatus::Failed;

    Logger::LogInfo(Logger::LogSource::Wifi, "DNS resolved %s to %s", remoteConnection.Address.data(), ipaddr_ntoa(&ipAddress));

    if (_pcb != nullptr)
        Logger::LogInfo(Logger::LogSource::Wifi, "PCB exists already %p", (void *)_pcb);
    else
    {
        _pcb = tcp_new();
        if (_pcb == nullptr)
            return BaseConnection::ConnectStatus::Failed;

        Logger::LogInfo(Logger::LogSource::Wifi, "New PCB Allocated %p", (void *)_pcb);
    }

    while (tcp_bind(_pcb, IP_ADDR_ANY, _port) != ERR_OK)
    {
        vTaskDelay(20);
    }

    tcp_arg(_pcb, this);
    tcp_recv(_pcb, receiveHandler);
    tcp_err(_pcb, errorHandler);
    tcp_poll(_pcb, pollHandler, 10);
    tcp_sent(_pcb, sentHandler);
    tcp_connect(_pcb, &ipAddress, remoteConnection.Port, connectedHandler);

    ConnectionChanged(ConnectionChangeReason::None);
    SetConnectionState(ConnectionState::Connected);
    _isConnected = true;
    return BaseConnection::ConnectStatus::SuccessfullyConnected;
}

err_t TcpConnection::connectedHandler(void *arg, struct tcp_pcb *pcb, err_t err)
{
    TcpConnection *tcpConnection = reinterpret_cast<TcpConnection *>(arg);
    if (tcpConnection == nullptr)
        return ERR_ARG;

    if (err != ERR_OK)
    {
        clearPcbHandler(tcpConnection, pcb);

        tcpConnection->SetConnectionState(ConnectionState::Disconnected);
        tcpConnection->ConnectionChanged(ConnectionChangeReason::Error);

        return ERR_OK;
    }
    printf("connectedHandler");
    tcpConnection->SetConnectionState(ConnectionState::Connected);
    tcpConnection->ConnectionChanged(ConnectionChangeReason::None);

    return ERR_OK;
}

err_t TcpConnection::sentHandler(void *arg, struct tcp_pcb *pcb, u16_t len)
{
    if (pcb == nullptr || arg == nullptr)
        return ERR_ABRT;

    TcpConnection *tcpConnection = reinterpret_cast<TcpConnection *>(arg);
    printf("sentHandler\n");
    tcpConnection->ResetTimeout();
    return (ERR_OK);
}

void TcpConnection::errorHandler(void *arg, err_t err)
{
    TcpConnection *tcpConnection = reinterpret_cast<TcpConnection *>(arg);
    if (tcpConnection == nullptr)
        return;

    Logger::LogInfo(Logger::LogSource::Wifi, "An Error happened: %s", DebugAssert::GetLwipErrorName(err));

    tcpConnection->Reset();
    tcpConnection->SetConnectionState(ConnectionState::Disconnected);

    ConnectionChangeReason reason = ConnectionChangeReason::Error;
    switch (err)
    {
    case ERR_ABRT:
        reason = ConnectionChangeReason::Aborted;
        break;
    case ERR_RST:
        reason = ConnectionChangeReason::Reset;
        break;
    case ERR_CLSD:
        reason = ConnectionChangeReason::Closed;
        break;
    }
    tcpConnection->ConnectionChanged(reason);
}

err_t TcpConnection::pollHandler(void *arg, struct tcp_pcb *pcb)
{
    TcpConnection *tcpConnection = reinterpret_cast<TcpConnection *>(arg);
    if (tcpConnection == nullptr)
        return ERR_ARG;
    printf("pollHandler\n");
    tcpConnection->IncrementTimeout();
    if (tcpConnection->GetTimeout() > 20)
    {

        Logger::LogInfo(Logger::LogSource::Wifi, "Released binding for local port %i. (ABORT)", pcb->local_port);

        tcpConnection->_pcb = nullptr;
        tcp_abort(pcb);

        if (tcpConnection->GetConnectionState() != ConnectionState::Disconnected)
        {
            tcpConnection->SetConnectionState(ConnectionState::Disconnected);
            tcpConnection->ConnectionChanged(ConnectionChangeReason::Timeout);
        }
        return (ERR_ABRT);
    }

    return (ERR_OK);
}

err_t TcpConnection::receiveHandler(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    TcpConnection *tcpConnection = reinterpret_cast<TcpConnection *>(arg);
    if (tcpConnection == nullptr)
        return ERR_ARG;

    if (err == ERR_OK)
    {
        if (p != NULL)
        {
            pbuf *temp_p = p;
            u16_t totalBytes = p->tot_len;
            while (temp_p != NULL)
            {
                tcpConnection->DataReceived((const uint8_t *)temp_p->payload, temp_p->len);
                temp_p = temp_p->next;
            }
            pbuf_free(p);
            tcp_recved(pcb, totalBytes);
        }
        else if (p == NULL) // Server closed connection
        {
            // Clear the PCB
            clearPcbHandler(tcpConnection, pcb);
            if (tcpConnection->GetConnectionState() != ConnectionState::Disconnected)
            {
                tcpConnection->SetConnectionState(ConnectionState::Disconnected);
                tcpConnection->ConnectionChanged(ConnectionChangeReason::Closed);
            }
            Logger::LogInfo(Logger::LogSource::Wifi, "Connection closed by remote host.");
        }
    }

    return ERR_OK;
}

void TcpConnection::clearPcbHandler(TcpConnection *tcpConnection, tcp_pcb *pcb)
{

    Logger::LogInfo(Logger::LogSource::Wifi, "PCB Clear Requested %p %p", (void *)pcb, (void *)tcpConnection->_pcb);

    if (tcpConnection == nullptr)
    {
        assert(false);
        return;
    }

    assert(pcb == tcpConnection->_pcb);
    if (pcb != tcpConnection->_pcb)
        return;

    uint16_t localPort = pcb->local_port;

    tcpConnection->_pcb = nullptr;
    tcp_arg(tcpConnection->_pcb, NULL);
    tcp_sent(tcpConnection->_pcb, NULL);
    tcp_recv(tcpConnection->_pcb, NULL);
    tcp_err(tcpConnection->_pcb, NULL);
    tcp_poll(tcpConnection->_pcb, NULL, 0);
    tcp_close(tcpConnection->_pcb);

    Logger::LogInfo(Logger::LogSource::Wifi, "Deleting pcb, port %i.", localPort);
    // _connectionContext.State = ConnectionState::Disconnected;
    
    tcpConnection->SetConnectionState(ConnectionState::Disconnected);
}

bool TcpConnection::DoSend(const unsigned char *data, uint16_t length)
{
    if (_pcb == nullptr || data == nullptr || length == 0) 
		return false;
	
	if (_connectionContext.State != ConnectionState::Connected)
		return false;
	
	// Send data
	{
		err_t error = tcp_write(_pcb, data, length, TCP_WRITE_FLAG_COPY);
		if (error == ERR_OK)
		{
            printf("Socket sent.\n");
			error = tcp_output(_pcb);
            // tcp_sndbuf(_pcb);
			if (error == ERR_OK)
				return true;
			else
			{
#ifdef DEBUG_TCP
				Logger::LogError(Logger::LogSource::Tcp, "DoSend|tcp_output|Error#%i|\n", error);
#endif // DEBUG_TCP
			}
		}
		else
		{
#ifdef DEBUG_TCP
			Logger::LogError(Logger::LogSource::Tcp, "DoSend|tcp_write|Error#%i|\n", error);
#endif // DEBUG_TCP	
		}
	}
	return false;
}

void TcpConnection::DoClose()
{
    if (_pcb == nullptr)
        return;

    Logger::LogInfo(Logger::LogSource::Wifi, "Closing Connection port %i.\n", _port);

    clearPcbHandler(this, _pcb);
    _pcb = nullptr;

    return;
}

void TcpConnection::DoReset()
{
    // To-Do
    return;
}

bool TcpConnection::IsConnected()
{
    return _isConnected;
}

uint16_t TcpConnection::GetPort()
{
    return _port;
}
} // namespace Protocol
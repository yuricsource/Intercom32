#pragma once

#include "lwip/inet.h"
#include "lwip/err.h"
#include "lwip/dns.h"
#include "lwip/opt.h"
#include "lwipopts.h"
#include "IPParser.h"
#include "Hardware.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "Hardware.h"
#include "semaphore.hpp"
#include "TimeLimit.h"

namespace Protocol
{

using cpp_freertos::BinarySemaphore;
using Hal::TimeLimit;
using Protocol::IPParser;

extern "C" void dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg);

class DnsClient
{

public:
	DnsClient()
	{
		dns_init();
	}

	enum class DnsResolverNonBlockingStage
	{
		Start = 0,
		Waiting,
		Done,
	};

	static inline DnsClient *Instance()
	{
		if (_dnsClient == nullptr)
		{
			_dnsClient = new DnsClient();
		}
		return _dnsClient;
	}

	enum class DnsResolveResult : uint8_t
	{
		UnknownError,
		InvalidDnsServer,
		ErrorToResolve,
		InvalidArguments,
		Processing,
		Done,
	};

	class DnsParameter
	{
	public:
		DnsParameter() : IpAddress(nullptr),
						 DnsSemaphore(nullptr){};
		ip_addr_t *IpAddress;
		BinarySemaphore *DnsSemaphore;
	};

	void SetDnsServer(const ip_addr_t &dnsServer);

	DnsResolveResult Resolve(const char *hostname, ip_addr_t &ipaddr);

private:
	static DnsClient *_dnsClient;
	BinarySemaphore _dnsSemaphore;

	friend void dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg);

private:
	/// @brief	Hide Copy constructor.
	DnsClient(const DnsClient &) = delete;

	/// @brief	Hide Assignment operator.
	DnsClient &operator=(const DnsClient &) = delete;

	/// @brief	Hide Move constructor.
	DnsClient(DnsClient &&) = delete;

	/// @brief	Hide Move assignment operator.
	DnsClient &operator=(DnsClient &&) = delete;
};

} // namespace Protocol
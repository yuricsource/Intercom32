#include "DnsClient.h"

namespace Protocol
{

DnsClient *DnsClient::_dnsClient = nullptr;

void DnsClient::SetDnsServer(const ip_addr_t &dnsServer)
{
	dns_setserver(DNS_MAX_SERVERS - 1, &dnsServer);
}

void dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
	DnsClient::DnsParameter *dnsParam = static_cast<DnsClient::DnsParameter *>(callback_arg);
	if (dnsParam == nullptr || dnsParam->DnsSemaphore == nullptr || dnsParam->IpAddress == nullptr)
		return;

	if (ipaddr != nullptr)
		dnsParam->IpAddress->u_addr.ip4.addr = ip4_addr_get_u32(&ipaddr->u_addr.ip4);
	else
		dnsParam->IpAddress->u_addr.ip4.addr = 0;

	dnsParam->DnsSemaphore->Give();
}

DnsClient::DnsResolveResult DnsClient::Resolve(const char *hostname, ip_addr_t &ipaddr)
{
	if (IPParser::Parse(hostname, ipaddr))
		return DnsResolveResult::Done;

	bool isDnsValid = false;
	for (int i = 0; i < DNS_MAX_SERVERS; i++)
	{
		const ip_addr_t *dnsaddr = dns_getserver(i);
		if (IPParser::IsValid(*dnsaddr))
		{
			isDnsValid = true;
			break;
		}
	}
	if (isDnsValid == false)
	{
		return DnsResolveResult::InvalidDnsServer;
	}

	ipaddr = ip_addr_any;
	DnsParameter dnsParam;

	dnsParam.DnsSemaphore = &_dnsSemaphore;
	dnsParam.IpAddress = &ipaddr;

	ip_addr_t cachedaddr = {};
	err_t err = dns_gethostbyname(hostname, &cachedaddr, &dns_callback, &dnsParam);
	switch (err)
	{
	case ERR_OK:
		ip_addr_copy(ipaddr, cachedaddr);
		return DnsResolveResult::Done;
	case ERR_INPROGRESS:
		while (ipaddr.u_addr.ip4.addr == 0)
		{
			if (!_dnsSemaphore.Take(10 * 100))
			{
				return DnsResolveResult::ErrorToResolve;
			}
		}
		if (ipaddr.u_addr.ip4.addr != 0)
		{
			return DnsResolveResult::Done;
		}
		break;
	case ERR_ARG:
		return DnsResolveResult::InvalidArguments;
	default:
		return DnsResolveResult::UnknownError;
	}
	return DnsResolveResult::UnknownError;
}

} // namespace Protocol
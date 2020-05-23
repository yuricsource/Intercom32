#include "IPParser.h"
#include "lwip/ip_addr.h"

namespace Protocol
{

bool IPParser::Parse(const char *str, ip4_addr_t &ipAddress)
{
	ip4_addr_t addr = {};
	if (ip4addr_aton(str, &addr) == 1)
	{
		ipAddress.addr = addr.addr;
		return true;
	}
	return false;
}

bool IPParser::ToString(const ip4_addr_t &ipAddress, char *str, uint8_t len)
{
	str = ip4addr_ntoa_r(&ipAddress, str, len);
	if (str == nullptr)
	{
		return false;
	}
	return true;
}

bool IPParser::IsValid(const char* str)
{
	ip_addr_t addr = {};
	return ipaddr_aton(str, &addr) == 1;
}

bool IPParser::IsValid(const ip_addr_t& ipaddr)
{
	return !ip_addr_isany(&ipaddr);
}

bool IPParser::Parse(const char *str, ip_addr_t &ipAddress)
{
	if (ipaddr_aton(str, &ipAddress) == 1)
		return true;
	return false;
}

bool IPParser::ToString(const ip_addr_t &ipAddress, char *str, uint8_t len)
{
	ipaddr_ntoa_r(&ipAddress, str, len);
	if (str == nullptr)
	{
		return false;
	}
	return true;
}
} // namespace Protocol
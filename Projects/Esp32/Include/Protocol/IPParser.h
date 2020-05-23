#pragma once

#include <cstdint>
#include "lwip/ip_addr.h"

namespace Protocol
{

class IPParser
{
public:
	static bool Parse(const char *str, ip4_addr_t &ipAddress);
	static bool Parse(const char *str, ip_addr_t &ipAddress);

	static bool IsValid(const char *str);
	static bool IsValid(const ip_addr_t &ipaddr);

	static bool ToString(const ip4_addr_t &ipAddress, char *str, uint8_t len);
	static bool ToString(const ip_addr_t &ipAddress, char *str, uint8_t len);
};

} // namespace Protocol
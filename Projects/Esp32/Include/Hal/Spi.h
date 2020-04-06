
#ifndef INCLUDE_HAL_SPI_H_
#define INCLUDE_HAL_SPI_H_

#include "HalCommon.h"

namespace Hal
{

class Spi
{
public:
	Spi();
	~Spi();

	void begin();
	uint8_t transfer(uint8_t data);
	void transfer(uint8_t* data, uint32_t size);
	void setBitOrder(uint8_t bitOrder);
	void setDataMode(uint8_t dataMode);

private:
};

} // namespace Hal

#endif /* INCLUDE_HAL_SPI_H_ */

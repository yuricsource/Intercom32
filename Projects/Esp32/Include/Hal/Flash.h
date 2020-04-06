
#ifndef INCLUDE_HAL_FLASH_H_
#define INCLUDE_HAL_FLASH_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "driver/gpio.h"
#include "esp_spi_flash.h"

namespace Hal
{

class Flash
{
public:
	~Flash();
	Flash();

	bool Read(uint32_t offset, uint8_t *data, size_t size);
	bool Write(uint32_t offset, uint8_t *data, size_t size);
	bool EraseSector(uint32_t sector);
	static constexpr uint32_t FlashSectorSize = SPI_FLASH_SEC_SIZE;

private:
};

} // namespace Hal

#endif /* INCLUDE_HAL_FLASH_H_ */

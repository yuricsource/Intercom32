
#include <Flash.h>
#include "esp_flash_partitions.h"
#include "esp_image_format.h"
#include "esp_partition.h"
#include "Hardware.h"
#include "esp_ota_ops.h"

namespace Hal
{

Flash::Flash()
{
}

Flash::~Flash()
{
}

bool Flash::Read(uint32_t offset, uint8_t *data, size_t size)
{
    if (data == nullptr || size == 0)
        return false;

    return spi_flash_read(offset, data, size) == ESP_OK;
}

bool Flash::Write(uint32_t offset, uint8_t *data, size_t size)
{
    if (data == nullptr || size == 0)
        return false;

    return spi_flash_write(offset, data, size) == ESP_OK;
}

bool Flash::EraseSector(uint32_t sector)
{
    return spi_flash_erase_sector(sector) == ESP_OK;
}

} // namespace Hal

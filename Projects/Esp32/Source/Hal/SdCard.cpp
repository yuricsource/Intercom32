
#include "SdCard.h"
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"

namespace Hal
{

SdCard::SdCard(Gpio *gpio) : _gpio(gpio)
{
}

bool SdCard::Mount()
{
	_gpio->SetMode(Gpio::GpioIndex::Gpio15, Gpio::Mode::Input);
	_gpio->SetPull(Gpio::GpioIndex::Gpio15, SdCardInputConfiguration);
	_gpio->SetMode(Gpio::GpioIndex::Gpio2, Gpio::Mode::Input);
	_gpio->SetPull(Gpio::GpioIndex::Gpio2, SdCardInputConfiguration);
	_gpio->SetMode(Gpio::GpioIndex::Gpio4, Gpio::Mode::Input);
	_gpio->SetPull(Gpio::GpioIndex::Gpio4,  Gpio::Pull::Down);
	_gpio->SetMode(Gpio::GpioIndex::Gpio12, Gpio::Mode::Input);
	_gpio->SetPull(Gpio::GpioIndex::Gpio12, SdCardInputConfiguration);
	_gpio->SetMode(Gpio::GpioIndex::Gpio13, Gpio::Mode::Input);
	_gpio->SetPull(Gpio::GpioIndex::Gpio13, SdCardInputConfiguration);

	if (isMounted)
		return true;
	sdmmc_host_t host = {};

	host.flags = SDMMC_HOST_FLAG_8BIT | SDMMC_HOST_FLAG_4BIT |
				 SDMMC_HOST_FLAG_1BIT | SDMMC_HOST_FLAG_DDR;
	host.slot = SDMMC_HOST_SLOT_1;
	host.max_freq_khz = SDMMC_FREQ_DEFAULT;
	host.io_voltage = 3.3f;
	host.init = &sdmmc_host_init;
	host.set_bus_width = &sdmmc_host_set_bus_width;
	host.get_bus_width = &sdmmc_host_get_slot_width;
	host.set_bus_ddr_mode = &sdmmc_host_set_bus_ddr_mode;
	host.set_card_clk = &sdmmc_host_set_card_clk;
	host.do_transaction = &sdmmc_host_do_transaction;
	host.deinit = &sdmmc_host_deinit;
	host.io_int_enable = sdmmc_host_io_int_enable;
	host.io_int_wait = sdmmc_host_io_int_wait;
	host.command_timeout_ms = 0;

	sdmmc_slot_config_t slotConfig = {};

	slotConfig.gpio_cd = PinNotConfigured;
	slotConfig.gpio_wp = PinNotConfigured;
	slotConfig.width = 0;

	esp_vfs_fat_sdmmc_mount_config_t mountConfig = {};
	mountConfig.format_if_mount_failed = false;
	mountConfig.max_files = 5;
	mountConfig.allocation_unit_size = 16 * 1024;

	sdmmc_card_t *card;
	esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slotConfig, &mountConfig, &card);

	if (ret != ESP_OK)
	{
		if (ret == ESP_FAIL)
		{
			printf("Failed to mount filesystem. "
				   "If you want the card to be formatted, set format_if_mount_failed = true.");
		}
		else
		{
			printf("Failed to initialize the card (%s). "
				   "Make sure SD card lines have pull-up resistors in place.",
				   esp_err_to_name(ret));
		}
		return false;
	}

	isMounted = true;
	return true;
}

void SdCard::Unmount()
{
	esp_vfs_fat_sdmmc_unmount();
	_gpio->SetMode(Gpio::GpioIndex::Gpio4, Gpio::Mode::Input);
	_gpio->SetPull(Gpio::GpioIndex::Gpio4,  Gpio::Pull::Down);
	isMounted = false;
}

bool SdCard::IsMounted()
{
	return isMounted;
}

SdCard::~SdCard()
{
}
} // namespace Hal

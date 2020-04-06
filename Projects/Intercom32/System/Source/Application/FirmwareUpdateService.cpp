#include "FirmwareUpdateService.h"
#include "ConfigurationAgent.h"
#include "Hardware.h"
#include "BankConfiguration.h"
#include "Md5Hash.h"
#include "HalCommon.h"
#include "Flash.h"

using Hal::BankConfiguration;
using Hal::Flash;
using Hal::Hardware;

namespace Applications
{

FirmwareUpdateService::FirmwareUpdateService() : cpp_freertos::Thread("FWUSVC", configFWUPDATESVC_STACK_DEPTH, 3)
{
}

FirmwareUpdateService::~FirmwareUpdateService()
{
}

bool FirmwareUpdateService::eraseOtherBank()
{
	Hardware *hardware = Hardware::Instance();
	uint32_t startAddress = hardware->GetBunkConfiguration().GetOtherBank().Address;
	uint32_t bankSize = hardware->GetBunkConfiguration().GetOtherBank().ImageSize;
	for (uint32_t eraseAddress = startAddress; eraseAddress < startAddress + bankSize; eraseAddress += Flash::FlashSectorSize)
	{
		Logger::LogInfo(Logger::LogSource::FirmwareUpdate, "Erasing address: %x, Sector: %d", 
						eraseAddress, eraseAddress / Flash::FlashSectorSize);
		if (hardware->GetFlash().EraseSector(eraseAddress / Flash::FlashSectorSize) == false)
			return false;
	}
	return true;
}

void FirmwareUpdateService::Run()
{

	if (eraseOtherBank())
		Logger::LogInfo(Logger::LogSource::FirmwareUpdate, "Erased other bank successfully");
	else
		Logger::LogInfo(Logger::LogSource::FirmwareUpdate, "Erase command failed");

	Hardware *hardware = Hardware::Instance();

	hardware->GetSdCard().Mount();

	Md5Hash md5;
	FILE *f = fopen("/sdcard/update.bin", "rb");
	uint8_t buffer[Flash::FlashSectorSize];
	if (f == nullptr)
	{
		printf("File update.bin doesn't exist\n");
	}
	else
	{
		printf("File update.bin found.\n");

		uint32_t len = 0;
		Flash &flash = hardware->GetFlash();
		uint32_t address = hardware->GetBunkConfiguration().GetOtherBank().Address;
		do
		{
			len = fread(buffer, 1, Flash::FlashSectorSize, f);
			if (flash.Write(address, buffer, len) == false)
			{
				Logger::LogInfo(Logger::LogSource::FirmwareUpdate, "Failed to write into flash.");
			}
			md5.Add(buffer, len);
			Logger::LogInfo(Logger::LogSource::FirmwareUpdate, "Writing address: %x, Size: %d", address, len);
			address += len;
		} while (len > 0);

		md5.Calculate();

		printf("MD5: %s\n", md5.ToString());
		
		hardware->GetBunkConfiguration().SetRunningBank(Hal::Bank::Bank2);
	}

	while (true)
	{
		vTaskDelay(1000);
	}
}

} // namespace Applications

// ESP_IMAGE_HEADER_MAGIC
// esp_partition_iterator_t pInterator = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, nullptr);

// if (pInterator == nullptr)
// 	Logger::LogInfo(Logger::LogSource::FirmwareUpdate, "Error, Interator failed to fetch the partition information.");
// else
// {
// 	const esp_partition_t *partition = esp_partition_get(pInterator);
// 	Logger::LogInfo(Logger::LogSource::FirmwareUpdate, "Partiton Type: %d, Sub Type: %d, Adress: %x, size:%x",
// 					(uint8_t)partition->type, (uint8_t)partition->subtype, partition->address, partition->size);
// 	Logger::LogInfo(Logger::LogSource::FirmwareUpdate, "Partition Label: %s.", partition->label);
// }

/*
	Hardware *hardware = Hardware::Instance();

	hardware->GetSdCard().Mount();

	Md5Hash md5;
	FILE *f = fopen("/sdcard/update.bin", "rb");
	char *buffer[512];
	if (f == nullptr)
	{
		printf("File update.bin doesn't exist\n");
	}
	else
	{
		printf("File update.bin found.\n");

		int len = 0;
		do
		{
			len = fread(&buffer, 1, 512, f);
			md5.Add(buffer, len);
		} while (len > 0);

		printf("Full file read.\n");

		//		md5.calculate();

		char md5Result[50] = {};
		md5.Calculate();

		md5.GetCharsResult(md5Result, 50);
		printf("MD5: %s\n", md5Result);

		printf("\n");
		printf("Calculation finished.\n");
	}
*/
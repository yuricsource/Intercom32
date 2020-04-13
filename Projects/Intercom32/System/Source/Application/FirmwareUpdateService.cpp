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

	while (true)
	{
		vTaskDelay(1000);
	}
}

} // namespace Applications

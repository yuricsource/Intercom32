
#ifndef INCLUDE_HAL_BANK_CONFIGURATION_H_
#define INCLUDE_HAL_BANK_CONFIGURATION_H_

#include "HalCommon.h"

namespace Hal
{

class BankConfiguration
{
public:
	BankConfiguration();
	~BankConfiguration();

	struct BankDescription
	{
		VersionName Version;	 /*!< Application version */
		VersionName ProjectName; /*!< Project name */
		VersionName IdfVer;		 /*!< Version IDF */
		TimeString Time;		 /*!< Compile time */
		DateString Date;		 /*!< Compile date*/
		Sha256Array Sha256;		 /*!< sha256 of elf file */
		uint32_t ImageSize;		 /*!< Image Size*/
		uint32_t PartitionSize;	 /*!< Partition Size*/
		Bank BankRunning;		 /*!< Bank Running*/
		uint32_t Address;		 /*!< Start address*/
	};

	const BankConfiguration::BankDescription GetCurrentBank();
	const BankConfiguration::BankDescription GetOtherBank();

	bool SetRunningBank(Bank nextBank);

private:
	BankDescription currentBank = { };
	BankDescription otherBank = { };
	uint32_t getImageSize(Bank bank);
};
} // namespace Hal

#endif /* INCLUDE_HAL_BANK_CONFIGURATION_H_ */

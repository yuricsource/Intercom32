
#include "BankConfiguration.h"
#include "Hardware.h"
#include "esp_partition.h"
#include "esp_ota_ops.h"
#include <cstring>

namespace Hal
{

BankConfiguration::BankConfiguration()
{
	esp_partition_iterator_t pInterator = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, nullptr);

	if (pInterator == nullptr)
		printf("Error, Interator failed to fetch the partition information.\n");
	else
	{
		const esp_partition_t *partition1 = esp_ota_get_running_partition();

		currentBank.Address = partition1->address;
		currentBank.BankRunning = ((uint8_t)partition1->subtype == 0) ? Bank::Bank1 : Bank::Bank2;
		currentBank.PartitionSize = partition1->size;
		currentBank.ImageSize = getImageSize(currentBank.BankRunning);
		esp_app_desc_t description;
		esp_ota_get_partition_description(partition1, &description);

		memcpy(currentBank.Version.data(), description.version, currentBank.Version.size());
		memcpy(currentBank.Time.data(), description.time, currentBank.Time.size());
		memcpy(currentBank.Date.data(), description.date, currentBank.Date.size());
		memcpy(currentBank.IdfVer.data(), description.idf_ver, currentBank.IdfVer.size());
		memcpy(currentBank.ProjectName.data(), description.project_name, currentBank.ProjectName.size());
		memcpy(currentBank.Sha256.data(), description.app_elf_sha256, currentBank.Sha256.size());

		if (currentBank.BankRunning == Bank::Bank1)
			pInterator = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, nullptr);
		else
			pInterator = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_FACTORY, nullptr);

		const esp_partition_t *partition2 = esp_partition_get(pInterator);

		otherBank.Address = partition2->address;
		otherBank.BankRunning = ((uint8_t)partition2->subtype == 0) ? Bank::Bank1 : Bank::Bank2;
		otherBank.PartitionSize = partition2->size;
		otherBank.ImageSize = getImageSize(otherBank.BankRunning);

		esp_ota_get_partition_description(partition2, &description);

		memcpy(otherBank.Version.data(), description.version, otherBank.Version.size());
		memcpy(otherBank.Time.data(), description.time, otherBank.Time.size());
		memcpy(otherBank.Date.data(), description.date, otherBank.Date.size());
		memcpy(otherBank.IdfVer.data(), description.idf_ver, otherBank.IdfVer.size());
		memcpy(otherBank.ProjectName.data(), description.project_name, otherBank.ProjectName.size());
		memcpy(otherBank.Sha256.data(), description.app_elf_sha256, otherBank.Sha256.size());
	}
}

const BankConfiguration::BankDescription BankConfiguration::GetCurrentBank()
{
	return currentBank;
}

const BankConfiguration::BankDescription BankConfiguration::GetOtherBank()
{
	return otherBank;
}

bool BankConfiguration::SetRunningBank(Bank nextBank)
{
	esp_partition_iterator_t pInterator;
	if (nextBank == Bank::Bank1)
		pInterator = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_FACTORY, nullptr);
	else
		pInterator = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, nullptr);

	if (pInterator == nullptr)
		printf("Error, Interator failed to fetch the partition information.");
	else
	{
		const esp_partition_t *partition = esp_partition_get(pInterator);
		printf("Partiton Type: %d, Sub Type: %d, Adress: %x, size:%x",
			   (uint8_t)partition->type, (uint8_t)partition->subtype, partition->address, partition->size);
		printf("Partition Label: %s.", partition->label);
	}

	const esp_partition_t *partition = esp_partition_get(pInterator);

	if (esp_ota_set_boot_partition(partition) != ESP_OK)
		return false;

	return true;
}

uint32_t BankConfiguration::getImageSize(Bank bank)
{
    esp_image_metadata_t data;
    const esp_partition_t *running ;//= esp_partition_find_first(ESP_PARTITION_TYPE_APP,
                                      //                        ESP_PARTITION_SUBTYPE_ANY,
                                    //                         NULL);

    if (bank == Bank::Bank1)
        running = esp_partition_find_first(ESP_PARTITION_TYPE_APP,
                                           ESP_PARTITION_SUBTYPE_ANY,
                                           NULL);
    else
        running = esp_partition_find_first(ESP_PARTITION_TYPE_APP,
                                           ESP_PARTITION_SUBTYPE_ANY,
                                           NULL);


    if (running == nullptr)
        return 0;

    const esp_partition_pos_t running_pos =
        {
            .offset = running->address,
            .size = running->size,
        };

    data.start_addr = running_pos.offset;
    esp_image_verify(ESP_IMAGE_VERIFY, &running_pos, &data);

    return data.image_len;
}

BankConfiguration::~BankConfiguration()
{
}

} // namespace Hal

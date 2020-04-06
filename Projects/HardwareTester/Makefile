#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

CXXFLAGS:=-DHARDWARE_TESTER=1

PROJECT_NAME := app-hardware-tester
include $(IDF_PATH)/make/project.mk


SPIFFS_IMAGE_FLASH_IN_PROJECT := 1
$(eval $(call spiffs_create_partition_image,storage,spiffs))
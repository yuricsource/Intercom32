#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := app-intercom-esp32

EXTRA_CFLAGS := -Wno-unused-local-typedefs

#CXXFLAGS:=-DARDUINO=104

include $(IDF_PATH)/make/project.mk


SPIFFS_IMAGE_FLASH_IN_PROJECT := 1
$(eval $(call spiffs_create_partition_image,storage,spiffs))
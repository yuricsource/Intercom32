#
# Main component makefile.
#
# This Makefile can be left empty. By default, it will take the sources in the 
# src/ directory, compile them and link them into lib(subdirectory_name).a 
# in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.
#



COMPONENT_SRCDIRS           +=  ../../Esp32/Source/Hal                                     \
                                ../../Esp32/Source/Hal/Camera/Conversions                  \
                                ../../Esp32/Source/Hal/Camera/Driver                       \
                                ../../Esp32/Source/Hal/Camera/Sensors                      \
                                ../../Esp32/Source/Hal/Wifi                                \
                                ../System/Source/Utils                                     \
                                ../System/Source/Configuration                             \
                                ../System/Source/Protocol                                  \
                                ../System/Source/Application                               \
                                ../System/Libraries/freertos-addons/Source                 \
                                ../System/Libraries/JsonParser                             \
                                ../System/Libraries/Arduino/Adafruit-GFX                   \
                                ../System/Libraries/Arduino/ArduinoPort

COMPONENT_ADD_INCLUDEDIRS   +=  ${IDF_PATH}/components/                                    \
                                ../../Esp32/Include/Hal                                    \
                                ../../Esp32/Include/Hal/Camera/Conversions                 \
                                ../../Esp32/Include/Hal/Camera/Driver                      \
                                ../../Esp32/Include/Hal/Camera/Sensors                     \
                                ../../Esp32/Include/Hal/Wifi                               \
                                ../System/Include/Utils                                    \
                                ../System/Include/Configuration                            \
                                ../System/Include/Common                                   \
                                ../System/Include/Protocol                                 \
                                ../System/Include/Application                              \
                                ../System/Libraries/freertos-addons/Source/include         \
                                ../System/Libraries/JsonParser                             \
                                ../System/Libraries/Arduino/Adafruit-GFX                   \
                                ../System/Libraries/Arduino/ArduinoPort
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
                                ../../Esp32/Source/Utils                                   \
                                ../../Esp32/Source/Configuration                           \
                                ../../Esp32/Source/Protocol                                \
                                ../../Esp32/Source/Application                             \
                                ../../Esp32/Libraries/freertos-addons/Source               \
                                ../../Esp32/Libraries/JsonParser                           \
                                ../../Esp32/Libraries/Arduino/Adafruit-GFX                 \
                                ../../Esp32/Libraries/Arduino/ArduinoPort

COMPONENT_ADD_INCLUDEDIRS   +=  ${IDF_PATH}/components/                                    \
                                ../../Esp32/Include/Hal                                    \
                                ../../Esp32/Include/Hal/Camera/Conversions                 \
                                ../../Esp32/Include/Hal/Camera/Driver                      \
                                ../../Esp32/Include/Hal/Camera/Sensors                     \
                                ../../Esp32/Include/Hal/Wifi                               \
                                ../../Esp32/Include/Utils                                  \
                                ../../Esp32/Include/Configuration                          \
                                ../../Esp32/Include/Common                                 \
                                ../../Esp32/Include/Protocol                               \
                                ../../Esp32/Include/Application                            \
                                ../../Esp32/Libraries/freertos-addons/Source/include       \
                                ../../Esp32/Libraries/JsonParser                           \
                                ../../Esp32/Libraries/Arduino/Adafruit-GFX                 \
                                ../../Esp32/Libraries/Arduino/ArduinoPort
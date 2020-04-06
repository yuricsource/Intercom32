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
                                ../../Esp32/Source/Middleware/Utils                        \
                                ../../Esp32/Source/Middleware/Configuration                \
                                ../../Esp32/Source/Middleware/Protocol                     \
                                ../System/Source/Application                               \
                                ../System/Libraries/freertos-addons/Source
COMPONENT_ADD_INCLUDEDIRS   +=  ${IDF_PATH}/components/                                    \
                                ../../Esp32/Include/Hal                                    \
                                ../../Esp32/Include/Hal/Camera/Conversions                 \
                                ../../Esp32/Include/Hal/Camera/Driver                      \
                                ../../Esp32/Include/Hal/Camera/Sensors                     \
                                ../../Esp32/Include/Hal/Wifi                               \
                                ../../Esp32/Include/Middleware/Utils                       \
                                ../../Esp32/Include/Middleware/Configuration               \
                                ../../Esp32/Include/Middleware/Common                      \
                                ../../Esp32//Include/Middleware/Protocol                   \
                                ../System/Include/Application                              \
                                ../System/Libraries/freertos-addons/Source/include
/*
 * ArduinoPort.h
 *
 *  Created on: 27 Sep 2019
 *      Author: yuri
 */

#ifndef MAIN_LIBRARIES_ARDUINOPORT_H_
#define MAIN_LIBRARIES_ARDUINOPORT_H_

#include "HalCommon.h"
#include "Hardware.h"

#define PROGMEM

template <typename T>
uint8_t pgm_read_byte (T address)
{
	return *address;
}

// System Port

#define ARDUINO						104

#define LOW							false
#define HIGH						true
#define LSBFIRST 					0
#define MSBFIRST 					1
#define millis						Hal::Hardware::Instance()->Milliseconds
#define digitalWrite(pin, state) 	Hal::Hardware::Instance()->GetGpio().Set(static_cast<Hal::Gpio::GpioIndex>(pin), state)
#define pinMode(pin, mode)			Hal::Hardware::Instance()->GetGpio().ArduinoPinMode(pin,mode)
#define digitalRead(pin)			Hal::Hardware::Instance()->GetGpio().Get(static_cast<Hal::Gpio::GpioIndex>(pin))
#define delay(millis)				vTaskDelay(millis / portTICK_PERIOD_MS)

// SPI Port
#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

#define SPIClass					Hal::Spi
#define SPI							Hal::Hardware::Instance()->GetSpi()

// I2C port
#define Wire						Hal::Hardware::Instance()->GetI2c()
#define beginTransmission			BeginTransmission
#define send						Send
#define endTransmission				EndTransmission
#define requestFrom					RequestFrom
#define receive						Receive
#endif /* MAIN_LIBRARIES_ARDUINOPORT_H_ */

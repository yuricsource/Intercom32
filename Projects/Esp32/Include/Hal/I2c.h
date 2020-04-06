
#ifndef INCLUDE_HAL_I2C_H_
#define INCLUDE_HAL_I2C_H_

#include "HalCommon.h"
#include "Gpio.h"

namespace Hal
{
class I2c
{
public:
	I2c(Gpio *IoPins, I2cPort i2cPort, Gpio::GpioIndex Sda, Gpio::GpioIndex Scl);
	~I2c();
	bool Initialize(Gpio::GpioIndex Sda, Gpio::GpioIndex Scl);
	bool IsDeviceConnected(uint8_t address);
	bool Write(uint8_t slave_addr, uint8_t *data, uint8_t len);
	bool WriteRegister(uint8_t slave_addr, uint8_t byteRegister, uint8_t byte);
	bool Read(uint8_t slave_addr, uint8_t *data, uint32_t len);

	// Arduino Port
	bool BeginTransmission(uint8_t address);
	bool Send(uint8_t byte);
	bool EndTransmission();
	bool RequestFrom(uint8_t slave_addr, uint32_t len);
	uint8_t Receive();

private:
	static constexpr uint32_t DefaultFrequency = 100000;
	static constexpr bool AckCheck = true;
	static constexpr bool AckValue = false;
	uint16_t bytesToReceive = 0;
	Gpio *_gpio;
	I2cPort _i2cPort;
	Gpio::GpioIndex _sdaPin;
	Gpio::GpioIndex _sclPin;

public:
};

} // namespace Hal

#endif /* INCLUDE_HAL_I2C_H_ */


#ifndef HAL_UART_H_
#define HAL_UART_H_

#include "Gpio.h"
#include "HalCommon.h"

namespace Hal
{

class Uart
{

public:
	Uart(Gpio *gpio, UartPort uartPort, uint32_t baudRate, Gpio::GpioIndex txPin, Gpio::GpioIndex rxPin);
	~Uart();

	void Configure(uint32_t baudRate, UartBitLength dataBit,
				   Parity parity, StopBit stopBit, HardwareFlowControl flowControl);
	void Write(const char *data, uint16_t len);
	int Read(uint8_t *data, uint16_t len);
	void Enable();
	void Disable();

private:
	static constexpr uint32_t UartBufferSize = 1024;
	Gpio *_gpio;
	UartPort _uartPort;
	uint32_t _baudRate;
	UartBitLength _dataBit = UartBitLength::Data8Btis;
	Parity _parity = Parity::None;
	StopBit _stopBit = StopBit::StopBit1;
	HardwareFlowControl _flowControl = HardwareFlowControl::Disable;
	Gpio::GpioIndex _txPin;
	Gpio::GpioIndex _rxPin;
	bool _driverEnabled = false;
};

} // namespace Hal

#endif /* HAL_UART_H_ */

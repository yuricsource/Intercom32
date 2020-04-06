
#ifndef HAL_DEBUGPORT_H_
#define HAL_DEBUGPORT_H_

#include "Gpio.h"
#include "HalCommon.h"
namespace Hal
{

class DebugPort
{
public:
	// Gpio Pins for Debug port
	static constexpr Gpio::GpioIndex DebugTx = Gpio::GpioIndex::Gpio1;
	static constexpr Gpio::GpioIndex DebugRx = Gpio::GpioIndex::Gpio3;

	DebugPort(Gpio *gpio, UartPort uartPort, uint32_t baudRate, Gpio::GpioIndex txPin, Gpio::GpioIndex rxPin);

	~DebugPort();
	void Configure(uint32_t baudRate, UartBitLength dataBit,
				   Parity parity, StopBit stopBit, HardwareFlowControl flowControl);
	void Write(const char *data, uint16_t len);
	int Read(uint8_t *data, uint16_t len);
	void Enable();
	void Disable();
	inline bool IsEnabled(){return _debugEnabled;};

private:
	static constexpr uint32_t UartBufferSize = 2048;
	static constexpr uint32_t UartTimeOut = 20 / portTICK_RATE_MS;
	Gpio *_gpio;
	UartPort _uartPort;
	uint32_t _baudRate;
	UartBitLength _dataBit = UartBitLength::Data8Btis;
	Parity _parity = Parity::None;
	StopBit _stopBit = StopBit::StopBit1;
	HardwareFlowControl _flowControl = HardwareFlowControl::Disable;
	Gpio::GpioIndex _txPin;
	Gpio::GpioIndex _rxPin;

	bool _debugEnabled = false;
};

} // namespace Hal

#endif /* HAL_DEBUGPORT_H_ */


#include "DebugPort.h"
#include "driver/uart.h"
#include "Dwt.h"

namespace Hal
{
DebugPort::DebugPort(Gpio *gpio, UartPort uartPort, uint32_t baudRate, Gpio::GpioIndex txPin, Gpio::GpioIndex rxPin) : _gpio(gpio), _uartPort(uartPort), _baudRate(baudRate), _txPin(txPin), _rxPin(rxPin)
{
	uart_config_t uart_config = {};
	uart_config.baud_rate = static_cast<int>(_baudRate);
	uart_config.data_bits = UART_DATA_8_BITS;
	uart_config.parity = UART_PARITY_DISABLE;
	uart_config.stop_bits = UART_STOP_BITS_1;
	uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
	uart_param_config(UART_NUM_0, &uart_config);
	_gpio->SetAlternate(_txPin, Gpio::AltFunc::Uart);
	_gpio->SetAlternate(_rxPin, Gpio::AltFunc::Uart);
	_debugEnabled = true;
}
DebugPort::~DebugPort()
{
}
void DebugPort::Configure(uint32_t baudRate, UartBitLength dataBit,
					 Parity parity, StopBit stopBit, HardwareFlowControl flowControl)
{
	_baudRate = baudRate;
	_dataBit = dataBit;
	_parity = parity;
	_stopBit = stopBit;
	_flowControl = flowControl;

	Disable();
	Dwt::DelayMilliseconds(50);
	Enable();
}

void DebugPort::Enable()
{
	if (_debugEnabled)
		return;

	_gpio->SetAlternate(_txPin, Gpio::AltFunc::Uart);
	_gpio->SetAlternate(_rxPin, Gpio::AltFunc::Uart);

	uart_config_t uart_config = {};
	uart_config.baud_rate = static_cast<int>(_baudRate);
	uart_config.data_bits = static_cast<uart_word_length_t>(_dataBit);
	uart_config.parity = static_cast<uart_parity_t>(_parity);
	uart_config.stop_bits = static_cast<uart_stop_bits_t>(_stopBit);
	uart_config.flow_ctrl = static_cast<uart_hw_flowcontrol_t>(_flowControl);
	uart_config.rx_flow_ctrl_thresh = 0;
	uart_config.use_ref_tick = false;

	uart_param_config(static_cast<uart_port_t>(_uartPort), &uart_config);
	uart_set_pin(static_cast<uart_port_t>(_uartPort), static_cast<int>(_txPin), static_cast<int>(_rxPin), 0, 0);
	uart_driver_install(static_cast<uart_port_t>(_uartPort), UartBufferSize, 0, 0, NULL, 0);

	_debugEnabled = true;
}

void DebugPort::Disable()
{
	if (_debugEnabled == false)
		return;

	uart_driver_delete(static_cast<uart_port_t>(_uartPort));
	_gpio->SetAlternate(_txPin, Gpio::AltFunc::Gpio);
	_gpio->SetAlternate(_rxPin, Gpio::AltFunc::Gpio);

	_debugEnabled = false;
}

void DebugPort::Write(const char *data, uint16_t len)
{
	uart_write_bytes(static_cast<uart_port_t>(_uartPort), data, len);
}

int DebugPort::Read(uint8_t *data, uint16_t len)
{
	return uart_read_bytes(static_cast<uart_port_t>(_uartPort), data, len, UartTimeOut);
}
} // namespace Hal

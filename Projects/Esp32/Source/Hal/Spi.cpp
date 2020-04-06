
#include <Spi.h>
#include "driver/spi_master.h"

namespace Hal
{

#define LCD_HOST    HSPI_HOST
#define DMA_CHAN    2

#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22

Spi::Spi()
{
    spi_device_handle_t spi;
    spi_bus_config_t buscfg = { };
    buscfg.miso_io_num = PIN_NUM_MISO;
    buscfg.mosi_io_num = PIN_NUM_MOSI;
    buscfg.sclk_io_num = PIN_NUM_CLK;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    buscfg.max_transfer_sz = 1024;

    spi_device_interface_config_t devcfg = { };
    devcfg.clock_speed_hz = 10*1000*1000;           //Clock out at 10 MHz
    devcfg.mode = 0;                                //SPI mode 0
    devcfg.spics_io_num = PIN_NUM_CS;               //CS pin
    devcfg.queue_size = 7;                          //We want to be able to queue 7 transactions at a time
    devcfg.pre_cb = nullptr;                        //Specify pre-transfer callback to handle D/C line
    devcfg.post_cb = nullptr;                        //Specify pos-transfer callback to handle D/C line

    // //Initialize the SPI bus
    // ret=spi_bus_initialize(LCD_HOST, &buscfg, DMA_CHAN);
    // ESP_ERROR_CHECK(ret);
    // //Attach the LCD to the SPI bus
    // ret=spi_bus_add_device(LCD_HOST, &devcfg, &spi);
    // ESP_ERROR_CHECK(ret);
}

Spi::~Spi()
{
}

void Spi::begin()
{

}

uint8_t Spi::transfer(uint8_t data)
{
    return 0;
}

void Spi::transfer(uint8_t* data, uint32_t size)
{

}

void Spi::setBitOrder(uint8_t bitOrder)
{

}

void Spi::setDataMode(uint8_t dataMode)
{

}


} // namespace Hal

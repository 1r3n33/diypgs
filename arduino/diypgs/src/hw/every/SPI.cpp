#ifdef ARDUINO_AVR_NANO_EVERY

#include "SPI.h"

namespace hw
{
    // https://www.arduino.cc/reference/en/language/functions/communication/spi/begin/
    // Initializes the SPI bus by setting SCK, MOSI, and SS to outputs, pulling SCK and MOSI low, and SS high.
    void SpiClass::begin() const
    {
        pinMode(MOSI, OUTPUT);
        pinMode(MISO, INPUT);
        pinMode(SCK, OUTPUT);
        pinMode(SS, OUTPUT);

        digitalWrite(MOSI, LOW);
        digitalWrite(SCK, LOW);
        digitalWrite(SS, HIGH);

        PORTMUX.TWISPIROUTEA |= MUX_SPI; // Acquire the LED_BUILTIN pin
        SPI0.CTRLA = DEFAULT_SPI_SETTINGS.SPI0CTRLA;
    }

    // https://www.arduino.cc/reference/en/language/functions/communication/spi/end/
    // Disables the SPI bus (leaving pin modes unchanged).
    void SpiClass::end() const
    {
        SPI0.CTRLA &= ~SPI_ENABLE_bm;
    }

    // https://www.arduino.cc/reference/en/language/functions/communication/spi/begintransaction/
    // Initializes the SPI bus using the defined SPISettings.
    void SpiClass::beginTransaction(const SPISettings settings) const
    {
        SPI0.CTRLA = settings.SPI0CTRLA;
    }

    void SpiClass::endTransaction() const
    {
    }

    void SpiClass::setBitOrder(const BitOrder /*bitOrder*/) const
    {
        // TODO
    }

    void SpiClass::setClockDivider(const uint8_t /*clockDivider*/) const
    {
        // TODO
    }

    void SpiClass::setDateMode(const DataMode /*dataMode*/) const
    {
        // TODO
    }

    // https://www.arduino.cc/reference/en/language/functions/communication/spi/transfer/
    // SPI transfer is based on a simultaneous send and receive: the received data is returned in receivedVal (or receivedVal16).
    // In case of buffer transfers the received data is stored in the buffer in-place (the old data is replaced with the data received).
    uint8_t SpiClass::transfer(const uint8_t data) const
    {
        SPI0.DATA = data;

        // Waits until data is exchanged
        while (!(SPI0.INTFLAGS & SPI_IF_bm))
        {
            ;
        }

        return SPI0.DATA;
    }

    uint16_t SpiClass::transfer16(const uint16_t /*data*/) const
    {
        // TODO
        return 0;
    }

    void SpiClass::transfer(uint8_t *data, const uint32_t size) const
    {
        const uint8_t *const end = data + size;
        while (data != end)
        {
            *data = transfer(*data);
            data++;
        }
    }

    void SpiClass::usingInterrupt(const uint32_t /*interruptNumber*/) const
    {
        // TODO
    }
}

#endif // ARDUINO_AVR_NANO_EVERY

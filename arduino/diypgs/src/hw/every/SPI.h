#pragma once
#ifdef ARDUINO_AVR_NANO_EVERY

#include "Arduino.h"

namespace hw
{
    enum DataMode : uint8_t
    {
        SPI_MODE0 = 0,
    };

    class SPISettings
    {
    public:
        constexpr SPISettings(const uint32_t /*clock*/, const BitOrder /*bitOrder*/, const DataMode /*dataMode*/) : SPI0CTRLA(SPI_CLK2X_bm | SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESC_DIV16_gc)
        {
        }

    private:
        const uint8_t SPI0CTRLA;

        friend class SpiClass;
    };

    class SpiClass
    {
    public:
        void begin() const;
        void end() const;

        void beginTransaction(const SPISettings settings) const;
        void endTransaction() const;

        void setBitOrder(const BitOrder bitOrder) const;
        void setClockDivider(const uint8_t clockDivider) const;
        void setDateMode(const DataMode dataMode) const;

        uint8_t transfer(const uint8_t data) const;
        uint16_t transfer16(const uint16_t data) const;
        void transfer(uint8_t *data, const uint32_t size) const;

        void usingInterrupt(const uint32_t interruptNumber) const;
    };

    static constexpr SpiClass SPI;
    static constexpr SPISettings DEFAULT_SPI_SETTINGS(F_CPU, BitOrder::MSBFIRST, DataMode::SPI_MODE0);
}

#endif // ARDUINO_AVR_NANO_EVERY

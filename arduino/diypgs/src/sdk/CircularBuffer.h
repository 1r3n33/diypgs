#pragma once

#include "Arduino.h"

namespace sdk
{
    class CircularBuffer
    {
    public:
        void setup(const uint8_t value)
        {
            for (uint8_t i = 0; i < BUFFER_SIZE; i++)
            {
                buffer[i] = value;
            }
            index = BUFFER_SIZE - 1; // First push will move index at zero.
        }

        // Move the current index and set the value.
        void push(const uint8_t value)
        {
            index = (index + 1) % BUFFER_SIZE;
            buffer[index] = value;
        }

        // Get the d-th previous value from index (index is kept at last push position).
        // d can't be greater than BUFFER_SIZE.
        uint8_t pop(const uint8_t d) const
        {
            const uint8_t i = (index + BUFFER_SIZE - d) % BUFFER_SIZE;
            return buffer[i];
        }

    private:
        static constexpr uint8_t BUFFER_SIZE = 16;

        uint8_t buffer[BUFFER_SIZE];
        uint8_t index;
    };
}

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

class CircularBuffer
{
public:
    static const uint8_t BUFFER_SIZE = 16;

private:
    uint8_t buffer[BUFFER_SIZE];
    uint8_t index;

public:
    CircularBuffer()
    {
    }

    void setup(const uint8_t value)
    {
        index = 0;
        for (uint8_t i = 0; i < BUFFER_SIZE; i++)
        {
            buffer[i] = value;
        }
    }

    void push(const uint8_t value)
    {
        buffer[index] = value;
        index = (index + 1) % BUFFER_SIZE;
    }

    uint8_t pop(const uint8_t delta) const
    {
        const uint8_t i = (index + BUFFER_SIZE - delta) % BUFFER_SIZE;
        return buffer[i];
    }
};

#endif

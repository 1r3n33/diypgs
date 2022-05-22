#pragma once

#include "Arduino.h"

class Gamepad
{
public:
    enum Button : uint8_t
    {
        UP = 0x01,
        DOWN = 0x02,
        LEFT = 0x04,
        RIGHT = 0x08,
        A = 0x10,
        B = 0x20,
    };

public:
    Gamepad(const uint8_t button_up_pin,
            const uint8_t button_down_pin,
            const uint8_t button_left_pin,
            const uint8_t button_right_pin,
            const uint8_t button_a_pin,
            const uint8_t button_b_pin);

public:
    void setup() const;
    uint8_t get() const;

private:
    const uint8_t BUTTON_UP_PIN;
    const uint8_t BUTTON_DOWN_PIN;
    const uint8_t BUTTON_LEFT_PIN;
    const uint8_t BUTTON_RIGHT_PIN;
    const uint8_t BUTTON_A_PIN;
    const uint8_t BUTTON_B_PIN;
};

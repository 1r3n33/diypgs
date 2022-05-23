#include "Gamepad.h"

namespace hw
{
    Gamepad::Gamepad(const uint8_t button_up_pin,
                     const uint8_t button_down_pin,
                     const uint8_t button_left_pin,
                     const uint8_t button_right_pin,
                     const uint8_t button_a_pin,
                     const uint8_t button_b_pin) : BUTTON_UP_PIN(button_up_pin),
                                                   BUTTON_DOWN_PIN(button_down_pin),
                                                   BUTTON_LEFT_PIN(button_left_pin),
                                                   BUTTON_RIGHT_PIN(button_right_pin),
                                                   BUTTON_A_PIN(button_a_pin),
                                                   BUTTON_B_PIN(button_b_pin)
    {
    }

    void Gamepad::setup() const
    {
        pinMode(BUTTON_UP_PIN, INPUT);
        pinMode(BUTTON_DOWN_PIN, INPUT);
        pinMode(BUTTON_LEFT_PIN, INPUT);
        pinMode(BUTTON_RIGHT_PIN, INPUT);
        pinMode(BUTTON_A_PIN, INPUT);
        pinMode(BUTTON_B_PIN, INPUT);
    }

    uint8_t Gamepad::get() const
    {
        uint8_t out = 0;
        out |= digitalRead(BUTTON_UP_PIN) == HIGH ? Button::UP : 0;
        out |= digitalRead(BUTTON_DOWN_PIN) == HIGH ? Button::DOWN : 0;
        out |= digitalRead(BUTTON_LEFT_PIN) == HIGH ? Button::LEFT : 0;
        out |= digitalRead(BUTTON_RIGHT_PIN) == HIGH ? Button::RIGHT : 0;
        out |= digitalRead(BUTTON_A_PIN) == HIGH ? Button::A : 0;
        out |= digitalRead(BUTTON_B_PIN) == HIGH ? Button::B : 0;
        return out;
    }
}

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller
{
private:
    uint8_t pins[6];

public:
    static const uint8_t BUTTON_UP = 0x01;
    static const uint8_t BUTTON_DOWN = 0x02;
    static const uint8_t BUTTON_LEFT = 0x04;
    static const uint8_t BUTTON_RIGHT = 0x08;
    static const uint8_t BUTTON_A = 0x10;
    static const uint8_t BUTTON_B = 0x20;

    Controller()
    {
    }

    void setup(const uint8_t button_up_pin,
               const uint8_t button_down_pin,
               const uint8_t button_left_pin,
               const uint8_t button_right_pin,
               const uint8_t button_a_pin,
               const uint8_t button_b_pin)
    {
        pins[0] = button_up_pin;
        pins[1] = button_down_pin;
        pins[2] = button_left_pin;
        pins[3] = button_right_pin;
        pins[4] = button_a_pin;
        pins[5] = button_b_pin;

        pinMode(pins[0], INPUT);
        pinMode(pins[1], INPUT);
        pinMode(pins[2], INPUT);
        pinMode(pins[3], INPUT);
        pinMode(pins[4], INPUT);
        pinMode(pins[5], INPUT);
    }

    uint8_t get() const
    {
        uint8_t out = 0;
        out |= digitalRead(pins[0]) == HIGH ? BUTTON_UP : 0;
        out |= digitalRead(pins[1]) == HIGH ? BUTTON_DOWN : 0;
        out |= digitalRead(pins[2]) == HIGH ? BUTTON_LEFT : 0;
        out |= digitalRead(pins[3]) == HIGH ? BUTTON_RIGHT : 0;
        out |= digitalRead(pins[4]) == HIGH ? BUTTON_A : 0;
        out |= digitalRead(pins[5]) == HIGH ? BUTTON_B : 0;
        return out;
    }
};

#endif

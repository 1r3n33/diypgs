#include "Buzzer.h"

namespace hw
{
    Buzzer::Buzzer(const uint8_t pin) : PIN(pin)
    {
    }

    void Buzzer::setup() const
    {
    }

    void Buzzer::play(const Note note, const uint16_t msDuration) const
    {
        tone(PIN, static_cast<unsigned int>(note), msDuration);
    }
}

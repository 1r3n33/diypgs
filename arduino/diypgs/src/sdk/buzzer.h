#ifndef BUZZER_H
#define BUZZER_H

class Buzzer
{
public:
    static const uint16_t NOTE_B0 = 31;
    static const uint16_t NOTE_C1 = 33;
    static const uint16_t NOTE_CS1 = 35;
    static const uint16_t NOTE_D1 = 37;
    static const uint16_t NOTE_DS1 = 39;
    static const uint16_t NOTE_E1 = 41;
    static const uint16_t NOTE_F1 = 44;
    static const uint16_t NOTE_FS1 = 46;
    static const uint16_t NOTE_G1 = 49;
    static const uint16_t NOTE_GS1 = 52;
    static const uint16_t NOTE_A1 = 55;
    static const uint16_t NOTE_AS1 = 58;
    static const uint16_t NOTE_B1 = 62;
    static const uint16_t NOTE_C2 = 65;
    static const uint16_t NOTE_CS2 = 69;
    static const uint16_t NOTE_D2 = 73;
    static const uint16_t NOTE_DS2 = 78;
    static const uint16_t NOTE_E2 = 82;
    static const uint16_t NOTE_F2 = 87;
    static const uint16_t NOTE_FS2 = 93;
    static const uint16_t NOTE_G2 = 98;
    static const uint16_t NOTE_GS2 = 104;
    static const uint16_t NOTE_A2 = 110;
    static const uint16_t NOTE_AS2 = 117;
    static const uint16_t NOTE_B2 = 123;
    static const uint16_t NOTE_C3 = 131;
    static const uint16_t NOTE_CS3 = 139;
    static const uint16_t NOTE_D3 = 147;
    static const uint16_t NOTE_DS3 = 156;
    static const uint16_t NOTE_E3 = 165;
    static const uint16_t NOTE_F3 = 175;
    static const uint16_t NOTE_FS3 = 185;
    static const uint16_t NOTE_G3 = 196;
    static const uint16_t NOTE_GS3 = 208;
    static const uint16_t NOTE_A3 = 220;
    static const uint16_t NOTE_AS3 = 233;
    static const uint16_t NOTE_B3 = 247;
    static const uint16_t NOTE_C4 = 262;
    static const uint16_t NOTE_CS4 = 277;
    static const uint16_t NOTE_D4 = 294;
    static const uint16_t NOTE_DS4 = 311;
    static const uint16_t NOTE_E4 = 330;
    static const uint16_t NOTE_F4 = 349;
    static const uint16_t NOTE_FS4 = 370;
    static const uint16_t NOTE_G4 = 392;
    static const uint16_t NOTE_GS4 = 415;
    static const uint16_t NOTE_A4 = 440;
    static const uint16_t NOTE_AS4 = 466;
    static const uint16_t NOTE_B4 = 494;
    static const uint16_t NOTE_C5 = 523;
    static const uint16_t NOTE_CS5 = 554;
    static const uint16_t NOTE_D5 = 587;
    static const uint16_t NOTE_DS5 = 622;
    static const uint16_t NOTE_E5 = 659;
    static const uint16_t NOTE_F5 = 698;
    static const uint16_t NOTE_FS5 = 740;
    static const uint16_t NOTE_G5 = 784;
    static const uint16_t NOTE_GS5 = 831;
    static const uint16_t NOTE_A5 = 880;
    static const uint16_t NOTE_AS5 = 932;
    static const uint16_t NOTE_B5 = 988;
    static const uint16_t NOTE_C6 = 1047;
    static const uint16_t NOTE_CS6 = 1109;
    static const uint16_t NOTE_D6 = 1175;
    static const uint16_t NOTE_DS6 = 1245;
    static const uint16_t NOTE_E6 = 1319;
    static const uint16_t NOTE_F6 = 1397;
    static const uint16_t NOTE_FS6 = 1480;
    static const uint16_t NOTE_G6 = 1568;
    static const uint16_t NOTE_GS6 = 1661;
    static const uint16_t NOTE_A6 = 1760;
    static const uint16_t NOTE_AS6 = 1865;
    static const uint16_t NOTE_B6 = 1976;
    static const uint16_t NOTE_C7 = 2093;
    static const uint16_t NOTE_CS7 = 2217;
    static const uint16_t NOTE_D7 = 2349;
    static const uint16_t NOTE_DS7 = 2489;
    static const uint16_t NOTE_E7 = 2637;
    static const uint16_t NOTE_F7 = 2794;
    static const uint16_t NOTE_FS7 = 2960;
    static const uint16_t NOTE_G7 = 3136;
    static const uint16_t NOTE_GS7 = 3322;
    static const uint16_t NOTE_A7 = 3520;
    static const uint16_t NOTE_AS7 = 3729;
    static const uint16_t NOTE_B7 = 3951;
    static const uint16_t NOTE_C8 = 4186;
    static const uint16_t NOTE_CS8 = 4435;
    static const uint16_t NOTE_D8 = 4699;
    static const uint16_t NOTE_DS8 = 4978;

private:
    uint8_t pin;

public:
    Buzzer()
    {
    }

    void setup(const uint8_t pin)
    {
        this->pin = pin;
    }

    void play(const uint16_t note, const uint16_t duration) const
    {
        tone(pin, note, duration);
    }
};

#endif

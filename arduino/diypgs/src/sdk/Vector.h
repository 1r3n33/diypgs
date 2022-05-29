#pragma once

namespace sdk
{
    class fixed16_t
    {
    public:
        fixed16_t()
        {
        }

        explicit constexpr fixed16_t(const int integer) : i8f8(integer * 256)
        {
        }

        static inline fixed16_t from(const int16_t i8f8)
        {
            fixed16_t f16;
            f16.i8f8 = i8f8;
            return f16;
        }

    public:
        constexpr int8_t toInt8() const
        {
            return i8f8 / 256;
        }

        fixed16_t operator+(const fixed16_t &other) const
        {
            return from(i8f8 + other.i8f8);
        }

        fixed16_t operator-(const fixed16_t &other) const
        {
            return from(i8f8 - other.i8f8);
        }

        void operator+=(const fixed16_t &other)
        {
            i8f8 += other.i8f8;
        }

        fixed16_t operator*(const int integer) const
        {
            return from(i8f8 * integer);
        }

        void operator*=(const int integer)
        {
            i8f8 *= integer;
        }

        constexpr bool operator<(const fixed16_t &other) const
        {
            return i8f8 < other.i8f8;
        }

        constexpr bool operator>(const fixed16_t &other) const
        {
            return i8f8 > other.i8f8;
        }

        fixed16_t operator-() const
        {
            return from(-i8f8);
        }

    private:
        int16_t i8f8;
    };

    class Vec2f16
    {
    public:
        Vec2f16()
        {
        }

        void add(const Vec2f16 &other)
        {
            x += other.x;
            y += other.y;
        }

    public:
        fixed16_t x;
        fixed16_t y;
    };
}

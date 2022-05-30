#pragma once

namespace sdk
{
    class fixed16_t
    {
    public:
        inline fixed16_t()
        {
        }

        constexpr explicit fixed16_t(const int integer, const int fractional = 0) : i8f8((integer * 256) | fractional)
        {
        }

    public:
        inline int8_t toInt8() const
        {
            return i8f8 / 256;
        }

        inline fixed16_t operator+(const fixed16_t &other) const
        {
            return fixed16_t(0, i8f8 + other.i8f8);
        }

        inline fixed16_t operator-(const fixed16_t &other) const
        {
            return fixed16_t(0, i8f8 - other.i8f8);
        }

        inline fixed16_t operator*(const int integer) const
        {
            return fixed16_t(0, i8f8 * integer);
        }

        inline void operator+=(const fixed16_t &other)
        {
            i8f8 += other.i8f8;
        }

        inline void operator-=(const fixed16_t &other)
        {
            i8f8 -= other.i8f8;
        }

        inline void operator*=(const int integer)
        {
            i8f8 *= integer;
        }

        inline fixed16_t operator-() const
        {
            return fixed16_t(0, -i8f8);
        }

        inline bool operator<(const fixed16_t &other) const
        {
            return i8f8 < other.i8f8;
        }

        inline bool operator>(const fixed16_t &other) const
        {
            return i8f8 > other.i8f8;
        }

    private:
        int16_t i8f8; // 8 bits for integer part; 8 bits for fractional part;
    };

    class Vec2f16
    {
    public:
        inline Vec2f16()
        {
        }

        constexpr explicit Vec2f16(const fixed16_t x_, const fixed16_t y_) : x(x_), y(y_)
        {
        }

        inline void operator+=(const Vec2f16 &other)
        {
            x += other.x;
            y += other.y;
        }

        inline Vec2f16 operator-(const Vec2f16 &other)
        {
            return Vec2f16(x - other.x, y - other.y);
        }

    public:
        fixed16_t x;
        fixed16_t y;
    };
}

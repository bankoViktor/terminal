#pragma once

using int8_t = signed char;
using int16_t = signed short;
using int32_t = signed int;

using uint8_t = unsigned char;
using uint16_t = unsigned short;
using uint32_t = unsigned int;

using color_t = struct color_t
{
    uint32_t value;

    color_t(uint8_t red, uint8_t green, uint8_t blue)
    {
        value = red | (green << 8) | (blue << 16);
    }
    operator uint32_t() const
    {
        return value;
    }
};

using horizontal_aligment_t = enum class horizontal_aligment_t
{
    Left,
    Center,
    Right
};

using vertical_aligment_t = enum class vertical_aligment_t
{
    Top,
    Middle,
    Bottom
};

using button_t = struct button_t
{
    char m_pszTitle[16];
};

using coord_t = int16_t;

using point_t = struct point_t
{
    coord_t x;
    coord_t y;

    point_t()
    {
        this->x = 0;
        this->y = 0;
    }
    point_t(coord_t x, coord_t y)
    {
        this->x = x;
        this->y = y;
    }
    void erase()
    {
        this->x = 0;
        this->y = 0;
    }
};

using rect_t = struct rect_t
{
    coord_t left;
    coord_t top;
    coord_t right;
    coord_t bottom;

    rect_t()
    {
        this->left = 0;
        this->top = 0;
        this->right = 0;
        this->bottom = 0;
    }
    rect_t(coord_t left, coord_t top, coord_t right, coord_t bottom)
    {
        this->left = left;
        this->top = top;
        this->right = right;
        this->bottom = bottom;
    }
    rect_t(const point_t& lt, coord_t width, coord_t height)
    {
        this->left = lt.x;
        this->top = lt.y;
        this->right = lt.x + width;
        this->bottom = lt.y + height;
    }
    void size(coord_t left, coord_t top, coord_t width, coord_t height)
    {
        this->left = left;
        this->top = top;
        this->right = left + width;
        this->bottom = top + height;
    }
    void erase()
    {
        this->left = 0;
        this->top = 0;
        this->right = 0;
        this->bottom = 0;
    }
    bool empty() const
    {
        return this->width() == 0 && this->height() == 0;
    }
    coord_t width() const
    {
        return right - left;
    }
    coord_t height() const
    {
        return bottom - top;
    }
    point_t lt() const
    {
        return point_t(this->left, this->top);
    }
    point_t rb() const
    {
        return point_t(this->right, this->bottom);
    }
    void offset(coord_t dx, coord_t dy)
    {
        this->left += dx;
        this->top += dy;
        this->right += dx;
        this->bottom += dy;
    }
    void inflate(coord_t dx, coord_t dy)
    {
        this->left -= dx;
        this->top -= dy;
        this->right += dx;
        this->bottom += dy;
    }
    void inflate(coord_t delta)
    {
        this->left -= delta;
        this->top -= delta;
        this->right += delta;
        this->bottom += delta;
    }
};

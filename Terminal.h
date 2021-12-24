#pragma once

#include "types.h"
#include "navigator.h"


template <typename T>
struct terminal_t
{
    T               context;
    navigator_t<T>  navigator;

    terminal_t(page_selector_f<T> selector, uint8_t pageIndex);
    void size(const rect_t& rc);
    void render();
    void input(uint8_t value);

    void calcButtonPos(uint8_t index, point_t& pt, int16_t delta = 0) const;
    void getClientRect(rect_t& rc) const;
    void drawButtons(const button_t* buttons) const;

#ifdef TERMINAL_DEBUG
    void drawClientRect() const;
    void drawButtonGrid() const;
    void drawMarker(const point_t& pt, const color_t& color) const;
#endif // TERMINAL_DEBUG
};


#pragma region Impl terminal_t

template<typename T>
inline terminal_t<T>::terminal_t(page_selector_f<T> selector, uint8_t pageIndex) :
    navigator(selector, pageIndex)
{
}

template<typename T>
void terminal_t<T>::calcButtonPos(uint8_t index, point_t& pt, int16_t delta) const
{
    rect_t rc = this->context.rc;

    auto stepX = (double)(rc.width() + BUTTON_STRECH_X * 2) / (BUTTON_COUNT_X + 1);
    auto stepY = (double)(rc.height() + BUTTON_STRECH_Y * 2) / (BUTTON_COUNT_Y + 1);

    if (index < BUTTONS_TOP)
    {
        // Top
        pt.x = rc.left - BUTTON_STRECH_X + BUTTON_MOVE_X + (coord_t)(stepX * (index + 1));
        pt.y = rc.top + delta;
    }
    else if (index < BUTTONS_RIGHT)
    {
        // Right
        pt.x = rc.right - 1 - delta;
        pt.y = rc.top - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (coord_t)(stepY * (index - BUTTONS_TOP + 1));
    }
    else if (index < BUTTONS_BOTTOM)
    {
        // Bottom
        pt.x = rc.left - BUTTON_STRECH_X + BUTTON_MOVE_X + (coord_t)(stepX * (BUTTON_COUNT_X - index + BUTTONS_RIGHT));
        pt.y = rc.bottom - 1 - delta;
    }
    else if (index < BUTTONS_LEFT)
    {
        // Left
        pt.x = rc.left + delta;
        pt.y = rc.top - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (coord_t)(stepY * (BUTTON_COUNT_Y - index + BUTTONS_BOTTOM));
    }
}

template<typename T>
void terminal_t<T>::getClientRect(rect_t& rc) const
{
    rc = this->context.rc;
    rc.inflate(-SAFE_OFFSET);
}

#ifdef TERMINAL_DEBUG

template <typename T>
void terminal_t<T>::drawButtonGrid() const
{
    auto rc = this->context.rc;
    point_t pt;

    for (auto i = 0; i < BUTTON_COUNT_X; ++i)
    {
        this->calcButtonPos(i, pt);
        context.moveTo(pt.x, rc.top);
        context.lineTo(pt.x, rc.bottom, GRID_COLOR);
    }

    for (auto i = BUTTON_COUNT_X; i < (BUTTON_COUNT_X + BUTTON_COUNT_Y); ++i)
    {
        this->calcButtonPos(i, pt);
        context.moveTo(rc.left, pt.y);
        context.lineTo(rc.right, pt.y, GRID_COLOR);
    }
}

template <typename T>
void terminal_t<T>::drawMarker(const point_t& pt, const color_t& color) const
{
    context.moveTo(pt.x - BUTTON_MARKER_SIZE, pt.y);
    context.lineTo(pt.x + BUTTON_MARKER_SIZE, pt.y, MARKER_COLOR);
    context.moveTo(pt.x, pt.y - BUTTON_MARKER_SIZE);
    context.lineTo(pt.x, pt.y + BUTTON_MARKER_SIZE, MARKER_COLOR);
}

template <typename T>
void terminal_t<T>::drawClientRect() const
{
    rect_t rc;
    this->getClientRect(rc);
    rc.inflate(1);
    this->context.rect(rc, CLIENT_RECT_COLOR);
}

#endif // TERMINAL_DEBUG

template <typename T>
void terminal_t<T>::input(uint8_t value)
{
    navigator.top().input(*this, value);
}

template<typename T>
void terminal_t<T>::size(const rect_t& rc)
{
    this->context.rc = rc;
}

template <typename T>
void terminal_t<T>::render()
{
#ifdef TERMINAL_DEBUG

    drawButtonGrid();
    drawClientRect();

#endif // TERMINAL_DEBUG

    const page_info_t<T>& page = navigator.top();
    page.render(*this, page.pdata);
}

template <typename T>
void terminal_t<T>::drawButtons(const button_t* buttons) const
{
    auto hAlign = horizontal_aligment_t::Center;
    auto vAlign = vertical_aligment_t::Top;
    auto dx = 0;
    auto dy = 0;

    for (auto i = 0; i < BUTTON_COUNT; ++i)
    {
        auto pButton = &buttons[i];

        // Align by Side
        if (i == BUTTONS_TOP)
        {
            // Right
            hAlign = horizontal_aligment_t::Right;
            vAlign = vertical_aligment_t::Middle;
        }
        else if (i == BUTTONS_LEFT)
        {
            // Bottom
            hAlign = horizontal_aligment_t::Center;
            vAlign = vertical_aligment_t::Bottom;
        }
        else if (i == BUTTONS_BOTTOM)
        {
            // Left
            hAlign = horizontal_aligment_t::Left;
            vAlign = vertical_aligment_t::Middle;
        }

        // Calc Text Rect
        rect_t rc;
        this->context.calcText(rc, pButton->szTitle, hAlign, vAlign);

        // Offset Text by Side
        if (i < BUTTONS_TOP)
        {
            // Top
            dx = -rc.width() / 2;
            dy = 0;
        }
        else if (i < BUTTONS_RIGHT)
        {
            // Right
            dx = -rc.width() + 1;
            dy = -rc.height() / 2;
        }
        else if (i < BUTTONS_BOTTOM)
        {
            // Bottom
            dx = -rc.width() / 2;
            dy = -rc.height() + 1;
        }
        else if (i < BUTTONS_LEFT)
        {
            // Left
            dx = 0;
            dy = -rc.height() / 2;
        }

        point_t pt;
        this->calcButtonPos(i, pt, BUTTON_OFFSET);

        rc.offset(pt.x + dx, pt.y + dy);

        // Draw Label
        this->context.text(rc, pButton->szTitle,
            TEXT_COLOR, TEXT_BGCOLOR, hAlign, vAlign);
    }
}

#pragma endregion

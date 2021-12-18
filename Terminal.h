#pragma once

#include "types.h"
#include "page.h"


template <typename T>
struct terminal_t
{
    T context;
    page_t<T>& current;

    terminal_t(page_t<T>& home);
    void display(const rect_t& rc);
    void render() const;
    void input(uint8_t index);

#ifdef TERMINAL_DEBUG
    void drawClientRect() const;
    void drawButtonGrid() const;
    void drawMarker(const point_t& pt, const color_t& color) const;
#endif // TERMINAL_DEBUG

};


#pragma region Impl terminal_t

template<typename T>
inline terminal_t<T>::terminal_t(page_t<T>& home) :
    current(home)
{
}

#ifdef TERMINAL_DEBUG

template <typename T>
void terminal_t<T>::drawButtonGrid() const
{
    auto rc = this->context.rc;
    point_t pt;

    for (auto i = 0; i < BUTTON_COUNT_X; ++i)
    {
        this->context.calcButtonPos(i, pt);
        context.moveTo(pt.x, rc.top);
        context.lineTo(pt.x, rc.bottom, GRID_COLOR);
    }

    for (auto i = BUTTON_COUNT_X; i < (BUTTON_COUNT_X + BUTTON_COUNT_Y); ++i)
    {
        this->context.calcButtonPos(i, pt);
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
    this->context.getClientRect(rc);
    rc.inflate(1);
    this->context.rect(rc, CLIENT_RECT_COLOR);
}

#endif // TERMINAL_DEBUG

template <typename T>
void terminal_t<T>::input(uint8_t index)
{
    this->current.input(index);
}

template<typename T>
void terminal_t<T>::display(const rect_t& rc)
{
    this->context.rc = rc;
}

template <typename T>
void terminal_t<T>::render() const
{
#ifdef TERMINAL_DEBUG

    drawButtonGrid();
    drawClientRect();

#endif // TERMINAL_DEBUG

    this->current.render(this->context);
}

#pragma endregion

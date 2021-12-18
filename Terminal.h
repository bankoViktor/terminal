#pragma once

#include "terminal_config.h"
#include "render_context.h"


#define BUTTON_COUNT                ((BUTTON_COUNT_X + BUTTON_COUNT_Y) * 2)
#define BUTTONS_TOP                 (BUTTON_COUNT_X)
#define BUTTONS_RIGHT               (BUTTON_COUNT_X + BUTTON_COUNT_Y)
#define BUTTONS_BOTTOM              (BUTTON_COUNT_X * 2 + BUTTON_COUNT_Y)
#define BUTTONS_LEFT                (BUTTON_COUNT)


typedef struct terminal_t
{
    render_context_t m_context;

    void init(const render_context_t& context);
    void render() const;
    void getClientRect(rect_t& rc) const;
    void calcButtonPos(uint8_t index, point_t& pt, int16_t delta = 0) const;
    void active(uint8_t index);

#ifdef TERMINAL_DEBUG
    void drawClientRect() const;
    void drawButtonGrid() const;
    void drawButtonsMarker() const;
#endif // TERMINAL_DEBUG

} terminal_t;

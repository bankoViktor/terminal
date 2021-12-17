#pragma once

#include "resource.h"
#include "framework.h"
#include "render_context.h"
#include "TerminalPage.h"


#define RECT_WIDTH(rc)          ((rc).right  - (rc).left)
#define RECT_HEIGHT(rc)         ((rc).bottom - (rc).top)

#define BUTTONS_TOP             (BUTTON_COUNT_X)
#define BUTTONS_RIGHT           (BUTTON_COUNT_X + BUTTON_COUNT_Y)
#define BUTTONS_BOTTOM          (BUTTON_COUNT_X * 2 + BUTTON_COUNT_Y)
#define BUTTONS_LEFT            (BUTTON_COUNT)





class Terminal
{
public:
    render_context_t m_context;

    void Init(const render_context_t& context);
    void Render() const;
    void GetClientRect(RECT* prc) const;
    
    void GetButtonPos(int index, POINT* ppt) const;
    void Active(int index);

#ifdef TERMINAL_DEBUG
    void DrawClientRect() const;
    void DrawButtonGrid() const;
    void DrawButtonsMarker() const;
#endif // TERMINAL_DEBUG

    void DrawButtons(const TerminalPage& page) const;

};

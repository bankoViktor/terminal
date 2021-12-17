
#include "Terminal.h"
#include "pages.h"


void Terminal::Init(const render_context_t& context)
{
    m_context = context;
}

void Terminal::GetClientRect(RECT* prc) const
{
    prc->left = INNER_OFFSET;
    prc->top = INNER_OFFSET;
    prc->right = m_context.width - INNER_OFFSET;
    prc->bottom = m_context.height - INNER_OFFSET;
}

#ifdef TERMINAL_DEBUG

void Terminal::DrawButtonGrid() const
{
    POINT pt = { 0 };

    for (auto i = 0; i < BUTTON_COUNT_X; ++i)
    {
        GetButtonPos(i, &pt);
        m_context.moveTo(pt.x, 0);
        m_context.lineTo(pt.x, m_context.height, GRID_COLOR);
    }

    for (auto i = BUTTON_COUNT_X; i < (BUTTON_COUNT_X + BUTTON_COUNT_Y); ++i)
    {
        GetButtonPos(i, &pt);
        m_context.moveTo(0, pt.y);
        m_context.lineTo(m_context.width, pt.y, GRID_COLOR);
    }
}

void Terminal::DrawButtonsMarker() const
{
    POINT pt = { 0 };

    for (auto i = 0; i < BUTTON_COUNT; ++i)
    {
        GetButtonPos(i, &pt);
        m_context.moveTo(pt.x - BUTTON_MARKER_SIZE, pt.y);
        m_context.lineTo(pt.x + BUTTON_MARKER_SIZE, pt.y, BUTTON_MARKER_COLOR);
        m_context.moveTo(pt.x, pt.y - BUTTON_MARKER_SIZE);
        m_context.lineTo(pt.x, pt.y + BUTTON_MARKER_SIZE, BUTTON_MARKER_COLOR);
    }
}

void Terminal::DrawClientRect() const
{
    RECT rc = { 0 };
    GetClientRect(&rc);

    m_context.rect(rc.left, rc.top, RECT_WIDTH(rc), RECT_HEIGHT(rc), GRID_COLOR);
}

#endif // TERMINAL_DEBUG

void Terminal::GetButtonPos(int index, POINT* ppt) const
{
    if (index < 0) return;

    RECT rc = { 0 };
    GetClientRect(&rc);

#ifdef BUTTON_OFFSET 
    InflateRect(&rc, -BUTTON_OFFSET, -BUTTON_OFFSET);
#endif

    auto stepX = (double)RECT_WIDTH(rc) / (BUTTON_COUNT_X + 1);
    auto stepY = (double)RECT_HEIGHT(rc) / (BUTTON_COUNT_Y + 1);

    if (index < BUTTONS_TOP)
    {
        // Top
        ppt->x = rc.left + (long)(stepX * (index + 1));
        ppt->y = rc.top;
    }
    else if (index < BUTTONS_RIGHT)
    {
        // Right
        ppt->x = rc.right;
        ppt->y = rc.top + (long)(stepY * (index - BUTTONS_TOP + 1));
    }
    else if (index < BUTTONS_BOTTOM)
    {
        // Bottom
        ppt->x = rc.left + (long)(stepX * (index - BUTTONS_RIGHT + 1));
        ppt->y = rc.bottom;
    }
    else if (index < BUTTONS_LEFT)
    {
        // Left
        ppt->x = rc.left;
        ppt->y = rc.top + (long)(stepY * (index - BUTTONS_BOTTOM + 1));
    }
}

void Terminal::DrawButtons(const TerminalPage& page) const
{
    HorizontalAligment hAlign = HorizontalAligment::Center;
    VerticalAligment vAlign = VerticalAligment::Top;
    auto dx = 0;
    auto dy = 0;

    for (auto i = 0; i < BUTTON_COUNT; ++i)
    {
        auto pButton = &page.m_pButtons[i];

        // Align by Side
        if (i == BUTTONS_TOP)
        {
            // Right
            hAlign = HorizontalAligment::Right;
            vAlign = VerticalAligment::Middle;
        }
        else if (i == BUTTONS_LEFT)
        {
            // Bottom
            hAlign = HorizontalAligment::Center;
            vAlign = VerticalAligment::Bottom;
        }
        else if (i == BUTTONS_BOTTOM)
        {
            // Left
            hAlign = HorizontalAligment::Left;
            vAlign = VerticalAligment::Middle;
        }

        // Calc Text Rect
        RECT rc = { 0 };
        m_context.calcText(&rc, pButton->m_pszTitle, hAlign, vAlign);

        // Offset Text by Side
        if (i < BUTTONS_TOP)
        {
            // Top
            dx = -RECT_WIDTH(rc) / 2;
            dy = 0;
        }
        else if (i < BUTTONS_RIGHT)
        {
            // Right
            dx = -RECT_WIDTH(rc);
            dy = -RECT_HEIGHT(rc) / 2;
        }
        else if (i < BUTTONS_BOTTOM)
        {
            // Bottom
            dx = -RECT_WIDTH(rc) / 2;
            dy = -RECT_HEIGHT(rc);
        }
        else if (i < BUTTONS_LEFT)
        {
            // Left
            dx = 0;
            dy = -RECT_HEIGHT(rc) / 2;
        }

        POINT pt = { 0 };
        GetButtonPos(i, &pt);

        OffsetRect(&rc, pt.x + dx, pt.y + dy);

        // Draw Label
        m_context.text(&rc, pButton->m_pszTitle, TEXT_COLOR, TEXT_BGCOLOR, hAlign, vAlign);
    }
}

void Terminal::Active(int index)
{
    OutputDebugStringA("Active");
}

void Terminal::Render() const
{
#ifdef TERMINAL_DEBUG

    DrawClientRect();
    DrawButtonGrid();
    DrawButtonsMarker();

#endif // TERMINAL_DEBUG

    Page1 page1;
    DrawButtons(page1);
}

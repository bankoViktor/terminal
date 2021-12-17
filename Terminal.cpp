
#include "Terminal.h"


void Terminal::Init(const render_context_t& context)
{
    m_context = context;
}

void Terminal::DrawButtonGrid() const
{
    RECT rc = {
        .left = INNER_OFFSET,
        .top = INNER_OFFSET,
        .right = m_context.width - INNER_OFFSET,
        .bottom = m_context.height - INNER_OFFSET,
    };

    auto stepX = (double) RECT_WIDTH(rc) / (BUTTON_COUNT_X + 1);
    auto stepY = (double) RECT_HEIGHT(rc) / (BUTTON_COUNT_Y + 1);

    for (auto x = INNER_OFFSET + stepX; x < rc.right - stepX / 2; x += stepX)
    {
        m_context.moveTo(x, 0);
        m_context.lineTo(x, m_context.height, GRID_COLOR);
    }

   for (auto y = INNER_OFFSET + stepY; y < rc.bottom - stepY / 2; y += stepY)
    {
        m_context.moveTo(0, y);
        m_context.lineTo(m_context.width, y, GRID_COLOR);
    }
}

void Terminal::GetButtonPos(int index, POINT* ppt) const
{
    //auto stepX = (m_context.width - offset * 2) / buttonsX;
    //auto stepY = (m_context.height - offset * 2) / buttonsY;

    ppt->x = 150 * (index + 1);
    ppt->y = 100;
}

void Terminal::DrawButtons(const TerminalPage& page) const
{
    auto stepX = (m_context.width - offset * 2) / BUTTON_COUNT_X;
    auto stepY = (m_context.height - offset * 2) / BUTTON_COUNT_Y;
    auto x = 0;
    auto y = 0;
    auto offsetX = 0;
    auto offsetY = 0;
    auto flags = NULL;

    for (auto i = 0; i < BUTTON_COUNT; ++i)
    {
        auto button = page.m_buttons[i];

        // Calc
        RECT rc = { 0 };
        m_context.calcText(&rc, button.pszTitle, NULL);

        // Origin & Align & Offsets by Side 
        if (i == 0)
        {
            // top
            x = offset - stepX / 2;
            y = offset + fontOffset;
            flags = DT_TOP | DT_CENTER;
            offsetX = -(rc.right - rc.left) / 2;
            offsetY = 0;
        }
        else if (i == BUTTON_COUNT_X)
        {
            // right
            x = m_context.width - offset - fontOffset;
            y = offset - stepY / 2;
            flags = DT_RIGHT;
            offsetX = -(rc.right - rc.left);
            offsetY = -(rc.bottom - rc.top) / 2;
        }
        else if (i == BUTTON_COUNT_X + BUTTON_COUNT_Y)
        {
            // bottom
            x = m_context.width - offset + stepX / 2;
            y = m_context.height - offset - fontOffset;
            flags = DT_BOTTOM | DT_CENTER;
            offsetX = -(rc.right - rc.left) / 2;
            offsetY = -(rc.bottom - rc.top);
        }
        else if (i == BUTTON_COUNT_X * 2 + BUTTON_COUNT_Y)
        {
            // left
            x = offset + fontOffset;
            y = m_context.height - offset + stepY / 2;
            flags = DT_LEFT;
            offsetX = 0;
            offsetY = -(rc.bottom - rc.top) / 2;
        }

        // Step by Side
        if (i < BUTTON_COUNT_X)                           // top
            x += stepX;
        else if (i < BUTTON_COUNT_X + BUTTON_COUNT_Y)           // right
            y += stepY;
        else if (i < BUTTON_COUNT_X * 2 + BUTTON_COUNT_Y)       // bottom
            x -= stepX;
        else if (i < (BUTTON_COUNT_X + BUTTON_COUNT_Y) * 2)     // left
            y -= stepY;

        // Draw Label
        OffsetRect(&rc, x + offsetX, y + offsetY);
        m_context.text(&rc, button.pszTitle, color, bgColor, flags | DT_NOPREFIX);
    }
}

void Terminal::Active(int)
{

}

void Terminal::Render() const
{
    //DTSUploadPage dts_page;

    auto cx = m_context.width - INNER_OFFSET * 2;
    auto cy = m_context.height - INNER_OFFSET * 2;
    m_context.rect(INNER_OFFSET, INNER_OFFSET, cx, cy, gridColor);

    DrawButtonGrid();

    //DrawButtons(dts_page);

    //dts_page.Render(rc);
}

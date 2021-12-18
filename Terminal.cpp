
#include "terminal.h"
#include "pages.h"


void terminal_t::init(const render_context_t& context)
{
    m_context = context;
}

void terminal_t::getClientRect(rect_t& rc) const
{
    rc = this->m_context.rc;
    rc.inflate(-SAFE_OFFSET);
}

void terminal_t::calcButtonPos(uint8_t index, point_t& pt, int16_t delta) const
{
    rect_t rc = m_context.rc;

    auto stepX = (double)rc.width() / (BUTTON_COUNT_X + 1);
    auto stepY = (double)rc.height() / (BUTTON_COUNT_Y + 1);

    if (index < BUTTONS_TOP)
    {
        // Top
        pt.x = rc.left + (coord_t)(stepX * (index + 1));
        pt.y = rc.top + delta;
    }
    else if (index < BUTTONS_RIGHT)
    {
        // Right
        pt.x = rc.right - 1 - delta;
        pt.y = rc.top + (coord_t)(stepY * (index - BUTTONS_TOP + 1));
    }
    else if (index < BUTTONS_BOTTOM)
    {
        // Bottom
        pt.x = rc.left + (coord_t)(stepX * (BUTTON_COUNT_X - index + BUTTONS_RIGHT));
        pt.y = rc.bottom - 1 - delta;
    }
    else if (index < BUTTONS_LEFT)
    {
        // Left
        pt.x = rc.left + delta;
        pt.y = rc.top + (coord_t)(stepY * (BUTTON_COUNT_Y - index + BUTTONS_BOTTOM));
    }
}

#ifdef TERMINAL_DEBUG

void terminal_t::drawButtonGrid() const
{
    point_t pt;

    for (auto i = 0; i < BUTTON_COUNT_X; ++i)
    {
        calcButtonPos(i, pt);
        m_context.moveTo(pt.x, m_context.rc.top);
        m_context.lineTo(pt.x, m_context.rc.bottom, GRID_COLOR);
    }

    for (auto i = BUTTON_COUNT_X; i < (BUTTON_COUNT_X + BUTTON_COUNT_Y); ++i)
    {
        calcButtonPos(i, pt);
        m_context.moveTo(m_context.rc.left, pt.y);
        m_context.lineTo(m_context.rc.right, pt.y, GRID_COLOR);
    }
}

void terminal_t::drawMarker(const point_t& pt, const color_t& color) const
{
    m_context.moveTo(pt.x - BUTTON_MARKER_SIZE, pt.y);
    m_context.lineTo(pt.x + BUTTON_MARKER_SIZE, pt.y, MARKER_COLOR);
    m_context.moveTo(pt.x, pt.y - BUTTON_MARKER_SIZE);
    m_context.lineTo(pt.x, pt.y + BUTTON_MARKER_SIZE, MARKER_COLOR);
}

void terminal_t::drawClientRect() const
{
    rect_t rc;
    getClientRect(rc);
    rc.inflate(1);
    m_context.rect(rc, CLIENT_RECT_COLOR);
}

#endif // TERMINAL_DEBUG

void terminal_t::active(uint8_t index)
{
}

void terminal_t::render() const
{
#ifdef TERMINAL_DEBUG

    drawButtonGrid();
    drawClientRect();

#endif // TERMINAL_DEBUG

    page1_t page1;

    page1.render(*this);
}

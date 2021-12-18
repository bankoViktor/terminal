
#include "terminal_page.h"

terminal_page_t::terminal_page_t(const button_t* pButtons)
{
    m_pszTitle = nullptr;
    m_pButtons = pButtons;
}

//void terminal_page_t::drawButtons(const terminal_t& terminal) const
//{
//    horizontal_aligment_t hAlign = horizontal_aligment_t::Center;
//    vertical_aligment_t vAlign = vertical_aligment_t::Top;
//    auto dx = 0;
//    auto dy = 0;
//
//    for (auto i = 0; i < BUTTON_COUNT; ++i)
//    {
//        auto pButton = &m_pButtons[i];
//
//        // Align by Side
//        if (i == BUTTONS_TOP)
//        {
//            // Right
//            hAlign = horizontal_aligment_t::Right;
//            vAlign = vertical_aligment_t::Middle;
//        }
//        else if (i == BUTTONS_LEFT)
//        {
//            // Bottom
//            hAlign = horizontal_aligment_t::Center;
//            vAlign = vertical_aligment_t::Bottom;
//        }
//        else if (i == BUTTONS_BOTTOM)
//        {
//            // Left
//            hAlign = horizontal_aligment_t::Left;
//            vAlign = vertical_aligment_t::Middle;
//        }
//
//        // Calc Text Rect
//        rect_t rc;
//        terminal.context.calcText(rc, pButton->m_pszTitle, hAlign, vAlign);
//
//        // Offset Text by Side
//        if (i < BUTTONS_TOP)
//        {
//            // Top
//            dx = -rc.width() / 2;
//            dy = 0;
//        }
//        else if (i < BUTTONS_RIGHT)
//        {
//            // Right
//            dx = -rc.width() + 1;
//            dy = -rc.height() / 2;
//        }
//        else if (i < BUTTONS_BOTTOM)
//        {
//            // Bottom
//            dx = -rc.width() / 2;
//            dy = -rc.height() + 1;
//        }
//        else if (i < BUTTONS_LEFT)
//        {
//            // Left
//            dx = 0;
//            dy = -rc.height() / 2;
//        }
//
//        point_t pt;
//        terminal.calcButtonPos(i, pt, BUTTON_OFFSET);
//
//        rc.offset(pt.x + dx, pt.y + dy);
//
//        // Draw Label
//        terminal.context.text(rc, pButton->m_pszTitle,
//            TEXT_COLOR, TEXT_BGCOLOR, hAlign, vAlign);
//    }
//}

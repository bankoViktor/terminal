#pragma once

#include "page.h"


static const button_t page1_buttons[BUTTON_COUNT] = {
    // top
    { 0 },
    { "A-G" },
    { "STBY" },
    { "A-A" },
    { "TST" },
    // right
    { "SLEW\n5.0" },
    { "OSB 07" },
    { "LATCH\nOFF" },
    { "OSB 09" },
    { 0 },
    // bottom
    { "DCLT" },
    { "MSG" },
    { "CDU" },
    { "MAV" },
    { "TGP" },
    // left
    { "ON&OFF" },
    { "OSB 17" },
    { "LSS\n1688" },
    { "OSB 19" },
    { "INT\nCOLD" },
};

template <typename T>
struct page1_t : page_t<T>
{
    page1_t()
    {
        this->m_pButtons = page1_buttons;
        this->m_pszTitle = "PAGE\nONE";
    }
    virtual void render(const T& context) const
    {
        rect_t rc;
        context.getClientRect(rc);

        // Title
        if (this->m_pszTitle)
        {
            rect_t rcText;
            context.calcText(rcText, this->m_pszTitle,
                horizontal_aligment_t::Center, vertical_aligment_t::Top);
            
            auto dx = rc.left + (rc.width() - rcText.width()) / 2;
            auto dy = rc.top + (coord_t)(rc.height() * .25);
            rcText.offset(dx, dy);
            context.text(rcText, this->m_pszTitle, TEXT_COLOR, TEXT_BGCOLOR,
                horizontal_aligment_t::Center, vertical_aligment_t::Top);
        }

        // Buttons
        this->drawButtons(context);
    }
    virtual void input(uint8_t index) const
    {
    }
};


static const button_t page2_buttons[BUTTON_COUNT] = {
    // top
    { "RTN" },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    // right
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    // bottom
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    // left
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
};

template <typename T>
struct page2_t : page_t<T>
{
    page2_t()
    {
        this->m_pButtons = page2_buttons;
        this->m_pszTitle = "PAGE\nTWO";
    }
    virtual void render(const T& context) const
    {
        rect_t rc;
        context.getClientRect(rc);

        // Title
        if (this->m_pszTitle)
        {
            rect_t rcText;
            context.calcText(rcText, this->m_pszTitle,
                horizontal_aligment_t::Center, vertical_aligment_t::Top);

            auto dx = rc.left + (rc.width() - rcText.width()) / 2;
            auto dy = rc.top + (coord_t)(rc.height() * .25);
            rcText.offset(dx, dy);
            context.text(rcText, this->m_pszTitle, TEXT_COLOR, TEXT_BGCOLOR,
                horizontal_aligment_t::Center, vertical_aligment_t::Top);
        }
    }
    virtual void input(uint8_t index) const
    {
    }
};

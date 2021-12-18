#pragma once

#include "page.h"


#define SET_BUTTON(_type,_title)                 {.type = (_type), .szTitle = (_title) }
#define NO_BUTTON                               SET_BUTTON(button_type_t::None, NULL)


static const button_t page1_buttons[BUTTON_COUNT] = {
    // top
    NO_BUTTON,
    SET_BUTTON(button_type_t::Active, "A-G"),
    SET_BUTTON(button_type_t::NoActive, "STBY"),
    SET_BUTTON(button_type_t::NoActive, "A-A"),
    SET_BUTTON(button_type_t::Action, "TST"),
    // right
    SET_BUTTON(button_type_t::Input, "SLEW\n%4.1f"),
    SET_BUTTON(button_type_t::Active, "OSB 07"),
    SET_BUTTON(button_type_t::Toggle, "LATCH\n%s"),
    SET_BUTTON(button_type_t::Active, "OSB 09"),
    NO_BUTTON,
    // bottom
    SET_BUTTON(button_type_t::NoActive, "DCLT"),
    SET_BUTTON(button_type_t::NoActive, "MSG"),
    SET_BUTTON(button_type_t::NoActive, "CDU"),
    SET_BUTTON(button_type_t::NoActive, "MAV"),
    SET_BUTTON(button_type_t::Active, "TGP"),
    // left
    SET_BUTTON(button_type_t::Goto, "PAGE2"),
    NO_BUTTON,
    SET_BUTTON(button_type_t::Input, "LSS\n%4i"),
    SET_BUTTON(button_type_t::Action, "OSB 19"),
    SET_BUTTON(button_type_t::Action, "INT\nCOLD"),
};

template <typename T>
struct my_terminal_t;

template <typename T>
struct page1_t : page_t<T>
{
    page1_t(const terminal_t<T>& terminal) : page_t<T>(terminal)
    {
        this->buttons = page1_buttons;
        this->szTitle = "PAGE\nONE";
    }
    virtual void render() const
    {
        rect_t rc;

        this->terminal.getClientRect(rc);

        // Title
        if (this->szTitle)
        {
            rect_t rcText;
            this->terminal.context.calcText(rcText, this->szTitle,
                horizontal_aligment_t::Center, vertical_aligment_t::Top);
            
            auto dx = rc.left + (rc.width() - rcText.width()) / 2;
            auto dy = rc.top + (coord_t)(rc.height() * .25);
            rcText.offset(dx, dy);
            this->terminal.context.text(rcText, this->szTitle, TEXT_COLOR, TEXT_BGCOLOR,
                horizontal_aligment_t::Center, vertical_aligment_t::Top);
        }

        //// Buttons
        this->drawButtons();
    }
    virtual void input(uint8_t index) const
    {
        const auto & button = this->buttons[index];

        if (index == 16 && button.type == button_type_t::Goto)
        {
           
        }
    }
};


static const button_t page2_buttons[BUTTON_COUNT] = {
    // top
    SET_BUTTON(button_type_t::Goto, "RTN"),
    NO_BUTTON,
    NO_BUTTON,
    NO_BUTTON,
    NO_BUTTON,
    // right
    NO_BUTTON,
    NO_BUTTON,
    NO_BUTTON,
    NO_BUTTON,
    NO_BUTTON,
    // bottom
    NO_BUTTON,
    NO_BUTTON,
    NO_BUTTON,
    NO_BUTTON,
    NO_BUTTON,
    // left
    NO_BUTTON,
    NO_BUTTON,
    NO_BUTTON,
    NO_BUTTON,
    NO_BUTTON,
};

template <typename T>
struct page2_t : page_t<T>
{
    page2_t(const terminal_t<T>& terminal) : page_t<T>(terminal)
    {
        this->buttons = page2_buttons;
        this->szTitle = "PAGE\nTWO";
    }
    virtual void render() const
    {
        //rect_t rc;
        //context.getClientRect(rc);

        //// Title
        //if (this->m_pszTitle)
        //{
        //    rect_t rcText;
        //    context.calcText(rcText, this->m_pszTitle,
        //        horizontal_aligment_t::Center, vertical_aligment_t::Top);

        //    auto dx = rc.left + (rc.width() - rcText.width()) / 2;
        //    auto dy = rc.top + (coord_t)(rc.height() * .25);
        //    rcText.offset(dx, dy);
        //    context.text(rcText, this->m_pszTitle, TEXT_COLOR, TEXT_BGCOLOR,
        //        horizontal_aligment_t::Center, vertical_aligment_t::Top);
        //}
    }
    virtual void input(uint8_t index) const
    {
    }
};

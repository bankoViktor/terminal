#pragma once

#include "types.h"


using terminal_page_t = struct terminal_page_t
{

public:
    const char*         m_pszTitle;
    const button_t*     m_pButtons;

    terminal_page_t(const button_t* pButtons);
    //void drawButtons(const terminal_t& terminal) const;

    //virtual void render(const terminal_t& terminal) const = 0;
    virtual void input(uint8_t index) const = 0;
};

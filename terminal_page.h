#pragma once

#include "types.h"
#include "terminal.h"

typedef struct terminal_page_t
{

public:
    const char*         m_pszTitle;
    const button_t*     m_pButtons;

    terminal_page_t(const button_t* pButtons);
    void drawButtons(const terminal_t terminal) const;

    virtual void render(const terminal_t& terminal) const = 0;
    virtual void active(const button_t& sender, void* param) const = 0;
} terminal_page_t;

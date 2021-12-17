#pragma once

#include "Config.h"
#include "Types.h"


class TerminalPage
{

public:
    const char* pszTitle;
    button_t m_buttons[BUTTON_COUNT];

    virtual void Render() const = 0;
    virtual void Active(const button_t sender, void* param) const = 0;
};

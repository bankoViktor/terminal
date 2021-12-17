#pragma once

#include "Config.h"
#include "Types.h"


class TerminalPage
{

public:
    const char*         m_pszTitle;
    const button_t*     m_pButtons;

    TerminalPage(const button_t* pButtons);
    virtual void Render() const = 0;
    virtual void Active(const button_t *pSender, void* param) const = 0;
};

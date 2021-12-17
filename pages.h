#pragma once

#include "TerminalPage.h"


class Page1 : public TerminalPage
{
public:
    Page1();
    virtual void Render() const;
    virtual void Active(const button_t* pSender, void* param) const;
};


class Page2 : public TerminalPage
{
public:
    Page2();
    virtual void Render() const;
    virtual void Active(const button_t* pSender, void* param) const;
};

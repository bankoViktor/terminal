#pragma once

#include "TerminalPage.h"

class DTSUploadPage : public TerminalPage
{
public:
    DTSUploadPage();
    virtual void Render() const;
    virtual void Active(const button_t sender, void* param) const;
};

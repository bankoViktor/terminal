#pragma once

#include "terminal.h"
#include "pages.h"


template <typename T>
struct my_terminal_t : terminal_t<T>
{
    page1_t<T> page_home;
    page2_t<T> page_2;

    my_terminal_t() : terminal_t<T>(page_home),
        page_home(*this),
        page_2(*this)
    {
    }
};

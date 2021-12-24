#pragma once

#include "terminal.h"
#include "page_info.h"

#include "page_home.h"
#include "page_input.h"


template <typename T>
void pageSelector(page_info_t<T>& page, uint8_t pageIndex)
{
    switch (pageIndex)
    {
    case 0:
        page = page_home_t<T>()();
        break;
    case 1:
        page = page_input_t<T>()();
        break;
    }
}

template <typename T>
struct my_terminal_t : terminal_t<T>
{
    my_terminal_t() : terminal_t<T>(pageSelector, 0)
    {
    }

    
};

#pragma once

#include "types.h"
#include "page_info.h"


#define NAVIGATOR_STACK_SIZE    5


template <typename T>
struct terminal_t;


template <typename T>
using page_selector_f = void(*)(page_info_t<T>& page, uint8_t pageNum);


template <typename T>
struct navigator_t
{
    int16_t             counter;
    page_info_t<T>      stack[NAVIGATOR_STACK_SIZE];
    page_selector_f<T>  selector;

    navigator_t(page_selector_f<T> selector, uint8_t pageNum)
    {
        this->selector = selector;
        this->counter = -1;
        this->push(pageNum);
    }
    const page_info_t<T>& top() const
    {
        return stack[counter];
    }
    void push(uint8_t pageNum)
    {
        if (counter < NAVIGATOR_STACK_SIZE)
        {
            selector(stack[++counter], pageNum);
        }
    }
    void pop()
    {
        if (counter > 0)
        {
            --counter;
        }
    }
};

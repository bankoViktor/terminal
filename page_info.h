
#pragma once

#include "types.h"


#define SET_BUTTON(_type,_title)                 {.type = (_type), .szTitle = (_title) }
#define NO_BUTTON                               SET_BUTTON(button_type_t::None, 0)


template <typename T>
struct terminal_t;


template <typename T>
struct page_info_t
{
    using render_callback_f = void(*)(terminal_t<T>& terminal, void* pdata);
    using input_callback_f = void(*)(terminal_t<T>& terminal, uint8_t index);

    const button_t*     buttons;
    void*               pdata;

    render_callback_f   render;
    input_callback_f    input;
};

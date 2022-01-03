#pragma once

#include "framework.h"
#include "resource.h"



#define MAX_LOADSTRING              100

#define WINDOW_BG_COLOR             RGB(61, 61, 61)

#define CTL_BUTTON_SIZE             30
#define CTL_BUTTON_ZONE_SIZE        40
#define CTL_BUTTON_BASE_ID          1000

#define RECT_WIDTH(rc)              ((rc).right  - (rc).left)
#define RECT_HEIGHT(rc)             ((rc).bottom - (rc).top)


//using home_page_t = struct home_page_t
//{
//    uint8_t*    caption[16];
//    uint16_t    counter;
//    int16_t     angle;
//};
//
//
//template <typename T>
//void page_Home_render_callback(const terminal_t<T>& terminal, const rect_t* prc, void* pdata)
//{
//    home_page_t& data = *(home_page_t*)pdata;
//
//    if (prc)
//    {
//        // Invalidate All
//    }
//    else
//    {
//        // Invalidate Rect
//    }
//}
//
//template <typename T>
//void page_Home_input_callback(const terminal_t<T>& terminal, uint8_t index)
//{
//    if (index == 1)
//    {
//    }
//}
//
//template <typename T>
//void page_Input_render_callback(const terminal_t<T>& terminal, const rect_t* prc, void* pdata)
//{
//    if (prc)
//    {
//        // Invalidate All
//    }
//    else
//    {
//        // Invalidate Rect
//    }
//}


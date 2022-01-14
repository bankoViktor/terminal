/*
 * File     frame_tab2.c
 * Date     05.01.2022
 */

#include "frame_tab2.h"
#include "bv_frame.h"
#include "bv_config.h"
#include "bv_terminal.h"
#include "bg_primitives.h"
#include "bv_tools.h"

#include "frame_tab1.h"

#include <string.h>
#include <stdio.h>


#define LABEL_BUFFER_LENGTH_MAX            64

#define toggle_in_range(v,b,e)      (v < e ? v + 1 : b)


extern terminal_t g_terminal;


static const button_t g_frame_tab2_buttons[BUTTON_COUNT] = {
    // top - 0
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    // right - 5
    { "MODE\n%s", BT_TOGGLE },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    // bottom - 10
    { "FIVE", BT_SELECTABLE },
    { "FOUR", BT_SELECTABLE },
    { "THREE", BT_SELECTABLE },
    { "TWO", BT_SELECTABLE },
    { "ONE", BT_SELECTABLE },
    // left - 15
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
};


typedef enum frame_tab1_buttons_t
{
    // top
    // right
    BTN_TAB2_MODE = 5,
    // bottom
    BTN_TAB2_FIVE = 10,
    BTN_TAB2_FOUR = 11,
    BTN_TAB2_THREE = 12,
    BTN_TAB2_TWO = 13,
    BTN_TAB2_ONE = 14,
    // left
} frame_tab1_buttons_t;


result_t FrameTab2Proc(
    frame_message_t nMsg,
    param_t param)
{
    switch (nMsg)
    {

    case FM_PAINT:
    {
        for (uint8_t i = 0; i < BUTTON_COUNT; i++)
        {
            const button_t* pButtton = &g_frame_tab2_buttons[i];

            // Marker
            point_t pt = { 0 };
            BVT_CalcButtonPos(&pt, i, 5);
            BVG_DrawButtonMarker(i, &pt, pButtton->type);

            color_t fore = TEXT_COLOR;
            color_t back = TEXT_BGCOLOR;

            // Label
            char szValue[LABEL_BUFFER_LENGTH_MAX + 1] = { 0 };

            switch (i)
            {
            case BTN_TAB2_MODE:
            {
                const char* szMode = g_terminal.data.bMode == 0 ? "OFF" :
                    g_terminal.data.bMode == 1 ? "MANUAL" :
                    g_terminal.data.bMode == 2 ? "SEMIAUTO" :
                    g_terminal.data.bMode == 3 ? "AUTO" : "ERROR";
                sprintf_s(szValue, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle, szMode);
                break;
            }

            case BTN_TAB2_TWO:
                strcpy_s(szValue, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle);
                fore = TEXT_BGCOLOR;
                back = TEXT_COLOR;
                break;

            default:
                if (pButtton->szTitle)
                {
                    strcpy_s(szValue, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle);
                }
                break;

            }

            if (szValue[0])
            {
                BVG_DrawButtonText(i, BUTTON_OFFSET, szValue, fore, back);
            }
        }
        break;
    } //!FM_PAINT

    case FM_NOTIFY:
    {
        /*
        uint8_t buttonIndex = DW2B(param, 0);
        uint8_t notificationCode = DW2B(param, 1);

        switch (notificationCode)
        {

        case BN_UP:
            switch (buttonIndex)
            {

            case BTN_TAB2_MODE:
                g_terminal.data.bMode = toggle_in_range(g_terminal.data.bMode, 0, 4);
                BVT_InvalidateRect(0, 1);
                break;

            case BTN_TAB2_ONE:
                BVT_PopFrame();
                BVT_PushFrame(FrameTab1Proc);
                BVT_InvalidateRect(0, 1);
                break;

            }
            break;

        case BN_DOWN:
            break;

        }
        BVT_InvalidateRect(0, 1);
        */
        break;
    } // !FM_NOTIFICATION

    }

    return _NULL;
}


/* END OF FILE */

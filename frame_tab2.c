/*
 * File     frame_tab2.c
 * Date     05.01.2022
 */

#include "frame_tab2.h"
#include "bv_frame.h"
#include "bv_config.h"
#include "bv_terminal.h"
#include "bg_primitives.h"
#include "frame_tab1.h"

#include <string.h>


#define LABEL_LENGTH_MAX        254


extern terminal_t g_terminal;


static const button_t g_frame_tab2_buttons[BUTTON_COUNT] = {
    // top - 0
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    // right - 5
    { 0 },
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
    // bottom
    BTN_TAB2_FIVE = 10,
    BTN_TAB2_FOUR = 11,
    BTN_TAB2_THREE = 12,
    BTN_TAB2_TWO = 13,
    BTN_TAB2_ONE = 14,
    // left
} frame_tab1_buttons_t;


void FrameTab2Proc(uint16_t nMsg, uint32_t param)
{
    switch (nMsg)
    {

    case FM_PAINT:
    {
        point_t pt = { 0 };

        for (uint8_t i = 0; i < BUTTON_COUNT; i++)
        {
            const button_t* pButtton = &g_frame_tab2_buttons[i];

            // Marker
            BVT_CalcButtonPos(&pt, i, 5);
            BVG_DrawButtonMarker(i, &pt, pButtton->type);

            color_t fore = TEXT_COLOR;
            color_t back = TEXT_BGCOLOR;

            // Label
            char szBuffer[LABEL_LENGTH_MAX + 1] = { 0 };

            switch (i)
            {

            case BTN_TAB2_TWO:
                strcpy_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle);
                fore = TEXT_BGCOLOR;
                back = TEXT_COLOR;
                break;

            default:
                if (pButtton->szTitle)
                {
                    strcpy_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle);
                }
                break;

            }

            if (szBuffer[0])
            {
                BVG_DrawButtonText(i, BUTTON_OFFSET, szBuffer, fore, back);
            }
        }

        break;
    } //!FM_PAINT

    case FM_NOTIFICATION:
    {
        uint8_t buttonIndex = _HIBYTE(param);
        uint8_t notificationCode = _LOBYTE(param);
        switch (notificationCode)
        {

        case BN_UP:
            switch (buttonIndex)
            {

            case BTN_TAB2_ONE:
                BVT_FramePop();
                BVT_FramePush(FrameTab1Proc);
                BVT_InvalidateRect(0, 1);
                break;

            }
            break;

        case BN_DOWN:
            break;

        }
        BVT_InvalidateRect(0, 1);
        break;
    } // !FM_NOTIFICATION

    }
}


/* END OF FILE */

/*
 * File     frame_tab1.c
 * Date     29.12.2021
 */

#include "frame_tab1.h"
#include "bv_frame.h"
#include "bv_config.h"
#include "bv_terminal.h"
#include "bg_primitives.h"
#include "frame_tab2.h"

#include <string.h>
#include <stdio.h>


#define LABEL_LENGTH_MAX        254


extern terminal_t g_terminal;


static const button_t g_frame_tab1_buttons[BUTTON_COUNT] = {
    // top - 0
    { "SEL 1", BT_SELECTABLE},
    { "SEL 2", BT_SELECTABLE},
    { "SEL 3", BT_SELECTABLE},
    { 0, BT_LEFT },
    { 0, BT_RIGHT },
    // right - 5
    { 0, BT_UP },
    { 0, BT_DOWN },
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
    { "T-INPUT\n%s", BT_INPUT },
    { "N-INPUT\n%4i", BT_INPUT },
    { "TOGGLE\n%s", BT_TOGGLE },
};


typedef enum frame_tab1_buttons_t
{
    // top
    BTN_TAB1_SEL1 = 0,
    BTN_TAB1_SEL2 = 1,
    BTN_TAB1_SEL3 = 2,
    BTN_TAB1_LEFT = 3,
    BTN_TAB1_RIGHT = 4,
    // right
    BTN_TAB1_UP = 5,
    BTN_TAB1_DOWN = 6,
    // bottom
    BTN_TAB1_FIVE = 10,
    BTN_TAB1_FOUR = 11,
    BTN_TAB1_THREE = 12,
    BTN_TAB1_TWO = 13,
    BTN_TAB1_ONE = 14,
    // left
    BTN_TAB1_TINPUT = 17,
    BTN_TAB1_NINPUT = 18,
    BTN_TAB1_TOGGLE = 19,
} frame_tab1_buttons_t;


void FrameTab1Proc(uint16_t nMsg, uint32_t param)
{
    switch (nMsg)
    {

    case FM_PAINT:
    {
        point_t pt = { 0 };

        for (uint8_t i = 0; i < BUTTON_COUNT; i++)
        {
            const button_t* pButtton = &g_frame_tab1_buttons[i];

            // Marker
            BVT_CalcButtonPos(&pt, i, 5);
            BVG_DrawButtonMarker(i, &pt, pButtton->type);

            color_t fore = TEXT_COLOR;
            color_t back = TEXT_BGCOLOR;

            // Label
            char szBuffer[LABEL_LENGTH_MAX + 1] = { 0 };

            switch (i)
            {

            case BTN_TAB1_SEL1:
            case BTN_TAB1_SEL2:
            case BTN_TAB1_SEL3:
                strcpy_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle);
                if (g_terminal.nSelectable == (i + 1))
                {
                    fore = TEXT_BGCOLOR;
                    back = TEXT_COLOR;
                }
                break;

            case BTN_TAB1_LEFT:
            case BTN_TAB1_RIGHT:
            case BTN_TAB1_UP:
            case BTN_TAB1_DOWN:
            {
                point_t pt = { 0 };
                BVT_CalcButtonPos(&pt, i, 25);

                coord_t hw = TRIANGLE_SIZE / 2;
                rect_t rc = { 0 };
                RECT_SetWithSize(&rc, pt.x - hw, pt.y - hw, TRIANGLE_SIZE, TRIANGLE_SIZE);
                    
                triangle_orientation_t or =
                    i == BTN_TAB1_LEFT ? TO_LEFT :
                    i == BTN_TAB1_UP ? TO_UP :
                    i == BTN_TAB1_RIGHT ? TO_RIGHT : TO_DOWN;
                BVP_DrawDirectionSymbol(&rc, or, TEXT_COLOR);
            }
                break;

            case BTN_TAB1_ONE:
                strcpy_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle);
                fore = TEXT_BGCOLOR;
                back = TEXT_COLOR;
                break;

            case BTN_TAB1_TINPUT:
                sprintf_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle, g_terminal.szInputText);
                break;

            case BTN_TAB1_NINPUT:
                sprintf_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle, g_terminal.nInputNumber);
                break;

            case BTN_TAB1_TOGGLE:
                sprintf_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle, g_terminal.bBool ? "ON" : "OFF");
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

            case BTN_TAB1_SEL1:
            case BTN_TAB1_SEL2:
            case BTN_TAB1_SEL3:
                g_terminal.nSelectable = buttonIndex + 1;
                break;

            case BTN_TAB1_TWO:
                BVT_FramePop();
                BVT_FramePush(FrameTab2Proc);
                BVT_InvalidateRect(0, 1);
                break;

            case BTN_TAB1_TOGGLE:
                g_terminal.bBool = !g_terminal.bBool;
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

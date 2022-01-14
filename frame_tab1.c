/*
 * File     frame_tab1.c
 * Date     29.12.2021
 */

#include "frame_tab1.h"
#include "bv_frame.h"
#include "bv_config.h"
#include "bv_terminal.h"
#include "bg_primitives.h"
#include "bv_tools.h"

#include "frame_tab2.h"
#include "frame_input.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define LABEL_BUFFER_LENGTH_MAX        254


extern terminal_t g_terminal;


static const button_t buttons[BUTTON_COUNT] = {
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
    { "F-INPUT\n%s", BT_INPUT },
    { "T-INPUT\n%s", BT_INPUT },
    { "N-INPUT\n%i", BT_INPUT },
    { "TOGGLE\n%s", BT_TOGGLE },
};


typedef enum button_index_t
{
    // top
    BI_SEL1 = 0,
    BI_SEL2 = 1,
    BI_SEL3 = 2,
    BI_LEFT = 3,
    BI_RIGHT = 4,
    // right
    BI_UP = 5,
    BI_DOWN = 6,
    // bottom
    BI_FIVE = 10,
    BI_FOUR = 11,
    BI_THREE = 12,
    BI_TWO = 13,
    BI_ONE = 14,
    // left
    BI_FINPUT = 16,
    BI_TINPUT = 17,
    BI_NINPUT = 18,
    BI_TOGGLE = 19,
} button_index_t;


static void OnPaint()
{
    // Buttons
    for (uint8_t nButtonIndex = 0; nButtonIndex < BUTTON_COUNT; nButtonIndex++)
    {
        const button_t* pButtton = &buttons[nButtonIndex];

        color_t fore = TEXT_COLOR;
        color_t back = TEXT_BGCOLOR;

        // Label
        uint8_t szLabel[LABEL_BUFFER_LENGTH_MAX] = { 0 };

        switch (nButtonIndex)
        {

        case BI_SEL1:
        case BI_SEL2:
        case BI_SEL3:
            strcpy_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle);
            if (g_terminal.data.bSelectable == (nButtonIndex + 1))
            {
                fore = TEXT_BGCOLOR;
                back = TEXT_COLOR;
            }
            break;

        case BI_LEFT:
        case BI_RIGHT:
        case BI_UP:
        case BI_DOWN:
        {
            point_t pt = { 0 };
            BVT_CalcButtonPos(&pt, nButtonIndex, 25);

            coord_t hw = TRIANGLE_SIZE / 2;
            rect_t rc = { 0 };
            RECT_SetWithSize(&rc, pt.x - hw, pt.y - hw, TRIANGLE_SIZE, TRIANGLE_SIZE);

            triangle_orientation_t or =
                nButtonIndex == BI_LEFT ? TO_LEFT :
                nButtonIndex == BI_UP ? TO_UP :
                nButtonIndex == BI_RIGHT ? TO_RIGHT : TO_DOWN;
            BVP_DrawDirectionSymbol(&rc, or , TEXT_COLOR);
            break;
        }

        case BI_ONE:
            strcpy_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle);
            fore = TEXT_BGCOLOR;
            back = TEXT_COLOR;
            break;

        case BI_TINPUT:
            sprintf_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle, g_terminal.data.szInputText);
            break;

        case BI_NINPUT:
            sprintf_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle, g_terminal.data.wInputNumber);
            break;

        case BI_TOGGLE:
            sprintf_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle, g_terminal.data.bBool ? "ON" : "OFF");
            break;

        default:
            if (pButtton->szTitle)
                strcpy_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle);
            break;

        }

        if (szLabel[0])
            BVG_DrawButtonText(nButtonIndex, BUTTON_OFFSET, szLabel, fore, back);
    }
}

static result_t OnNotify(notification_header_t* pNMHDR)
{
    result_t result = _NULL;

    switch (pNMHDR->nCode)
    {
        case IN_INIT:
        {
            init_notification_t* pINM = (init_notification_t*)pNMHDR;
            _itoa_s(g_terminal.data.wInputNumber, pINM->szValue, pINM->nLengthMax, 10);
            pINM->nLengthMax = 4;
            break;
        }

        case IN_UPDATE:
        {
            update_notification_t* pUNM = (update_notification_t*)pNMHDR;
            uint16_t nValue = atoi(pUNM->szValue);
            if (nValue < 1000 || nValue > 9999)
            {
                pUNM->szMessage = "The value must be in\nthe range 1000-9999";
            }
            else
            {
                g_terminal.data.wInputNumber = nValue;
                result = _TRUE;
            }
            break;
        }

       //case BI_TINPUT:
       //    
       //    g_terminal.data.wInputNumber =
       //        (uint16_t)atoi(g_terminal.input.szBuffer);
       //    break;
       //
       //case BI_NINPUT:
       //    g_terminal.data.wInputNumber =
       //        (uint16_t)atoi(g_terminal.input.szBuffer);
       //    break;
    }

    return result;
}

static result_t OnButtonUp(uint8_t nButtonIndex)
{
    result_t result = _NULL;

    switch (nButtonIndex)
    {

    case BI_SEL1:
    case BI_SEL2:
    case BI_SEL3:
        g_terminal.data.bSelectable = nButtonIndex + 1;
        break;

    case BI_TWO:
        BVT_PopFrame();
        BVT_PushFrame(FrameTab2Proc);
        break;

    case BI_FINPUT:
        break;

    case BI_NINPUT:
        BVT_PushFrame(FrameInputProc);
        break;

    case BI_TOGGLE:
        g_terminal.data.bBool = !g_terminal.data.bBool;
        break;

    }

    return result;
}


result_t FrameTab1Proc(
    frame_message_t nMsg,
    param_t param)
{
    result_t result = _NULL;

    switch (nMsg)
    {

    case FM_PAINT:
        OnPaint();
        break;

    case FM_NOTIFY:
        result = OnNotify((notification_header_t*)param);
        break;

    case FM_BUTTONUP:
        result = OnButtonUp((uint8_t)param);
        BVT_InvalidateRect(_NULL, _TRUE);
        break;

    }

    return result;
}


/* END OF FILE */

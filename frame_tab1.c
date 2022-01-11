/*
 * File     frame_tab1.c
 * Date     29.12.2021
 */

#include "frame_tab1.h"
#include "bv_frame.h"
#include "bv_config.h"
#include "bv_terminal.h"
#include "bv_hwdriver.h"
#include "bv_primitives.h"
#include "bv_tools.h"
#include "bv_debug.h"

#include "frame_tab2.h"
#include "frame_input.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define LABEL_LENGTH_MAX        63


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
    { "FMT-INPUT\n%s", BT_INPUT },
    { "F-INPUT\n%-4.2f", BT_INPUT },
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
    BI_FRAME_FIVE = 10,
    BI_FRAME_FOUR = 11,
    BI_FRAME_THREE = 12,
    BI_FRAME_TWO = 13,
    BI_FRAME_ONE = 14,
    // left
    BI_FMTINPUT = 15,
    BI_FINPUT = 16,
    BI_TINPUT = 17,
    BI_NINPUT = 18,
    BI_TOGGLE = 19,
} button_index_t;


static void OnPaint()
{
    // Buttons
    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        const button_t* pButtton = &buttons[i];

        // Label
        uint8_t szBuffer[LABEL_LENGTH_MAX + 1] = { 0 };
        color_t fore = TEXT_COLOR;
        color_t back = TEXT_BGCOLOR;
        switch (i)
        {

        case BI_SEL1:
        case BI_SEL2:
        case BI_SEL3:
            strcpy_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle);
            if (g_terminal.data.bSelectable == (i + 1))
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
            BVT_CalcButtonPos(&pt, i, 25);

            coord_t hw = TRIANGLE_SIZE / 2;
            rect_t rc = { 0 };
            RECT_SetWithSize(&rc, pt.x - hw, pt.y - hw, TRIANGLE_SIZE, TRIANGLE_SIZE);

            triangle_orientation_t or =
                i == BI_LEFT ? TO_LEFT :
                i == BI_UP ? TO_UP :
                i == BI_RIGHT ? TO_RIGHT : TO_DOWN;
            BVP_DrawDirectionSymbol(&rc, or , TEXT_COLOR);
            break;
        }

        case BI_FRAME_ONE:
            strcpy_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle);
            fore = TEXT_BGCOLOR;
            back = TEXT_COLOR;
            break;

        case BI_TINPUT:
            //sprintf_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle, g_terminal.data.szInputText);
            break;

        case BI_NINPUT:
            //sprintf_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle, g_terminal.data.wInputNumber);
            break;

        case BI_TOGGLE:
            //sprintf_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle, g_terminal.data.bBool ? "ON" : "OFF");
            break;

        default:
            if (pButtton->szTitle)
                strcpy_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle);
            break;

        } // !switch (i)

        if (szBuffer[0])
            BVG_DrawButtonText(i, BUTTON_OFFSET, szBuffer, fore, back);
    }
}

static result_t OnNotify(
    notification_header_t* pNTF)
{
    result_t result = _NULL;
    switch (pNTF->nCode)
    {

    case IN_INIT:
    {
        init_notification_t* pINTF = (init_notification_t*)pNTF;
        pINTF->nLengthMax = 4;
        sprintf_s(pINTF->szValue, INPUT_BUFFER_LENGTH, "%i", g_terminal.data.wInputNumber);
        break;
    }

    } // !switch (notificationCode)

    return result;
}

static void OnButtonUp(
    uint32_t nButtonIndex)
{
    switch (nButtonIndex)
    {

    case BI_SEL1:
    case BI_SEL2:
    case BI_SEL3:
        g_terminal.data.bSelectable = nButtonIndex + 1;
        break;

    case BI_FRAME_TWO:
        BVT_PopFrame();
        BVT_PushFrame(FrameTab2Proc);
        break;

    case BI_FINPUT:
        break;

    case BI_TINPUT:
        break;

    case BI_NINPUT:
    {
        BVT_PushFrame(FrameInputProc);
        break;
    }

    case BI_TOGGLE:
        g_terminal.data.bBool = !g_terminal.data.bBool;
        break;

    } // !switch (nButtonIndex)
}

result_t FrameTab1Proc(
    frame_message_t nMsg,
    param_t param)
{
    PrintFrameMessage(FrameTab1Proc, nMsg, param);
    
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
        OnButtonUp((uint8_t)param);
        break;

    } // !switch (nMsg)

    return result;
}


/* END OF FILE */

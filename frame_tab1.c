/*
 * File     frame_tab1.c
 * Date     29.12.2021
 */

#include "frame_tab1.h"
#include "bv_frame.h"
#include "bv_config.h"
#include "bv_terminal.h"
#include "bv_primitives.h"
#include "bv_tools.h"
#include "bv_debug.h"

#include "frame_tab2.h"
#include "frame_dsms.h"
#include "frame_input.h"
#include "shared.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define LABEL_BUFFER_LENGTH_MAX        253


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
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
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
            coord_t nOffset = SAFE_OFFSET + BUTTON_LABEL_OFFSET + TRIANGLE_SIZE / 2 + 1;
            BVT_GetButtonPos(&pt, nButtonIndex, nOffset);

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
            BVP_DrawButtonText(nButtonIndex, BUTTON_LABEL_OFFSET, szLabel, fore, back);
    }
}

static result_t OnNotify(notification_header_t* pNMHDR)
{
    result_t result = _NULL;

    switch (pNMHDR->nButtonIndex)
    {

    case BI_FINPUT:
    {

        break;
    }

    case BI_TINPUT:
    {
        switch (pNMHDR->nCode)
        {

        case IN_INIT:
        {
            init_notification_t* pINM = (init_notification_t*)pNMHDR;
            strcpy_s(pINM->szValue, pINM->nLengthMax, g_terminal.data.szInputText);
            pINM->nLengthMax = 16;
            pINM->mode = IFM_TEXT;
            break;
        }

        case IN_UPDATE:
        {
            update_notification_t* pUNM = (update_notification_t*)pNMHDR;
            size_t nLen = strnlen_s(pUNM->szValue, INPUT_BUFFER_LENGTH);
            if (nLen)
            {
                result = _TRUE;
            }
            else
            {
                pUNM->szMessage = "The text length must be\nthe range 1-16";
            }
            break;
        }

        } // !switch (pNMHDR->nCode)
        break;
    }

    case BI_NINPUT:
    {
        switch (pNMHDR->nCode)
        {

        case IN_INIT:
        {
            init_notification_t* pINM = (init_notification_t*)pNMHDR;
            _itoa_s(g_terminal.data.wInputNumber, pINM->szValue, pINM->nLengthMax, 10);
            pINM->nLengthMax = 4;
            pINM->mode = IFM_NUMBER;
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

        } // !switch (pNMHDR->nCode)
        break;
    }

    } // !switch (pNMHDR->nButtonIndex)

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

    case BI_FINPUT:
        break;

    case BI_TINPUT:
        BVT_PushFrame(FrameInputProc, nButtonIndex);
        break;

    case BI_NINPUT:
        BVT_PushFrame(FrameInputProc, nButtonIndex);
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
    PrintFrameMessage(FrameTab1Proc, nMsg, param);

    result_t result = _NULL;
    uint8_t needDef = _FALSE;

    switch (nMsg)
    {

    case FM_PAINT:
        OnPaint();
        needDef = _TRUE;
        break;

    case FM_NOTIFY:
        result = OnNotify((notification_header_t*)param);
        break;

    case FM_BUTTONUP:
        result = OnButtonUp((uint8_t)param);
        needDef = _TRUE;
        break;

    }

    if (needDef)
        result = DefFrameProc(nMsg, param, FrameTab1Proc);

    return result;
}


/* END OF FILE */

/*
 * File     frame_tab1.c
 * Date     29.12.2021
 */

#include "../inc/frame_tab1.h"

#include "../../core/inc/bv_frame.h"
#include "../../core/inc/bv_config.h"
#include "../../core/inc/bv_terminal.h"
#include "../../core/inc/bv_primitives.h"
#include "../../core/inc/bv_tools.h"
#include "../../core/inc/bv_debug.h"
#include "../../user_data.h"

#include "../inc/frame_tab2.h"
#include "../inc/frame_dsms.h"
#include "../inc/frame_input.h"
#include "../inc/frame_shared.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define LABEL_BUFFER_LENGTH_MAX        253


static const button_t g_aButtons[BUTTONS_COUNT] = {
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
    { "D-INPUT\n%4.2f", BT_INPUT },
    { "N-INPUT\n%i", BT_INPUT },
    { "T-INPUT\n%s", BT_INPUT },
    { "TOGGLE\n%s", BT_TOGGLE },
};


// 0 - Top; 5 - Right; 10 - Bottom; 15 - Left
typedef enum button_index_t
{
    BI_SEL1 = 0,
    BI_SEL2 = 1,
    BI_SEL3 = 2,
    BI_LEFT = 3,
    BI_RIGHT = 4,
    BI_UP = 5,
    BI_DOWN = 6,
    BI_DOUBLE_INPUT = 16,
    BI_INTEGER_INPUT = 17,
    BI_TEXT_INPUT = 18,
    BI_TOGGLE = 19,
} button_index_t;


static void OnPaint()
{
    // Buttons
    for (uint8_t nButtonIndex = 0; nButtonIndex < BUTTONS_COUNT; nButtonIndex++)
    {
        const button_t* pButtton = &g_aButtons[nButtonIndex];

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
            if (g_UserData.bSelectable == (nButtonIndex + 1))
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

            if (nButtonIndex == BI_UP)
            {
                uint8_t szBuf[100] = {0};
                snprintf(szBuf, 99, "VALUE\n%i", g_UserData.nValue);
                BVP_DrawButtonText(nButtonIndex, 0.5, BUTTON_LABEL_OFFSET, szBuf, fore, back);
            }

            break;
        }

        case BI_DOUBLE_INPUT:
            sprintf_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle, g_UserData.dInputDouble);
            break;

        case BI_INTEGER_INPUT:
            sprintf_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle, g_UserData.wInputNumber);
            break;

        case BI_TEXT_INPUT:
            sprintf_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle, g_UserData.szTestTextValue);
            break;

        case BI_TOGGLE:
            sprintf_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle, g_UserData.bBool ? "ON" : "OFF");
            break;

        default:
            if (pButtton->szTitle)
                strcpy_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle);
            break;

        }

        if (szLabel[0])
            BVP_DrawButtonText(nButtonIndex, 0, BUTTON_LABEL_OFFSET, szLabel, fore, back);
    }
}

static result_t OnNotify(notification_header_t* pNHDR)
{
    result_t result = _NULL;

    switch (pNHDR->nButtonIndex)
    {

    case BI_DOUBLE_INPUT:
    {
        switch (pNHDR->nCode)
        {

        case IN_INIT:
        {
            init_notification_t* pIN = (init_notification_t*)pNHDR;
            snprintf(pIN->szBuffer, pIN->nLengthMax, "%04.2f", g_UserData.dInputDouble);
            pIN->nLengthMax = 6;
            pIN->bmMode = IFM_TYPE_DOUBLE;
            pIN->pszKeysMap = "0123456789";
            pIN->nFromButtonIndex = pNHDR->nButtonIndex;
            break;
        }

        case IN_UPDATE:
        {
            update_notification_t* pUN = (update_notification_t*)pNHDR;
            double dValue = atof(pUN->szBuffer);
            if (dValue >= 0.5f && dValue <= 2.4f)
            {
                g_UserData.dInputDouble = dValue;
                result = _TRUE;
            }
            else
                pUN->szMessage = "The value must be in\nthe range 0.5 - 2.4";
            break;
        }

        } // !switch (pNMHDR->nCode)
        break;
    }

    case BI_TEXT_INPUT:
    {
        switch (pNHDR->nCode)
        {

        case IN_INIT:
        {
            init_notification_t* pIN = (init_notification_t*)pNHDR;
            strcpy_s(pIN->szBuffer, pIN->nLengthMax, g_UserData.szTestTextValue);
            pIN->nLengthMax = TEST_TEXT_LENGHT;
            pIN->bmMode = IFM_TYPE_TEXT;
            pIN->pszKeysMap = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
            pIN->nFromButtonIndex = pNHDR->nButtonIndex;
            break;
        }

        case IN_UPDATE:
        {
            update_notification_t* pUN = (update_notification_t*)pNHDR;
            size_t nLen = strnlen_s(pUN->szBuffer, INPUT_BUFFER_LENGTH_MAX);
            if (nLen)
            {
                strcpy_s(g_UserData.szTestTextValue, TEST_TEXT_LENGHT, pUN->szBuffer);
                result = _TRUE;
            }
            else
                pUN->szMessage = "The text length must be\nthe range 1 - 16";
            break;
        }

        } // !switch (pNMHDR->nCode)
        break;
    }

    case BI_INTEGER_INPUT:
    {
        switch (pNHDR->nCode)
        {

        case IN_INIT:
        {
            init_notification_t* pIN = (init_notification_t*)pNHDR;
            _itoa_s(g_UserData.wInputNumber, pIN->szBuffer, pIN->nLengthMax, 10);
            pIN->nLengthMax = 4;
            pIN->bmMode = IFM_TYPE_INTEGER;
            pIN->pszKeysMap = "0123456789";
            pIN->nFromButtonIndex = pNHDR->nButtonIndex;
            break;
        }

        case IN_UPDATE:
        {
            update_notification_t* pUN = (update_notification_t*)pNHDR;
            uint16_t nValue = atoi(pUN->szBuffer);
            if (nValue >= 1000 && nValue < 9999)
            {
                g_UserData.wInputNumber = nValue;
                result = _TRUE;
            }
            else
                pUN->szMessage = "The value must be in\nthe range 1000 - 9999";
            break;
        }

        } // !switch (pNHDR->nCode)
        break;
    }

    } // !switch (pNHDR->nButtonIndex)

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
        g_UserData.bSelectable = nButtonIndex + 1;
        break;

    case BI_UP:
        TOGGLE_IN_RANGE(g_UserData.nValue, 0, 100, 25);
        break;

    case BI_DOWN:
        TOGGLE_IN_RANGE(g_UserData.nValue, 0, 100, -25);
        break;

    case BI_DOUBLE_INPUT:
    case BI_TEXT_INPUT:
    case BI_INTEGER_INPUT:
        BVT_PushFrame(FrameInputProc, nButtonIndex);
        break;

    case BI_TOGGLE:
        TOGGLE_IN_RANGE_LOOP(g_UserData.bBool, 0, 1);
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
    {
        uint8_t nButtonIndex = (uint8_t)param;
        result = OnButtonUp(nButtonIndex);
        needDef = _TRUE;
        break;
    }

    }

    if (needDef)
        result = BV_DefFrameProc(nMsg, param, FrameTab1Proc);

    return result;
}


/* END OF FILE */

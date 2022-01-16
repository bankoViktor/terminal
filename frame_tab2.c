/*
 * File     frame_tab2.c
 * Date     05.01.2022
 */

#include "frame_tab2.h"
#include "bv_frame.h"
#include "bv_config.h"
#include "bv_terminal.h"
#include "bv_primitives.h"
#include "bv_tools.h"
#include "bv_debug.h"

#include "frame_tab1.h"

#include <string.h>
#include <stdio.h>


#define LABEL_BUFFER_LENGTH_MAX            64



extern terminal_t g_terminal;


static const button_t buttons[BUTTON_COUNT] = {
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


typedef enum buttons_index_t
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
} buttons_index_t;


static void OnPaint()
{
    for (uint8_t nButtonIndex = 0; nButtonIndex < BUTTON_COUNT; nButtonIndex++)
    {
        const button_t* pButtton = &buttons[nButtonIndex];

        // Marker
        point_t pt = { 0 };
        BVT_GetButtonPos(&pt, nButtonIndex, 5);
        BVP_DrawButtonMarker(nButtonIndex, &pt, pButtton->type);

        color_t fore = TEXT_COLOR;
        color_t back = TEXT_BGCOLOR;

        // Label
        uint8_t szLabel[LABEL_BUFFER_LENGTH_MAX] = { 0 };

        switch (nButtonIndex)
        {

        case BTN_TAB2_MODE:
        {
            const uint8_t* szMode = g_terminal.data.bMode == 0 ? "OFF" :
                g_terminal.data.bMode == 1 ? "MANUAL" :
                g_terminal.data.bMode == 2 ? "SEMIAUTO" :
                g_terminal.data.bMode == 3 ? "AUTO" : "ERROR";
            sprintf_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle, szMode);
            break;
        }

        case BTN_TAB2_TWO:
            strcpy_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle);
            fore = TEXT_BGCOLOR;
            back = TEXT_COLOR;
            break;

        default:
            if (pButtton->szTitle)
            {
                strcpy_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle);
            }
            break;

        }

        if (szLabel[0])
        {
            BVP_DrawButtonText(nButtonIndex, BUTTON_LABEL_OFFSET, szLabel, fore, back);
        }
    }
}

static result_t OnNotify(notification_header_t* pNMHDR)
{
    result_t result = _NULL;

    switch (pNMHDR->nCode)
    {

    }

    //BVT_InvalidateRect(0, 1);

    return result;
}

static void OnButtonUp(uint8_t nButtonIndex)
{
    switch (nButtonIndex)
    {

    case BTN_TAB2_MODE:
        g_terminal.data.bMode = TOGGLE_IN_RANGE(g_terminal.data.bMode, 0, 4);
        break;

    case BTN_TAB2_ONE:
        BVT_PopFrame();
        BVT_PushFrame(FrameTab1Proc, _NULL);
        break;
    }

    BVT_InvalidateRect(_NULL, _TRUE);
}


result_t FrameTab2Proc(
    frame_message_t nMsg,
    param_t param)
{
    PrintFrameMessage(FrameTab2Proc, nMsg, param);

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

    }

    return result;
}


/* END OF FILE */

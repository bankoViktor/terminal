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


#define LABEL_LENGTH_MAX            63
#define toggle_in_range(v,b,e)      (v < e ? v + 1 : b)


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


typedef enum button_index_t
{
    // top
    // right
    BTN_MODE = 5,
    // bottom
    BTN_FIVE = 10,
    BTN_FOUR = 11,
    BTN_THREE = 12,
    BTN_TWO = 13,
    BTN_ONE = 14,
    // left
} button_index_t;


static void OnPaint()
{
    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        const button_t* pButtton = &buttons[i];

        // Marker
        point_t pt = { 0 };
        BVT_CalcButtonPos(&pt, i, 5);
        BVG_DrawButtonMarker(i, &pt, pButtton->nType);

        color_t fore = TEXT_COLOR;
        color_t back = TEXT_BGCOLOR;

        // Label
        char szBuffer[LABEL_LENGTH_MAX + 1] = { 0 };

        switch (i)
        {
        case BTN_MODE:
        {
           /* const char* szMode = g_terminal.data.bMode == 0 ? "OFF" :
                g_terminal.data.bMode == 1 ? "MANUAL" :
                g_terminal.data.bMode == 2 ? "SEMIAUTO" :
                g_terminal.data.bMode == 3 ? "AUTO" : "ERROR";
            sprintf_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle, szMode);
            break;*/
        }

        case BTN_TWO:
            strcpy_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle);
            fore = TEXT_BGCOLOR;
            back = TEXT_COLOR;
            break;

        default:
            if (pButtton->szTitle)
                strcpy_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle);
            break;

        }

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

    }

    //BVT_InvalidateRect(0, 1);

    return result;
}

static void OnButtonUp(
    uint8_t nButtonIndex)
{

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
    }

    return result;
}


/* END OF FILE */

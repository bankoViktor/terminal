/*
 * File     frame_tab2.c
 * Date     05.01.2022
 */

#include "../inc/frame_tab2.h"

#include "../../core/inc/bv_frame.h"
#include "../../core/inc/bv_config.h"
#include "../../core/inc/bv_terminal.h"
#include "../../core/inc/bv_primitives.h"
#include "../../core/inc/bv_tools.h"
#include "../../core/inc/bv_debug.h"
#include "../../user_data.h"

#include "../inc/frame_tab1.h"
#include "../inc/frame_dsms.h"
#include "../inc/frame_shared.h"

#include <string.h>
#include <stdio.h>


#define LABEL_BUFFER_LENGTH_MAX            64



extern terminal_t g_terminal;


static const button_t g_aButtons[BUTTONS_COUNT] = {
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
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
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
    BI_MODE = 5,
    // bottom
    // left
} buttons_index_t;


static void OnPaint()
{
    for (uint8_t nButtonIndex = 0; nButtonIndex < BUTTONS_COUNT; nButtonIndex++)
    {
        const button_t* pButtton = &g_aButtons[nButtonIndex];

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

        case BI_MODE:
        {
            const uint8_t* szMode = g_UserData.bMode == 0 ? "OFF" :
                g_UserData.bMode == 1 ? "MANUAL" :
                g_UserData.bMode == 2 ? "SEMIAUTO" :
                g_UserData.bMode == 3 ? "AUTO" : "ERROR";
            sprintf_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle, szMode);
            break;
        }

        default:
            if (pButtton->szTitle)
                strcpy_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle);
            break;

        }

        if (szLabel[0])
            BVP_DrawButtonText(nButtonIndex, 0, BUTTON_LABEL_OFFSET, szLabel, fore, back);
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

    case BI_MODE:
        TOGGLE_IN_RANGE_LOOP(g_UserData.bMode, 0, 4);
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
        OnButtonUp((uint8_t)param);
        needDef = _TRUE;
        break;

    }

    if (needDef)
        result = BV_DefFrameProc(nMsg, param, FrameTab2Proc);

    return result;
}


/* END OF FILE */

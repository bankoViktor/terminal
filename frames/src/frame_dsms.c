/*
 * File     frame_dsms.c
 * Date     19.01.2022
 */

#include "../inc/frame_dsms.h"

#include "../../core/inc/bv_frame.h"
#include "../../core/inc/bv_config.h"
#include "../../core/inc/bv_terminal.h"
#include "../../core/inc/bv_primitives.h"
#include "../../core/inc/bv_tools.h"
#include "../../core/inc/bv_debug.h"

#include "../inc/frame_tab1.h"
#include "../inc/frame_tab2.h"
#include "../inc/frame_shared.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define LABEL_BUFFER_LENGTH_MAX        253


extern terminal_t g_terminal;


static const button_t g_aButtons[BUTTONS_COUNT] = {
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


typedef enum button_index_t
{
    // top
    // right
    // bottom
    // left
} button_index_t;


static void OnPaint()
{
    rect_t rc = { 0 };
    BVT_GetClientRect(&rc);
    RECT_Inflate(&rc, -20, -20);
    BVP_DrawDSMS(&rc);

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

        default:
            if (pButtton->szTitle)
                strcpy_s(szLabel, LABEL_BUFFER_LENGTH_MAX, pButtton->szTitle);
            break;

        }

        if (szLabel[0])
            BVP_DrawButtonText(nButtonIndex, 0, BUTTON_LABEL_OFFSET, szLabel, fore, back);
    }
}

static result_t OnButtonUp(uint8_t nButtonIndex)
{
    result_t result = _NULL;

    switch (nButtonIndex)
    {

    }

    return result;
}


result_t FrameDSMSProc(
    frame_message_t nMsg,
    param_t param)
{
    PrintFrameMessage(FrameDSMSProc, nMsg, param);

    result_t result = _NULL;
    uint8_t needDef = _FALSE;

    switch (nMsg)
    {

    case FM_PAINT:
        OnPaint();
        needDef = _TRUE;
        break;

    case FM_BUTTONUP:
        result = OnButtonUp((uint8_t)param);
        needDef = _TRUE;
        break;

    }

    if (needDef)
        result = BV_DefFrameProc(nMsg, param, FrameDSMSProc);

    return result;
}


/* END OF FILE */

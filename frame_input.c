/*
 * File     frame_input.c
 * Date     05.01.2022
 */

#include "frame_input.h"
#include "bv_terminal.h"
#include "bv_primitives.h"
#include "bv_hwdriver.h"
#include "bv_tools.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


#define LABEL_LENGTH_MAX            64


uint8_t     szValue[INPUT_BUFFER_LENGTH];
uint32_t    nLengthMax;
uint32_t    nCursorPos;


static const button_t buttons[BUTTON_COUNT] = {
    // top - 0
    { "RTN", BT_ACTION },
    { 0 },
    { "CLR", BT_ACTION },
    { "BCK\nSPC", BT_ACTION },
    { "ETR", BT_ACTION },
    // right - 5
    { "6", BT_ACTION },
    { "7", BT_ACTION },
    { "8", BT_ACTION },
    { "9", BT_ACTION },
    { "0", BT_ACTION },
    // bottom - 10
    { 0 },
    { 0 },
    { 0 }, //{ "DOT", BT_ACTION },
    { 0 },
    { 0 },
    // left - 15
    { "5", BT_ACTION },
    { "4", BT_ACTION },
    { "3", BT_ACTION },
    { "2", BT_ACTION },
    { "1", BT_ACTION },
};


typedef enum button_index_t
{
    // top - 0
    BI_RETURN = 0,
    BI_CLEAR = 2,
    BI_BACKSPACE = 3,
    BI_ENTER = 4,
    // right - 5
    BI_6 = 5,
    BI_7 = 6,
    BI_8 = 7,
    BI_9 = 8,
    BI_0 = 9,
    // bottom - 10
    BI_RIGHT = 10,
    BI_LEFT = 11,
    BI_DOT = 12,
    // left - 15
    BI_5 = 15,
    BI_4 = 16,
    BI_3 = 17,
    BI_2 = 18,
    BI_1 = 19,
} button_index_t;


static void MoveCursor(int8_t nDeltaPos)
{
    if (nDeltaPos > 0)
    {
        nCursorPos = nCursorPos < strlen(szValue)
            ? min(nCursorPos + 1, nLengthMax - 1)
            : nCursorPos;
    }
    else if (nDeltaPos < 0)
    {
        nCursorPos = nCursorPos > 0
            ? nCursorPos - 1
            : 0;
    }
}


static void OnCreate()
{
    // Init
    szValue[0] = '\0';
    nLengthMax = 0;
    nCursorPos = 0;

    // Notification
    frame_proc_f proc = BVT_GetPreviousFrame();
    init_notification_t ntf = { 0 };
    ntf.hdr.fromeProc = FrameInputProc;
    ntf.hdr.nCode = IN_INIT;
    ntf.szValue = szValue;
    ntf.nLengthMax = INPUT_BUFFER_LENGTH;
    _SendMsgNotification(proc, &ntf);

    // Set init values
    nLengthMax = ntf.nLengthMax;
}

static void OnPaint()
{
    // Buffer
    BVP_DrawInput(szValue, nLengthMax, nCursorPos);

    // Button
    for (uint8_t nButtonIndex = 0; nButtonIndex < BUTTON_COUNT; nButtonIndex++)
    {
        const button_t* pButtton = &buttons[nButtonIndex];

        // Marker
        point_t pt = { 0 };
        BVT_GetButtonPos(&pt, nButtonIndex, 5);
        BVP_DrawButtonMarker(nButtonIndex, &pt, pButtton->type);

        // Label
        uint8_t szLabel[LABEL_LENGTH_MAX + 1] = { 0 };

        switch (nButtonIndex)
        {

        case BI_RIGHT:
        case BI_LEFT:
        {
            point_t pt = { 0 };
            coord_t nOffset = SAFE_OFFSET + BUTTON_LABEL_OFFSET + TRIANGLE_SIZE / 2 + 1;
            BVT_GetButtonPos(&pt, nButtonIndex, SAFE_OFFSET + nOffset);

            coord_t hw = TRIANGLE_SIZE / 2;

            rect_t rc = { 0 };
            RECT_SetWithSize(&rc, pt.x - hw, pt.y - hw, TRIANGLE_SIZE, TRIANGLE_SIZE);

            triangle_orientation_t or = nButtonIndex == BI_RIGHT ? TO_RIGHT : TO_LEFT;
            BVP_DrawDirectionSymbol(&rc, or , TEXT_COLOR);
            break;
        }

        default:
            if (pButtton->szTitle)
                strcpy_s(szLabel, LABEL_LENGTH_MAX, pButtton->szTitle);
            break;

        }

        if (szLabel[0])
            BVP_DrawButtonText(nButtonIndex, BUTTON_LABEL_OFFSET, szLabel, TEXT_COLOR, TEXT_BGCOLOR);
    }
}

static result_t OnButtonUp(uint8_t nButtonIndex)
{
    result_t result = _NULL;

    switch (nButtonIndex)
    {

    case BI_RETURN:
        BVT_PopFrame();
        break;

    case BI_CLEAR:
        szValue[0] = '\0';
        nCursorPos = 0;
        break;

    case BI_BACKSPACE:
        if (szValue[nCursorPos] == '\0')
            MoveCursor(-1);
        szValue[nCursorPos] = '\0';
        break;

    case BI_ENTER:
    {
        frame_proc_f proc = BVT_GetPreviousFrame();
        update_notification_t ntf = { 0 };
        ntf.hdr.fromeProc = FrameInputProc;
        ntf.hdr.nCode = IN_UPDATE;
        ntf.szValue = szValue;
        ntf.szMessage = _NULL;
        result_t ret = _SendMsgNotification(proc, &ntf);
        if (ret)
        {
            // Valid
            BVT_PopFrame();
        }
        else
        {
            // Invalid
            BVP_DrawErrorMessage(ntf.szMessage);
            return result;
        }
        break;
    }

    case BI_RIGHT:
        MoveCursor(+1);
        break;

    case BI_LEFT:
        MoveCursor(-1);
        break;

    case BI_DOT:
        //g_terminal.input.szBuffer[g_terminal.input.nCursorPos] = '.';
        //MoveCursor(+1);
        break;

    default:
    {
        uint8_t num =
            nButtonIndex == BI_0 ? 0 :
            nButtonIndex < BI_5 ? nButtonIndex + 1 : 20 - nButtonIndex;
        uint8_t next = isdigit(szValue[nCursorPos]);
        szValue[nCursorPos] = 0x30 + num;
        if (!next)
            szValue[nCursorPos + 1] = '\0';
        MoveCursor(+1);
        break;
    }

    }

    BVT_InvalidateRect(_NULL, _TRUE);

    return result;
}


result_t FrameInputProc(
    frame_message_t nMsg,
    param_t param)
{
    result_t result = _NULL;

    switch (nMsg)
    {

    case FM_CREATE:
        OnCreate();
        break;

    case FM_DESTROY:
        break;

    case FM_PAINT:
        OnPaint();
        break;

    case FM_BUTTONUP:
        OnButtonUp((uint8_t)param);
        break;

    }

    return result;
}


/* END OF FILE */

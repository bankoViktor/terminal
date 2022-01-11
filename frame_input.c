/*
 * File     frame_input.c
 * Date     05.01.2022
 */

#include "frame_input.h"
#include "bv_terminal.h"
#include "bv_primitives.h"
#include "bv_hwdriver.h"
#include "bv_tools.h"
#include "bv_debug.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


#define LABEL_LENGTH_MAX            64


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


static uint8_t          szValue[INPUT_BUFFER_LENGTH + 1] = { 0 };
static uint16_t         nCursorPos;
static uint16_t         nLengthMax;
static uint8_t          bParamNumner;
static const uint8_t*   pszFormat = _NULL;


static void MoveCursor(
    int8_t deltaPos)
{
    size_t nLength = strlen(szValue);

    if (deltaPos > 0)
        nCursorPos = nCursorPos < nLength ? min(nCursorPos + 1, nLengthMax - 1) : nCursorPos;
    else if (deltaPos < 0)
        nCursorPos = nCursorPos > 0 ? nCursorPos - 1 : 0;
}

static void OnCreate()
{
    frame_proc_f proc = BVT_GetPreviousFrame();

    init_notification_t ntf;
    ntf.hdr.fFromProc = FrameInputProc;
    ntf.hdr.nCode = IN_INIT;
    ntf.szValue = szValue;

    _SendMsgNotify(proc, &ntf);

    nLengthMax = ntf.nLengthMax;
    nCursorPos = 0;
}

static void OnPaint()
{
    // Buttons
    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        const button_t* pButtton = &buttons[i];

        // Marker
        //point_t pt = { 0 };
        //BVT_CalcButtonPos(&pt, i, 5);
        //BVG_DrawButtonMarker(i, &pt, pButtton->type);

        // Label
        uint8_t szBuffer[LABEL_LENGTH_MAX + 1] = { 0 };
        switch (i)
        {

        case BI_RIGHT:
        case BI_LEFT:
        {
            point_t pt = { 0 };
            BVT_CalcButtonPos(&pt, i, 25);

            coord_t hw = TRIANGLE_SIZE / 2;
            rect_t rc = { 0 };
            RECT_SetWithSize(&rc, pt.x - hw, pt.y - hw, TRIANGLE_SIZE, TRIANGLE_SIZE);

            triangle_orientation_t or = i == BI_RIGHT ? TO_RIGHT : TO_LEFT;
            BVP_DrawDirectionSymbol(&rc, or , TEXT_COLOR);
            break;
        }

        default:
            if (pButtton->szTitle)
                strcpy_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle);
            break;

        } // !switch (i)

        if (szBuffer[0])
            BVG_DrawButtonText(i, BUTTON_OFFSET, szBuffer, TEXT_COLOR, TEXT_BGCOLOR);
    }

    // Buffer
    BVP_DrawInput(szValue, nLengthMax, nCursorPos);
}

static result_t OnNotify(
    notification_header_t* pNTF)
{
    result_t result = _NULL;
    switch (pNTF->nCode)
    {

    } // !switch (notificationCode)

    return result;
}

static void OnButtonUp(
    uint8_t nButtonIndex)
{
    switch (nButtonIndex)
    {

    case BI_RETURN:
        BVT_PopFrame();
        break;

    case BI_CLEAR:
        memset(szValue, _NULL, INPUT_BUFFER_LENGTH + 1);
        nCursorPos = 0;
        break;

    case BI_BACKSPACE:
        if (szValue[nCursorPos] == '\0')
            MoveCursor(-1);
        szValue[nCursorPos] = '\0';
        break;

    case BI_ENTER:
    {
        //uint32_t    dwValue = atoi(szValue);
        //uint8_t     bInvalid = _FALSE;

        uint8_t     szMsgBuffer[MESSAGE_BUFFER_SIZE + 1] = { 0 };
        //uint8_t*    szMsgFmt = _NULL;
        //uint32_t    dwMsgValue = 0;

        //if (dwValue < g_terminal.input.dwMin)
        //{
        //    szMsgFmt = "Invalid value\nmust be greater %i";
        //    dwMsgValue = g_terminal.input.dwMin;
        //    bInvalid = _TRUE;
        //}
        //else if (dwValue > g_terminal.input.dwLengthMax)
        //{
        //    szMsgFmt = "Invalid value\nmust be less %i";
        //    dwMsgValue = g_terminal.input.dwLengthMax;
        //    bInvalid = _TRUE;
        //}

        //if (bInvalid)
        //{
        //    sprintf_s(szMsgBuffer, MESSAGE_BUFFER_SIZE, szMsgFmt, dwMsgValue);
        //    BVP_DrawErrorMessage(szMsgBuffer);
        //}
        //else
        //{
        //    frame_proc_f proc = BVT_GetPreviousFrame();
        //    _SendMsgNotification(proc, g_terminal.input.bParamNumner, IN_UPDATE);
        //    BVT_PopFrame();
        //}

        frame_proc_f proc = BVT_GetPreviousFrame();
       /* update_notify_t notify = { 0 };
        notify.notify.dwButtonIndex = bParamNumner;
        notify.notify.nCode = IN_UPDATE;
        notify.szValue = szValue;
        notify.szMessage = szMsgBuffer;
        result_t ret = _SendMsgNotify(proc, &notify);
        if (!ret)
        {

        }*/
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

    } // !switch (nButtonIndex)

    _SendMsgPaint(FrameInputProc);
}


result_t FrameInputProc(
    frame_message_t nMsg,
    param_t param)
{
    PrintFrameMessage(FrameInputProc, nMsg, param);

    result_t result = _NULL;

    switch (nMsg)
    {

    case FM_CREATE:
        OnCreate();
        break;

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

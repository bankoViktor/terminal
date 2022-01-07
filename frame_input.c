/*
 * File     frame_input.c
 * Date     05.01.2022
 */

#include "frame_input.h"
#include "bv_terminal.h"
#include "bg_primitives.h"
#include "bv_hwdriver.h"
#include "bv_tools.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


#define LABEL_LENGTH_MAX            64


extern terminal_t g_terminal;


static const button_t g_frame_input_buttons[BUTTON_COUNT] = {
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


void MoveCursor(
    int8_t deltaPos)
{
    uint16_t* pnCursorPos = &g_terminal.input.wCursorPos;
    uint16_t* pnLengthMax = &g_terminal.input.wLengthMax;
    size_t nLength = strlen(g_terminal.input.szBuffer);

    if (deltaPos > 0)
    {
        *pnCursorPos = *pnCursorPos < nLength ? min(*pnCursorPos + 1, *pnLengthMax - 1) : *pnCursorPos;
    }
    else if (deltaPos < 0)
    {
        *pnCursorPos = *pnCursorPos > 0 ? *pnCursorPos - 1 : 0;
    }
}


result_t FrameInputProc(
    frame_message_t nMsg,
    param_t param)
{
    switch (nMsg)
    {

    case FM_PAINT:
    {
        // Buffer
        BVP_DrawInput(
            g_terminal.input.szBuffer,
            g_terminal.input.wLengthMax,
            g_terminal.input.wCursorPos
        );

        // Button
        for (uint8_t i = 0; i < BUTTON_COUNT; i++)
        {
            const button_t* pButtton = &g_frame_input_buttons[i];

            // Marker
            point_t pt = { 0 };
            BVT_CalcButtonPos(&pt, i, 5);
            BVG_DrawButtonMarker(i, &pt, pButtton->type);

            // Label
            char szBuffer[LABEL_LENGTH_MAX + 1] = { 0 };

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
            }
                break;

            default:
                if (pButtton->szTitle)
                    strcpy_s(szBuffer, LABEL_LENGTH_MAX, pButtton->szTitle);
                break;

            }

            if (szBuffer[0])
                BVG_DrawButtonText(i, BUTTON_OFFSET, szBuffer, TEXT_COLOR, TEXT_BGCOLOR);
        }
        break;
    } // !FM_PAINT

    case FM_NOTIFICATION:
    {
        button_index_t buttonIndex = DW2B(param, 0);
        notification_code_t notificationCode = DW2B(param, 1);
        switch (notificationCode)
        {
        case BN_UP:
            switch (buttonIndex)
            {
            case BI_RETURN:
                BVT_PopFrame();
                break;

            case BI_CLEAR:
                memset(g_terminal.input.szBuffer, _NULL, INPUT_BUFFER_LENGTH + 1);
                g_terminal.input.wCursorPos = 0;
                break;

            case BI_BACKSPACE:
                if (g_terminal.input.szBuffer[g_terminal.input.wCursorPos] == '\0')
                    MoveCursor(-1);
                g_terminal.input.szBuffer[g_terminal.input.wCursorPos] = '\0';
                break;

            case BI_ENTER:
            {
                uint32_t    dwValue = atoi(g_terminal.input.szBuffer);
                uint8_t     bInvalid = _FALSE;

                uint8_t     szMsgBuffer[MESSAGE_BUFFER_SIZE + 1] = { 0 };
                uint8_t*    szMsgFmt = _NULL;
                uint32_t    dwMsgValue = 0;

                if (dwValue < g_terminal.input.dwMin)
                {
                    szMsgFmt = "Invalid value\nmust be greater %i";
                    dwMsgValue = g_terminal.input.dwMin;
                    bInvalid = _TRUE;
                }
                else if (dwValue > g_terminal.input.dwMax)
                {
                    szMsgFmt = "Invalid value\nmust be less %i";
                    dwMsgValue = g_terminal.input.dwMax;
                    bInvalid = _TRUE;
                }

                if (bInvalid)
                {
                    sprintf_s(szMsgBuffer, MESSAGE_BUFFER_SIZE, szMsgFmt, dwMsgValue);
                    BVP_DrawErrorMessage(szMsgBuffer);
                }
                else
                {
                    frame_proc_f proc = BVT_GetPreviousFrame();
                    _SendMsgUpdate(proc, g_terminal.input.bParamNumner);
                    BVT_PopFrame();
                }
            }
                break;

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
                    buttonIndex == BI_0 ? 0 : 
                    buttonIndex < BI_5 ? buttonIndex + 1 : 20 - buttonIndex;
                uint8_t next = isdigit(g_terminal.input.szBuffer[g_terminal.input.wCursorPos]);
                g_terminal.input.szBuffer[g_terminal.input.wCursorPos] = 0x30 + num;
                if (!next)
                    g_terminal.input.szBuffer[g_terminal.input.wCursorPos + 1] = '\0';
                MoveCursor(+1);
                break;
            }

            }
            break;
        case BN_DOWN:
            break;
        }
        BVT_InvalidateRect(0, 1);
        break;
    } // !FM_NOTIFICATION

    }

    return _NULL;
}


/* END OF FILE */

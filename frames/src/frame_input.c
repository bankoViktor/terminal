/*
 * File     frame_input.c
 * Date     05.01.2022
 */

#include "../inc/frame_input.h"

#include "../../core/inc/bv_terminal.h"
#include "../../core/inc/bv_primitives.h"
#include "../../core/inc/bv_hwdriver.h"
#include "../../core/inc/bv_tools.h"
#include "../../core/inc/bv_debug.h"

#include <string.h>
#include <stdlib.h>


#define LABEL_LENGTH_MAX            64


static uint8_t                  g_szBuffer[INPUT_BUFFER_LENGTH_MAX];    // буфер введенного значения
static uint32_t                 g_nLengthMax;                           // максимальная длина вводимого значения
static uint32_t                 g_nCursorPos;                           // позиция курсора ввода
static input_frame_mode_t       g_bmMode;                               // режим ввода
static const uint8_t*           g_pszKeysMap;                           // указатель на строку карты допустимых символов
static uint8_t                  g_nKeysMapPos;                          // позиция в карте допустимых символов
static uint8_t                  g_nFromButtonIndex;


// 0 - Top; 5 - Right; 10 - Bottom; 15 - Left
typedef enum button_index_t
{
    BI_RETURN = 0,
    BI_CLEAR = 1,
    BI_BACKSPACE = 3,
    BI_ENTER = 4,
    BI_KEYS_RIGHT = 10,
    BI_KEYS_LEFT = 11,
    BI_SPACE = 12,
    BI_CURSOR_RIGHT = 13,
    BI_CURSOR_LEFT = 14,
} button_index_t;


static void MoveCursor(int8_t nDeltaPos)
{
    if (nDeltaPos > 0)
    {
        g_nCursorPos = g_nCursorPos < strnlen_s(g_szBuffer, INPUT_BUFFER_LENGTH_MAX)
            ? min(g_nCursorPos + 1, g_nLengthMax - 1)
            : g_nCursorPos;
    }
    else if (nDeltaPos < 0)
    {
        g_nCursorPos = g_nCursorPos > 0
            ? g_nCursorPos - 1
            : 0;
    }
}

static void MoveKeys(int8_t nDeltaPos)
{
    uint8_t nLen = (uint8_t)strlen(g_pszKeysMap);

    if (nDeltaPos > 0 && nLen - g_nKeysMapPos > nDeltaPos ||
        nDeltaPos < 0 && g_nKeysMapPos >= abs(nDeltaPos))
    {
        g_nKeysMapPos += nDeltaPos;
    }
}

static uint8_t GetChar(uint8_t nButtonIndex)
{
    int16_t nCharOffset = -1;

    if (nButtonIndex == BUTTONS_COUNT - 1)
        nCharOffset = 0;
    else if (nButtonIndex < BUTTONS_LEFT_OFFSET)
        nCharOffset = nButtonIndex;
    else
        nCharOffset = BUTTONS_COUNT - 1 - nButtonIndex;

    return nCharOffset == -1
        ? 0
        : g_pszKeysMap[(uint8_t)g_nKeysMapPos + nCharOffset];
}

static void SetChar(uint8_t ch)
{
    if (g_szBuffer[g_nCursorPos] == '\0')
        g_szBuffer[g_nCursorPos + 1] = '\0';

    g_szBuffer[g_nCursorPos] = ch;

    MoveCursor(+1);
}

static uint8_t IsInputButton(uint8_t nButtonIndex)
{
    return nButtonIndex >= BUTTONS_RIGHT_OFFSET && nButtonIndex < BUTTONS_BOTTOM_OFFSET ||
        nButtonIndex >= BUTTONS_LEFT_OFFSET && nButtonIndex < BUTTONS_COUNT;
}


static void OnCreate(uint8_t nButtonIndex)
{
    // Init
    g_nCursorPos = 0;
    g_szBuffer[0] = '\0';
    g_nLengthMax = 0;
    g_bmMode = 0;
    g_nKeysMapPos = 0;

    // Send Init Notification to Caller frame
    frame_proc_f proc = BVT_GetPreviousFrame();
    init_notification_t ntf = { 0 };
    ntf.hdr.fFromProc = FrameInputProc;
    ntf.hdr.nCode = IN_INIT;
    ntf.hdr.nButtonIndex = nButtonIndex;
    ntf.szBuffer = g_szBuffer;
    ntf.nLengthMax = INPUT_BUFFER_LENGTH_MAX;
    ntf.bmMode = g_bmMode;
    _SendMsgNotification(proc, &ntf);

    // Set init values
    g_nLengthMax = ntf.nLengthMax;
    g_bmMode = ntf.bmMode;
    g_pszKeysMap = ntf.pszKeysMap;
    g_nFromButtonIndex = ntf.nFromButtonIndex;
}

static void OnPaint()
{
    // Buffer
    BVP_DrawInput(g_szBuffer, g_nLengthMax, g_nCursorPos);

    // Button
    for (uint8_t nButtonIndex = 0; nButtonIndex < BUTTONS_COUNT; nButtonIndex++)
    {
        // Label
        uint8_t szLabel[LABEL_LENGTH_MAX + 1] = { 0 };

        switch (nButtonIndex)
        {

        case BI_RETURN:
            strcpy_s(szLabel, LABEL_LENGTH_MAX, "RETURN");
            break;

        case BI_CLEAR:
            strcpy_s(szLabel, LABEL_LENGTH_MAX, "CLEAR");
            break;

        case BI_SPACE:
            if (g_bmMode & IFM_TYPE_TEXT)
                strcpy_s(szLabel, LABEL_LENGTH_MAX, "SPACE");
            else if (g_bmMode & IFM_TYPE_DOUBLE)
                strcpy_s(szLabel, LABEL_LENGTH_MAX, "DOT");
            break;

        case BI_BACKSPACE:
            strcpy_s(szLabel, LABEL_LENGTH_MAX, "BACK\nSPACE");
            break;

        case BI_ENTER:
            strcpy_s(szLabel, LABEL_LENGTH_MAX, "ENTER");
            break;

        case BI_KEYS_RIGHT:
        case BI_KEYS_LEFT:
        case BI_CURSOR_RIGHT:
        case BI_CURSOR_LEFT:
        {
            point_t pt = { 0 };
            coord_t nOffset = SAFE_OFFSET + BUTTON_LABEL_OFFSET + TRIANGLE_SIZE / 2 + 1;
            BVT_GetButtonPos(&pt, nButtonIndex, nOffset);

            coord_t hw = TRIANGLE_SIZE / 2;
            rect_t rc = { 0 };
            RECT_SetWithSize(&rc, pt.x - hw, pt.y - hw, TRIANGLE_SIZE, TRIANGLE_SIZE);

            triangle_orientation_t orientation = nButtonIndex == BI_CURSOR_RIGHT || nButtonIndex == BI_KEYS_RIGHT
                ? TO_RIGHT
                : TO_LEFT;

            BVP_DrawDirectionSymbol(&rc, orientation, TEXT_COLOR);
            
           /* if (nButtonIndex == BI_CURSOR_LEFT)
                BVP_DrawButtonText(nButtonIndex, BUTTON_LABEL_OFFSET, "CURSOR", TEXT_COLOR, TEXT_BGCOLOR);
            
            if (nButtonIndex == BI_KEYS_LEFT)
                BVP_DrawButtonText(nButtonIndex, BUTTON_LABEL_OFFSET, "KEYS", TEXT_COLOR, TEXT_BGCOLOR);*/
            break;
        }

        default:
            if (IsInputButton(nButtonIndex))
            {
                uint8_t ch = GetChar(nButtonIndex);
                szLabel[0] = ch;
                szLabel[1] = '\0';
            }
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
        g_szBuffer[0] = '\0';
        g_nCursorPos = 0;
        break;

    case BI_BACKSPACE:
        if (g_szBuffer[g_nCursorPos] == '\0')
            MoveCursor(-1);
        g_szBuffer[g_nCursorPos] = '\0';
        break;

    case BI_ENTER:
    {
        frame_proc_f proc = BVT_GetPreviousFrame();
        update_notification_t ntf = { 0 };
        ntf.hdr.fFromProc = FrameInputProc;
        ntf.hdr.nCode = IN_UPDATE;
        ntf.hdr.nButtonIndex = g_nFromButtonIndex;
        ntf.szBuffer = g_szBuffer;
        ntf.szMessage = _NULL;
        result_t ret = _SendMsgNotification(proc, &ntf);
        if (ret)
            BVT_PopFrame();
        else
        {
            BVP_DrawErrorMessage(ntf.szMessage);
            return result;
        }
        break;
    }

    case BI_KEYS_RIGHT:
        MoveKeys(BUTTON_COUNT_Y * 2);
        break;

    case BI_KEYS_LEFT:
        MoveKeys(-BUTTON_COUNT_Y * 2);
        break;

    case BI_SPACE:
        if (g_bmMode & IFM_TYPE_TEXT)
            SetChar(' ');
        else if (g_bmMode & IFM_TYPE_DOUBLE)
            SetChar('.');
        break;

    case BI_CURSOR_RIGHT:
        MoveCursor(+1);
        break;

    case BI_CURSOR_LEFT:
        MoveCursor(-1);
        break;

    default:
        if (IsInputButton(nButtonIndex))
        {
            uint8_t ch = GetChar(nButtonIndex);
            if (ch > 0)
                SetChar(ch);
        }
        break;

    } // !switch (nButtonIndex)

    BVT_InvalidateRect(_NULL, _TRUE);

    return result;
}


result_t FrameInputProc(
    frame_message_t eMsg,
    param_t param)
{
    PrintFrameMessage(FrameInputProc, eMsg, param);

    result_t result = _NULL;

    switch (eMsg)
    {

    case FM_CREATE:
    {
        uint8_t nButtonIndex = (uint8_t)param;
        OnCreate(nButtonIndex);
        break;
    }

    case FM_DESTROY:
        break;

    case FM_PAINT:
        OnPaint();
        break;

    case FM_BUTTONUP:
    {
        uint8_t nButtonIndex = (uint8_t)param;
        OnButtonUp(nButtonIndex);
        break;
    }

    }

    return result;
}


/* END OF FILE */

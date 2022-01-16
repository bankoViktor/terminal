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
#define LANG_COUNT                  2


uint8_t             g_szValue[INPUT_BUFFER_LENGTH];
uint32_t            g_nLengthMax;
uint32_t            g_nCursorPos;
input_frame_mode_t  g_mode;
input_frame_case_t  g_case;
uint8_t             g_bShift;
uint8_t             g_nLang;
input_frame_lang_t  lang;


static const button_t buttons[BUTTON_COUNT] =
{
    // top - 0
    { "RTN", BT_ACTION },
    { 0 },
    { "CLR", BT_ACTION },
    { "BCK\nSPC", BT_ACTION },
    { "ETR", BT_ACTION },
    // right - 5
    { 0, BT_ACTION },
    { 0, BT_ACTION },
    { 0, BT_ACTION },
    { 0, BT_ACTION },
    { 0, BT_ACTION },
    // bottom - 10
    { 0, BT_ACTION },
    { 0, BT_ACTION },
    { 0, BT_ACTION },
    { "SHIFT\n%s", BT_TOGGLE },
    { "LANG\n%s", BT_TOGGLE },
    // left - 15
    { 0, BT_ACTION },
    { 0, BT_ACTION },
    { 0, BT_ACTION },
    { 0, BT_ACTION },
    { 0, BT_ACTION },
};


typedef struct lang_info_t
{
    input_frame_lang_t  lang;
    const uint8_t*      szName;
    uint8_t             nUpperOffset;
    uint8_t             nLowerOffset;
} lang_info_t;


static const lang_info_t g_aLangs[LANG_COUNT] =
{
    { IFL_EN, "ENG", 64, 96 },
    { IFL_RU, "RUS", 191, 223 },
};


typedef enum button_index_t
{
    // top - 0
    BI_RETURN = 0,
    BI_CLEAR = 2,
    BI_BACKSPACE = 3,
    BI_ENTER = 4,
    // right - 5
    // bottom - 10
    BI_RIGHT = 10,
    BI_LEFT = 11,
    BI_SHIFT = 13,
    BI_LANG = 14,
    // left - 15
    BI_5 = 15,
    BI_4 = 16,
    BI_3 = 17,
    BI_2 = 18,
    BI_1 = 19,
} button_index_t;


static const lang_info_t* GetLang()
{
    return &g_aLangs[g_nLang];
}

static uint8_t GetChar(uint8_t nButtonIndex)
{
    uint8_t result = _NULL;

    uint8_t nCharOffset = nButtonIndex == 9 ? 0 :
        nButtonIndex < 15 ? nButtonIndex + 1 : 20 - nButtonIndex;

    switch (g_mode)
    {

    case IFM_NUMBER:
        result = 48;
        break;

    case IFM_TEXT:
    {
        const lang_info_t* pLI = GetLang();
        result = g_bShift ? pLI->nUpperOffset : pLI->nLowerOffset;
        break;
    }
    
    }

    result += nCharOffset;

    return result;
}

static void MoveCursor(int8_t nDeltaPos)
{
    if (nDeltaPos > 0)
    {
        g_nCursorPos = g_nCursorPos < strlen(g_szValue)
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

static uint8_t IsInputButton(uint8_t nButtonIndex)
{
    return nButtonIndex >= BUTTONS_TOP && nButtonIndex < BUTTONS_RIGHT ||
        nButtonIndex >= BUTTONS_BOTTOM && nButtonIndex < BUTTONS_LEFT;
}

static void OnCreate(uint8_t nButtonIndex)
{
    // Init
    g_szValue[0] = '\0';
    g_nLengthMax = 0;
    g_nCursorPos = 0;
    g_mode = IFM_NUMBER;
    lang = IFL_ALL;
    g_case = IFC_BOTH;

    // Notification
    frame_proc_f proc = BVT_GetPreviousFrame();
    init_notification_t ntf = { 0 };
    ntf.hdr.fromeProc = FrameInputProc;
    ntf.hdr.nCode = IN_INIT;
    ntf.hdr.nButtonIndex = nButtonIndex;
    ntf.szValue = g_szValue;
    ntf.nLengthMax = INPUT_BUFFER_LENGTH;
    ntf.mode = g_mode;
    _SendMsgNotification(proc, &ntf);

    // Set init values
    g_nLengthMax = ntf.nLengthMax;
    g_mode = ntf.mode;
}

static void OnPaint()
{
    // Buffer
    BVP_DrawInput(g_szValue, g_nLengthMax, g_nCursorPos);

    // Button
    for (uint8_t nButtonIndex = 0; nButtonIndex < BUTTON_COUNT; nButtonIndex++)
    {
        const button_t* pButtton = &buttons[nButtonIndex];

        // Label
        uint8_t szLabel[LABEL_LENGTH_MAX + 1] = { 0 };

        switch (nButtonIndex)
        {

        case BI_RIGHT:
        case BI_LEFT:
        {
            point_t pt = { 0 };
            coord_t nOffset = SAFE_OFFSET + BUTTON_LABEL_OFFSET + TRIANGLE_SIZE / 2 + 1;
            BVT_GetButtonPos(&pt, nButtonIndex, nOffset);

            coord_t hw = TRIANGLE_SIZE / 2;

            rect_t rc = { 0 };
            RECT_SetWithSize(&rc, pt.x - hw, pt.y - hw, TRIANGLE_SIZE, TRIANGLE_SIZE);

            triangle_orientation_t or = nButtonIndex == BI_RIGHT ? TO_RIGHT : TO_LEFT;
            BVP_DrawDirectionSymbol(&rc, or , TEXT_COLOR);
            break;
        }

        case BI_RETURN:
        case BI_CLEAR:
        case BI_BACKSPACE:
        case BI_ENTER:
            strcpy_s(szLabel, LABEL_LENGTH_MAX, pButtton->szTitle);
            break;

        case BI_SHIFT:
            if (g_mode == IFM_TEXT && g_case == IFC_BOTH)
                sprintf_s(szLabel, LABEL_LENGTH_MAX, pButtton->szTitle, g_bShift ? "ON" : "OFF");
            break;

        case BI_LANG:
            if (g_mode == IFM_TEXT && lang == IFL_ALL)
                sprintf_s(szLabel, LABEL_LENGTH_MAX, pButtton->szTitle, GetLang(g_nLang)->szName);
            break;

        default:
        {
            if (IsInputButton(nButtonIndex))
            {
                uint8_t ch = GetChar(nButtonIndex);
                szLabel[0] = ch;
                szLabel[1] = '\0';
            }
            break;
        }

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
        g_szValue[0] = '\0';
        g_nCursorPos = 0;
        break;

    case BI_BACKSPACE:
        if (g_szValue[g_nCursorPos] == '\0')
            MoveCursor(-1);
        g_szValue[g_nCursorPos] = '\0';
        break;

    case BI_ENTER:
    {
        frame_proc_f proc = BVT_GetPreviousFrame();
        update_notification_t ntf = { 0 };
        ntf.hdr.fromeProc = FrameInputProc;
        ntf.hdr.nCode = IN_UPDATE;
        ntf.szValue = g_szValue;
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

    case BI_SHIFT:
        g_bShift = !g_bShift;
        break;

    case BI_LANG:
        g_nLang = TOGGLE_IN_RANGE(g_nLang, 0, LANG_COUNT - 1);
        break;

    default:
    {
        if (IsInputButton(nButtonIndex))
        {
            uint8_t ch = GetChar(nButtonIndex);
            switch (g_mode)
            {

            case IFM_NUMBER:
                g_szValue[g_nCursorPos] = ch;
                if (!isdigit(g_szValue[g_nCursorPos]))
                    g_szValue[g_nCursorPos + 1] = '\0';
                MoveCursor(+1);
                break;

            case IFM_TEXT:
                g_szValue[g_nCursorPos] = ch;
                g_szValue[g_nCursorPos + 1] = '\0';
                MoveCursor(+1);
                break;

            } // !switch (g_mode)
        }
        break;
    }

    } // !switch (nButtonIndex)

    BVT_InvalidateRect(_NULL, _TRUE);

    return result;
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
        OnCreate((uint8_t)param);
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

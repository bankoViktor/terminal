/*
 * File     bv_terminal.c
 * Date     28.12.2021
 */

#include "bv_terminal.h"
#include "bv_frame.h"
#include "bv_config.h"
#include "bv_types.h"
#include "bv_hwdriver.h"
#include "bv_primitives.h"
#include "frame_tab1.h"

#include "config_win32.h"

#include <string.h>
#include <stdio.h>


terminal_t g_terminal = { 0 };


// TODO Шрифт
// TODO Смещение безопасности для ХY или LTRB по отдельности
// TODO Реализация обработки клавиатуры (состояния UP/Down)
// TODO Сабклассинг для BUTTON для перехвата UP/DOWN событий


void BVT_Init()
{
    g_terminal.data.bBool = _TRUE;
    g_terminal.data.bSelectable = 2;
    g_terminal.data.bMode = _FALSE;
    g_terminal.data.wInputNumber = 32;
    strcpy_s(g_terminal.data.szInputText, 16, "mark-1");

    BVT_PushFrame(FrameTab1Proc);
}

void BVT_InvalidateRect(
    const rect_t* prc,
    uint8_t bgErase)
{
    if (bgErase)
    {
        rect_t rc = { 0 };
        BVT_GetRect(&rc);
        BVG_DrawFill(&rc, BACKGROUND_COLOR);
    }

    // Grid
    BVP_DrawButtonGrid();

    // Safe rect
    BVP_DrawClientRect();

    // Frame proc
    frame_proc_f proc = BVT_GetTopFrame();
    _SendMsgPaint(proc);
}

void BVT_GetRect(
    rect_t* prc)
{
    prc->left = SAFE_OFFSET_LEFT;
    prc->top = SAFE_OFFSET_TOP;
    prc->right = TERMINAL_WIDTH - SAFE_OFFSET_RIGHT;
    prc->bottom = TERMINAL_HEIGHT - SAFE_OFFSET_BOTTOM;
}

void BVT_GetClientRect(
    rect_t* prc)
{
    BVT_GetRect(prc);
    RECT_Inflate(prc, -SAFE_OFFSET, -SAFE_OFFSET);
}

void BVT_GetButtonPos(
    point_t* ppt,
    uint8_t nIndex,
    int16_t nOffset)
{
    rect_t rc = { 0 };
    BVT_GetRect(&rc);

    double dStepX = (double)(RECT_GetWidth(&rc) + BUTTON_STRECH_X * 2) / (BUTTON_COUNT_X + 1);
    double dStepY = (double)(RECT_GetHeight(&rc) + BUTTON_STRECH_Y * 2) / (BUTTON_COUNT_Y + 1);

    if (nIndex < BUTTONS_TOP)
    {
        // Top
        ppt->x = rc.left - BUTTON_STRECH_X + BUTTON_MOVE_X + (coord_t)(dStepX * (nIndex + 1));
        ppt->y = rc.top + nOffset;
    }
    else if (nIndex < BUTTONS_RIGHT)
    {
        // Right
        ppt->x = rc.right - 1 - nOffset;
        ppt->y = rc.top - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (coord_t)(dStepY * (nIndex - BUTTONS_TOP + 1));
    }
    else if (nIndex < BUTTONS_BOTTOM)
    {
        // Bottom
        ppt->x = rc.left - BUTTON_STRECH_X + BUTTON_MOVE_X + (coord_t)(dStepX * (BUTTON_COUNT_X - nIndex + BUTTONS_RIGHT));
        ppt->y = rc.bottom - 1 - nOffset;
    }
    else if (nIndex < BUTTONS_LEFT)
    {
        // Left
        ppt->x = rc.left + nOffset;
        ppt->y = rc.top - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (coord_t)(dStepY * (BUTTON_COUNT_Y - nIndex + BUTTONS_BOTTOM));
    }
}

void BVT_PushFrame(
    frame_proc_f proc)
{
    if (g_terminal.nCounter < TERMINAL_STACK_SIZE)
    {
        g_terminal.stack[g_terminal.nCounter] = proc;
        g_terminal.nCounter++;

        _SendMsgCreate(proc);
    }
}

frame_proc_f BVT_PopFrame()
{
    if (g_terminal.nCounter > 0)
    {
        frame_proc_f proc = g_terminal.stack[g_terminal.nCounter - 1];
        g_terminal.nCounter--;

        _SendMsgDestroy(proc);
        return proc;
    }
    return 0;
}

frame_proc_f BVT_GetTopFrame()
{
    return g_terminal.stack[g_terminal.nCounter - 1];
}

frame_proc_f BVT_GetPreviousFrame()
{
    return  g_terminal.nCounter == 0
        ? _NULL
        : g_terminal.stack[g_terminal.nCounter - 2];
}

void BVT_OffsetByButton(
    rect_t* prc,
    const point_t* ppt,
    uint8_t nButtonIndex)
{
    coord_t dx = 0;
    coord_t dy = 0;

    if (nButtonIndex < BUTTONS_TOP)
    {
        // Top
        dx = -RECT_GetWidth(prc) / 2;
        dy = 0;
    }
    else if (nButtonIndex < BUTTONS_RIGHT)
    {
        // Right
        dx = -RECT_GetWidth(prc) + 1;
        dy = -RECT_GetHeight(prc) / 2;
    }
    else if (nButtonIndex < BUTTONS_BOTTOM)
    {
        // Bottom
        dx = -RECT_GetWidth(prc) / 2;
        dy = -RECT_GetHeight(prc) + 1;
    }
    else if (nButtonIndex < BUTTONS_LEFT)
    {
        // Left
        dx = 0;
        dy = -RECT_GetHeight(prc) / 2;
    }

    RECT_Offset(prc, ppt->x + dx, ppt->y + dy);
}

void BVT_GetAlignByIndex(
    uint8_t bIndex,
    horizontal_aligment_t* phAlign,
    vertical_aligment_t* pvAlign)
{
    if (bIndex < BUTTONS_TOP)
    {
        // Top
        *phAlign = H_ALIGN_CENTER;
        *pvAlign = V_ALIGN_TOP;
    }
    else if (bIndex < BUTTONS_RIGHT)
    {
        // Right
        *phAlign = H_ALIGN_RIGHT;
        *pvAlign = V_ALIGN_MIDDLE;
    }
    else if (bIndex < BUTTONS_BOTTOM)
    {
        // Bottom
        *phAlign = H_ALIGN_CENTER;
        *pvAlign = V_ALIGN_BOTTOM;
    }
    else if (bIndex < BUTTONS_LEFT)
    {
        // Left
        *phAlign = H_ALIGN_LEFT;
        *pvAlign = V_ALIGN_MIDDLE;
    }
}


/* END OF FILE */

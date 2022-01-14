/*
 * File     bv_terminal.c
 * Date     28.12.2021
 */

#include "bv_terminal.h"
#include "bv_frame.h"
#include "bv_config.h"
#include "bv_types.h"
#include "bv_hwdriver.h"
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
    strcpy_s(g_terminal.data.szInputText, 15, "mark-1");

    BVT_PushFrame(FrameTab1Proc);
}

void BVT_InvalidateRect(
    const rect_t* prc,
    uint8_t bgErase)
{
    if (bgErase)
    {
        rect_t rc = { 0 };
        BVT_GetClientRect(&rc);
        BVG_DrawFill(&rc, BACKGROUND_COLOR);
    }

    // Grid
    BVG_DrawButtonGrid();

    // Save rect
    BVG_DrawClientRect();

    // Frame proc
    frame_proc_f proc = BVT_GetTopFrame();
    _SendMsgPaint(proc);
}

void BVT_GetClientRect(
    rect_t* prc)
{
    RECT_SetWithSize(prc,
        CTL_BUTTON_ZONE_SIZE, CTL_BUTTON_ZONE_SIZE,
        TERMINAL_WIDTH, TERMINAL_HEIGHT);

    //RECT_Inflate(prc, -BUTTON_OFFSET, -BUTTON_OFFSET);
}

void BVT_CalcButtonPos(
    point_t* ppt,
    uint8_t nIndex,
    int16_t nOffset)
{
    rect_t rc = { 0 };
    BVT_GetClientRect(&rc);

    double stepX = (double)(RECT_GetWidth(&rc) + BUTTON_STRECH_X * 2) / (BUTTON_COUNT_X + 1);
    double stepY = (double)(RECT_GetHeight(&rc) + BUTTON_STRECH_Y * 2) / (BUTTON_COUNT_Y + 1);

    if (nIndex < BUTTONS_TOP)
    {
        // Top
        ppt->x = rc.left - BUTTON_STRECH_X + BUTTON_MOVE_X + (coord_t)(stepX * (nIndex + 1));
        ppt->y = rc.top + nOffset;
    }
    else if (nIndex < BUTTONS_RIGHT)
    {
        // Right
        ppt->x = rc.right - 1 - nOffset;
        ppt->y = rc.top - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (coord_t)(stepY * (nIndex - BUTTONS_TOP + 1));
    }
    else if (nIndex < BUTTONS_BOTTOM)
    {
        // Bottom
        ppt->x = rc.left - BUTTON_STRECH_X + BUTTON_MOVE_X + (coord_t)(stepX * (BUTTON_COUNT_X - nIndex + BUTTONS_RIGHT));
        ppt->y = rc.bottom - 1 - nOffset;
    }
    else if (nIndex < BUTTONS_LEFT)
    {
        // Left
        ppt->x = rc.left + nOffset;
        ppt->y = rc.top - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (coord_t)(stepY * (BUTTON_COUNT_Y - nIndex + BUTTONS_BOTTOM));
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


/* END OF FILE */

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

#include <string.h>
#include <stdio.h>


terminal_t g_terminal = { 0 };


void BVT_Magic()
{
    // Keyboard
    //BVK_HandleKeyboard();

    frame_proc_f proc = BVT_GetTopFrame();
    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        button_state_t state = BVK_GetState(i);

        switch (state)
        {

        case BS_DOWN:
            BVK_Clear(i);
            _SendMsgButtonDown(proc, i);
            break;

        case BS_UP:
            BVK_Clear(i);
            _SendMsgButtonUp(proc, i);
            break;

        }
    }
}

void BVT_Init()
{
    g_terminal.data.bBool = _TRUE;
    g_terminal.data.bSelectable = 2;
    g_terminal.data.bMode = _FALSE;
    g_terminal.data.wInputNumber = 32;
    strcpy_s(g_terminal.data.szInputText, 15, "mark-1");
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
    prc->left = SAFE_OFFSET_LEFT;
    prc->top = SAFE_OFFSET_TOP;
    prc->right = TERMINAL_WIDTH - SAFE_OFFSET_RIGHT;
    prc->bottom = TERMINAL_HEIGHT - SAFE_OFFSET_BOTTOM;
}

void BVT_CalcButtonPos(
    point_t* ppt,
    uint8_t nIndex,
    int16_t nOffset)
{
    rect_t rc = { 0, 0, TERMINAL_WIDTH, TERMINAL_HEIGHT };

    double fStepX = (double)(RECT_GetWidth(&rc) + BUTTON_STRECH_X * 2) / (BUTTON_COUNT_X + 1);
    double fStepY = (double)(RECT_GetHeight(&rc) + BUTTON_STRECH_Y * 2) / (BUTTON_COUNT_Y + 1);

    if (nIndex < BUTTONS_TOP)
    {
        // Top
        ppt->x = rc.left - BUTTON_STRECH_X + BUTTON_MOVE_X + (coord_t)(fStepX * (nIndex + 1));
        ppt->y = rc.top + nOffset;
    }
    else if (nIndex < BUTTONS_RIGHT)
    {
        // Right
        // Right
        ppt->x = rc.right - 1 - nOffset;
        ppt->y = rc.top - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (coord_t)(fStepY * (nIndex - BUTTONS_TOP + 1));
    }
    else if (nIndex < BUTTONS_BOTTOM)
    {
        // Bottom
        ppt->x = rc.left - BUTTON_STRECH_X + BUTTON_MOVE_X + (coord_t)(fStepX * (BUTTON_COUNT_X - nIndex + BUTTONS_RIGHT));
        ppt->y = rc.bottom - 1 - nOffset;
    }
    else if (nIndex < BUTTONS_LEFT)
    {
        // Left
        ppt->x = rc.left + nOffset;
        ppt->y = rc.top - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (coord_t)(fStepY * (BUTTON_COUNT_Y - nIndex + BUTTONS_BOTTOM));
    }
}

void BVT_PushFrame(
    frame_proc_f proc)
{
    if (g_terminal.bCounter < TERMINAL_STACK_SIZE)
    {
        g_terminal.aStack[g_terminal.bCounter] = proc;
        g_terminal.bCounter++;
        _SendMsgCreate(proc);
        _SendMsgEraseBackground(proc);
        _SendMsgPaint(proc);
    }
}

frame_proc_f BVT_PopFrame()
{
    if (g_terminal.bCounter > 0)
    {
        frame_proc_f fProc = g_terminal.aStack[g_terminal.bCounter - 1];
        _SendMsgDestroy(fProc);

        g_terminal.bCounter--;

        frame_proc_f fProc2 = g_terminal.aStack[g_terminal.bCounter - 1];
        _SendMsgEraseBackground(fProc2);
        _SendMsgPaint(fProc2);
        return fProc;
    }
    return _NULL;
}

frame_proc_f BVT_GetTopFrame()
{
    return g_terminal.aStack[g_terminal.bCounter - 1];
}

frame_proc_f BVT_GetPreviousFrame()
{
    return  g_terminal.bCounter == 0 
        ? _NULL 
        : g_terminal.aStack[g_terminal.bCounter - 2];
}

void BVT_OffsetButton(
    rect_t* prc,
    const point_t* ppt,
    uint8_t bIndex)
{
    coord_t dx = 0;
    coord_t dy = 0;

    if (bIndex < BUTTONS_TOP)
    {
        // Top
        dx = -RECT_GetWidth(prc) / 2;
        dy = 0;
    }
    else if (bIndex < BUTTONS_RIGHT)
    {
        // Right
        dx = -RECT_GetWidth(prc) + 1;
        dy = -RECT_GetHeight(prc) / 2;
    }
    else if (bIndex < BUTTONS_BOTTOM)
    {
        // Bottom
        dx = -RECT_GetWidth(prc) / 2;
        dy = -RECT_GetHeight(prc) + 1;
    }
    else if (bIndex < BUTTONS_LEFT)
    {
        // Left
        dx = 0;
        dy = -RECT_GetHeight(prc) / 2;
    }

    RECT_Offset(prc, ppt->x + dx, ppt->y + dy);
}


/* END OF FILE */

/*
 * File     bv_terminal.c
 * Date     28.12.2021
 */

#include <string.h>
#include <stdio.h>

#include "../inc/bv_terminal.h"
#include "../inc/bv_frame.h"
#include "../inc/bv_config.h"
#include "../inc/bv_types.h"
#include "../inc/bv_hwdriver.h"
#include "../inc/bv_primitives.h"

#include "../../config_win32.h"


static terminal_t g_terminal = { 0 };


// TODO Шрифт
// TODO Смещение безопасности для ХY или LTRB по отдельности
// TODO Реализация обработки клавиатуры (состояния UP/Down)
// TODO Сабклассинг для BUTTON для перехвата UP/DOWN событий

void BVT_Init(frame_proc_f firstFrameProc)
{
    BVT_PushFrame(firstFrameProc, _NULL);
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

void BVT_GetRect(rect_t* prc)
{
    prc->left = SAFE_OFFSET_LEFT;
    prc->top = SAFE_OFFSET_TOP;
    prc->right = TERMINAL_WIDTH - SAFE_OFFSET_RIGHT;
    prc->bottom = TERMINAL_HEIGHT - SAFE_OFFSET_BOTTOM;
}

void BVT_GetClientRect(rect_t* prc)
{
    BVT_GetRect(prc);
    RECT_Inflate(prc, -SAFE_OFFSET, -SAFE_OFFSET);
}

void BVT_GetButtonStep(double* pdStepX, double* pdStepY)
{
    if (pdStepX == 0 && pdStepY == 0) return;

    rect_t rc = { 0 };
    BVT_GetRect(&rc);

    if (pdStepX)
        *pdStepX = (double)(RECT_GetWidth(&rc) + BUTTON_STRECH_X * 2) / (BUTTON_COUNT_X + 1);
    
    if (pdStepY)
        *pdStepY = (double)(RECT_GetHeight(&rc) + BUTTON_STRECH_Y * 2) / (BUTTON_COUNT_Y + 1);
}

void BVT_GetButtonPos(
    point_t* pPt,
    uint8_t nButtonIndex,
    int16_t nOffset)
{
    double dStepX;
    double dStepY;
    BVT_GetButtonStep(&dStepX, &dStepY);

    rect_t rc = { 0 };
    BVT_GetRect(&rc);
    if (nButtonIndex < BUTTONS_RIGHT_OFFSET)
    {
        // Top
        pPt->x = rc.left - BUTTON_STRECH_X + BUTTON_MOVE_X + (coord_t)(dStepX * (nButtonIndex + 1));
        pPt->y = rc.top + nOffset;
    }
    else if (nButtonIndex < BUTTONS_BOTTOM_OFFSET)
    {
        // Right
        pPt->x = rc.right - 1 - nOffset;
        pPt->y = rc.top - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (coord_t)(dStepY * (nButtonIndex - BUTTONS_RIGHT_OFFSET + 1));
    }
    else if (nButtonIndex < BUTTONS_LEFT_OFFSET)
    {
        // Bottom
        pPt->x = rc.left - BUTTON_STRECH_X + BUTTON_MOVE_X + (coord_t)(dStepX * (BUTTON_COUNT_X - nButtonIndex + BUTTONS_BOTTOM_OFFSET));
        pPt->y = rc.bottom - 1 - nOffset;
    }
    else if (nButtonIndex < BUTTONS_COUNT)
    {
        // Left
        pPt->x = rc.left + nOffset;
        pPt->y = rc.top - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (coord_t)(dStepY * (BUTTON_COUNT_Y - nButtonIndex + BUTTONS_LEFT_OFFSET));
    }
}

void BVT_PushFrame(frame_proc_f proc, uint8_t nButtonIndex)
{
    if (g_terminal.nFrameCounter < TERMINAL_STACK_SIZE)
    {
        g_terminal.afFrameStack[g_terminal.nFrameCounter] = proc;
        g_terminal.nFrameCounter++;

        _SendMsgCreate(proc, nButtonIndex);
    }
}

frame_proc_f BVT_PopFrame()
{
    if (g_terminal.nFrameCounter > 0)
    {
        frame_proc_f proc = g_terminal.afFrameStack[g_terminal.nFrameCounter - 1];
        g_terminal.nFrameCounter--;

        _SendMsgDestroy(proc);
        return proc;
    }
    return 0;
}

void BVT_ChangeFrame(frame_proc_f proc)
{
    BVT_PopFrame();
    BVT_PushFrame(proc, _NULL);
}

frame_proc_f BVT_GetTopFrame()
{
    return g_terminal.afFrameStack[g_terminal.nFrameCounter - 1];
}

frame_proc_f BVT_GetPreviousFrame()
{
    return  g_terminal.nFrameCounter == 0
        ? _NULL
        : g_terminal.afFrameStack[g_terminal.nFrameCounter - 2];
}

void BVT_OffsetByButton(
    rect_t* prc,
    const point_t* ppt,
    uint8_t nButtonIndex)
{
    coord_t dx = 0;
    coord_t dy = 0;

    if (nButtonIndex < BUTTONS_RIGHT_OFFSET)
    {
        // Top
        dx = -RECT_GetWidth(prc) / 2;
        dy = 0;
    }
    else if (nButtonIndex < BUTTONS_BOTTOM_OFFSET)
    {
        // Right
        dx = -RECT_GetWidth(prc) + 1;
        dy = -RECT_GetHeight(prc) / 2;
    }
    else if (nButtonIndex < BUTTONS_LEFT_OFFSET)
    {
        // Bottom
        dx = -RECT_GetWidth(prc) / 2;
        dy = -RECT_GetHeight(prc) + 1;
    }
    else if (nButtonIndex < BUTTONS_COUNT)
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
    if (bIndex < BUTTONS_RIGHT_OFFSET)
    {
        // Top
        *phAlign = H_ALIGN_CENTER;
        *pvAlign = V_ALIGN_TOP;
    }
    else if (bIndex < BUTTONS_BOTTOM_OFFSET)
    {
        // Right
        *phAlign = H_ALIGN_RIGHT;
        *pvAlign = V_ALIGN_MIDDLE;
    }
    else if (bIndex < BUTTONS_LEFT_OFFSET)
    {
        // Bottom
        *phAlign = H_ALIGN_CENTER;
        *pvAlign = V_ALIGN_BOTTOM;
    }
    else if (bIndex < BUTTONS_COUNT)
    {
        // Left
        *phAlign = H_ALIGN_LEFT;
        *pvAlign = V_ALIGN_MIDDLE;
    }
}


/* END OF FILE */

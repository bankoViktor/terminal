/*
 * File     bv_terminal.c
 * Date     28.12.2021
 */

#include "bv_terminal.h"
#include "bv_frame.h"
#include "bv_config.h"
#include "bv_types.h"
#include "bv_hwdriver.h"

#include "main.h"


static terminal_t g_terminal = { 0 };


void BVT_InvalidateRect(
    const rect_t* prc,
    uint8_t bgErase)
{
    if (bgErase)
    {
        // Erase
    }

    // Draw in Rect
    BVG_DrawButtonGrid();

    // Draw Button Markers
    //point_t pt = { 0 };
    //for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    //{
    //    BVT_CalcButtonPos(&pt, i, 0);
    //    BVG_DrawMarker(&pt, MARKER_COLOR);
    //}

    frame_proc_f proc = BVT_GetTopFrame();
    if (proc)
    {
        proc(FM_PAINT, 0);
    }
}

void BVT_GetClientRect(
    rect_t* prc)
{
    RECT_SetWithSize(prc,
        CTL_BUTTON_ZONE_SIZE, CTL_BUTTON_ZONE_SIZE,
        TERMINAL_WIDTH, TERMINAL_HEIGHT);
}

void BVT_CalcButtonPos(
    point_t* ppt,
    uint8_t buttonIndex,
    int16_t delta)
{
    rect_t rc = { 0 };
    BVT_GetClientRect(&rc);

    double stepX = (double)(RECT_GetWidth(&rc) + BUTTON_STRECH_X * 2) / (BUTTON_COUNT_X + 1);
    double stepY = (double)(RECT_GetHeight(&rc) + BUTTON_STRECH_Y * 2) / (BUTTON_COUNT_Y + 1);

    if (buttonIndex < BUTTONS_TOP)
    {
        // Top
        ppt->x = rc.left - BUTTON_STRECH_X + BUTTON_MOVE_X + (coord_t)(stepX * (buttonIndex + 1));
        ppt->y = rc.top + delta;
    }
    else if (buttonIndex < BUTTONS_RIGHT)
    {
        // Right
        ppt->x = rc.right - 1 - delta;
        ppt->y = rc.top - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (coord_t)(stepY * (buttonIndex - BUTTONS_TOP + 1));
    }
    else if (buttonIndex < BUTTONS_BOTTOM)
    {
        // Bottom
        ppt->x = rc.left - BUTTON_STRECH_X + BUTTON_MOVE_X + (coord_t)(stepX * (BUTTON_COUNT_X - buttonIndex + BUTTONS_RIGHT));
        ppt->y = rc.bottom - 1 - delta;
    }
    else if (buttonIndex < BUTTONS_LEFT)
    {
        // Left
        ppt->x = rc.left + delta;
        ppt->y = rc.top - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (coord_t)(stepY * (BUTTON_COUNT_Y - buttonIndex + BUTTONS_BOTTOM));
    }
}

void BVT_FramePush(
    frame_proc_f proc)
{
    if (g_terminal.nCounter < TERMINAL_STACK_SIZE)
    {
        g_terminal.stack[g_terminal.nCounter++] = proc;
    }
}

frame_proc_f BVT_FramePop()
{
    return g_terminal.nCounter > 0
        ? g_terminal.stack[g_terminal.nCounter--]
        : 0;
}

frame_proc_f BVT_GetTopFrame()
{
    return g_terminal.stack[g_terminal.nCounter - 1];
}

/* END OF FILE */

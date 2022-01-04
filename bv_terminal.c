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

#include "main.h"


terminal_t g_terminal = { 0 };


// TODO Шрифт
// TODO Отрисовка примитивов
// TODO Отрисовка в памяти (и перенос на окна для Windows)
// TODO Заливка кадра в памяти по щелчку мыши (для отдадки перерисовки)
// TODO Смещение безопасности для ХY или LTRB по отдельности
// TODO Реализация обработки клавиатуры (состояния UP/Down
// TODO Сабклассинг для BUTTON для перехвата UP/DOWN событий


void BVT_Init()
{
    g_terminal.nInputNumber = 4432;
    g_terminal.bBool = 1;
    g_terminal.nSelectable = 2;
    strcpy_s(g_terminal.szInputText, 15, "mark-1");

    BVT_FramePush(FrameTab1Proc);
}

void BVT_InvalidateRect(
    const rect_t* prc,
    uint8_t bgErase)
{
    if (bgErase)
    {
        // Erase
    }

    // Grid
    BVG_DrawButtonGrid();

    // Save rect
    BVG_DrawClientRect();

    // Frame proc
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

    //RECT_Inflate(prc, -BUTTON_OFFSET, -BUTTON_OFFSET);
}

void BVT_CalcButtonPos(
    point_t* ppt,
    uint8_t index,
    int16_t offset)
{
    rect_t rc = { 0 };
    BVT_GetClientRect(&rc);

    double stepX = (double)(RECT_GetWidth(&rc) + BUTTON_STRECH_X * 2) / (BUTTON_COUNT_X + 1);
    double stepY = (double)(RECT_GetHeight(&rc) + BUTTON_STRECH_Y * 2) / (BUTTON_COUNT_Y + 1);

    if (index < BUTTONS_TOP)
    {
        // Top
        ppt->x = rc.left - BUTTON_STRECH_X + BUTTON_MOVE_X + (coord_t)(stepX * (index + 1));
        ppt->y = rc.top + offset;
    }
    else if (index < BUTTONS_RIGHT)
    {
        // Right
        ppt->x = rc.right - 1 - offset;
        ppt->y = rc.top - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (coord_t)(stepY * (index - BUTTONS_TOP + 1));
    }
    else if (index < BUTTONS_BOTTOM)
    {
        // Bottom
        ppt->x = rc.left - BUTTON_STRECH_X + BUTTON_MOVE_X + (coord_t)(stepX * (BUTTON_COUNT_X - index + BUTTONS_RIGHT));
        ppt->y = rc.bottom - 1 - offset;
    }
    else if (index < BUTTONS_LEFT)
    {
        // Left
        ppt->x = rc.left + offset;
        ppt->y = rc.top - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (coord_t)(stepY * (BUTTON_COUNT_Y - index + BUTTONS_BOTTOM));
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

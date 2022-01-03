/*
 * File     bvg_terminal.h
 * Date     28.12.2021
 */

#ifndef __BVG_TERMINAL_H
#define __BVG_TERMINAL_H

#include "bvg_rect.h"
#include "bvg_config.h"


typedef struct terminal_t
{
    frame_proc_f    stack[TERMINAL_STACK_SIZE];
    uint8_t         nCounter;
} terminal_t;


void BVT_FramePush(
    frame_proc_f proc);

frame_proc_f BVT_FramePop();

frame_proc_f BVT_GetTopFrame();

void BVT_InvalidateRect(
    const rect_t* prc,
    uint8_t bgErase);

void BVT_CalcButtonPos(
    point_t* ppt,
    uint8_t buttonIndex,
    int16_t delta);

void BVT_GetClientRect(
    rect_t* prc);


#endif // !__BVG_TERMINAL_H

 /* END OF FILE */

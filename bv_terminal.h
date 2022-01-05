/*
 * File     bv_terminal.h
 * Date     28.12.2021
 */

#ifndef __BV_TERMINAL_H
#define __BV_TERMINAL_H

#include "bv_rect.h"
#include "bv_config.h"
#include "bv_frame.h"


typedef struct terminal_t
{
    frame_proc_f    stack[TERMINAL_STACK_SIZE];
    uint8_t         nCounter;
    uint8_t         bBool;
    uint8_t         nSelectable;
    uint16_t        nInputNumber;
    char            szInputText[16];
    uint8_t         nMode;
} terminal_t;


extern void BVT_Init();

extern void BVT_FramePush(
    frame_proc_f proc);

extern frame_proc_f BVT_FramePop();

extern frame_proc_f BVT_GetTopFrame();

extern void BVT_InvalidateRect(
    const rect_t* prc,
    uint8_t bgErase);

extern void BVT_CalcButtonPos(
    point_t* ppt,
    uint8_t index,
    int16_t offset);

extern void BVT_GetClientRect(
    rect_t* prc);


#endif // !__BV_TERMINAL_H

 /* END OF FILE */

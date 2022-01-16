/*
 * File     bv_terminal.h
 * Date     28.12.2021
 */

#ifndef __BV_TERMINAL_H
#define __BV_TERMINAL_H

#include "bv_config.h"
#include "bv_rect.h"
#include "bv_frame.h"


#define INPUT_TEXT_SIZE              16


typedef struct terminal_t
{
    /* Данные терминала */
    frame_proc_f    stack[TERMINAL_STACK_SIZE];
    uint8_t         nCounter;

    /* Данные */
    struct
    {
        uint8_t         bBool;
        uint8_t         bSelectable;
        uint8_t         bMode;
        uint16_t        wInputNumber;
        uint8_t         szInputText[INPUT_TEXT_SIZE];
    }               data;

} terminal_t;


extern void BVT_Init();

extern void BVT_InvalidateRect(
    const rect_t* prc,
    uint8_t bgErase);

extern void BVT_GetRect(
    rect_t* prc);

extern void BVT_GetClientRect(
    rect_t* prc);

extern void BVT_GetButtonPos(
    point_t* ppt,
    uint8_t nIndex,
    int16_t nOffset);

extern void BVT_PushFrame(
    frame_proc_f proc,
    uint8_t nButtonIndex);

extern frame_proc_f BVT_PopFrame();

extern frame_proc_f BVT_GetTopFrame();

extern frame_proc_f BVT_GetPreviousFrame();

extern void BVT_OffsetByButton(
    rect_t* prc,
    const point_t* ppt,
    uint8_t nIndex);

extern void BVT_GetAlignByIndex(
    uint8_t nIndex,
    horizontal_aligment_t* phAlign,
    vertical_aligment_t* pvAlign);


#endif // !__BV_TERMINAL_H

 /* END OF FILE */

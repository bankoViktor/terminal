/*
 * File     bv_terminal.h
 * Date     28.12.2021
 */

#ifndef __BV_TERMINAL_H
#define __BV_TERMINAL_H

#include "bv_config.h"
#include "bv_rect.h"
#include "bv_frame.h"


#define INPUT_TEXT_SIZE              15


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
        uint8_t         szInputText[INPUT_TEXT_SIZE + 1];
    }               data;

    /* Данные фрейма ввода */
    struct 
    {
        uint8_t         szBuffer[INPUT_BUFFER_LENGTH + 1];
        uint8_t         bParamNumner;
        uint16_t        wCursorPos;
        uint16_t        wLengthMax;
        uint32_t        dwMin;
        uint32_t        dwMax;
    }               input;

} terminal_t;


extern void BVT_Init();

extern void BVT_InvalidateRect(
    const rect_t* prc,
    uint8_t bgErase);

extern void BVT_GetClientRect(
    rect_t* prc);

extern void BVT_CalcButtonPos(
    point_t* ppt,
    uint8_t index,
    int16_t offset);

extern void BVT_PushFrame(
    frame_proc_f proc);

extern frame_proc_f BVT_PopFrame();

extern frame_proc_f BVT_GetTopFrame();

extern frame_proc_f BVT_GetPreviousFrame();


#endif // !__BV_TERMINAL_H

 /* END OF FILE */

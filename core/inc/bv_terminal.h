/*
 * File     bv_terminal.h
 * Date     28.12.2021
 */

#ifndef __BV_TERMINAL_H
#define __BV_TERMINAL_H

#include "bv_config.h"
#include "bv_rect.h"
#include "bv_frame.h"


/* Системные данные терминала */
typedef struct terminal_t
{
    /* Стек фреймов */
    frame_proc_f    afFrameStack[TERMINAL_STACK_SIZE];

    /* Счетчик вершины стека фреймов */
    uint8_t         nFrameCounter;
} terminal_t;


/// <summary>
/// Инициализирует терминал.
/// </summary>
/// <param name="firstFrameProc">Домашний фрейм.</param>
void BVT_Init(frame_proc_f firstFrameProc);

void BVT_InvalidateRect(
    const rect_t* prc,
    uint8_t bgErase);

void BVT_GetRect(rect_t* prc);

void BVT_GetClientRect(rect_t* prc);

void BVT_GetButtonPos(
    point_t* ppt,
    uint8_t nIndex,
    int16_t nOffset);

/// <summary>
/// Добавляет новый фрейм в стек фреймов.
/// </summary>
/// <param name="proc">Новый фрейм.</param>
/// <param name="nButtonIndex">Индекс кнопки ??</param>
void BVT_PushFrame(frame_proc_f proc, uint8_t nButtonIndex);

/// <summary>
/// Извлекает и возвращает текущий фрейм.
/// </summary>
/// <returns>Текущий фрейм.</returns>
frame_proc_f BVT_PopFrame();

/// <summary>
/// Изменяет текущий фрейм на указанный.
/// </summary>
/// <param name="proc">Новый фрейм.</param>
void BVT_ChangeFrame(frame_proc_f proc);

/// <summary>
/// Возвращает текущий фрейм.
/// </summary>
/// <returns>Текущий фрейм.</returns>
frame_proc_f BVT_GetTopFrame();

frame_proc_f BVT_GetPreviousFrame();

void BVT_OffsetByButton(
    rect_t* prc,
    const point_t* ppt,
    uint8_t nIndex);

void BVT_GetAlignByIndex(
    uint8_t nIndex,
    horizontal_aligment_t* phAlign,
    vertical_aligment_t* pvAlign);


#endif // !__BV_TERMINAL_H

 /* END OF FILE */

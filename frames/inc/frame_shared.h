/*
 * File     frame_shared.h
 * Date     30.01.2022
 */

#ifndef __FRAME_SHARED_H
#define __FRAME_SHARED_H


#include "../../core/inc/bv_types.h"
#include "../../core/inc/bv_frame.h"


/// <summary>
/// Стандартная функция обработки сообщений фрейма.
/// </summary>
/// <param name="eMsg">Сообщение.</param>
/// <param name="param">Параметр, зависит от сообщения.</param>
/// <param name="proc">Функция обработки сообщений фрейма.</param>
/// <returns>Результат, зависит от сообщения.</returns>
result_t BV_DefFrameProc(
    frame_message_t eMsg,
    param_t param,
    frame_proc_f proc);


#endif // !__FRAME_SHARED_H

/* END OF FILE */

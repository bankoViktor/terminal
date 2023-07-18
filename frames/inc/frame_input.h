/*
 * File     frame_input.h
 * Date     05.01.2022
 */

#ifndef __FRAME_INPUT_H
#define __FRAME_INPUT_H

#include "../../core/inc/bv_types.h"
#include "../../core/inc/bv_frame.h"


// Notification codes
#define IN_BASE                         100
#define IN_INIT                         (IN_BASE + 1)
#define IN_UPDATE                       (IN_BASE + 2)


#define INPUT_BUFFER_LENGTH_MAX         32

// Режимы редактирования
typedef enum input_frame_mode_t
{
    // Редактируемое значение - текст.
    IFM_TYPE_TEXT = 0x0001,
    // Редактируемое значение - целое без знаковое число.
    IFM_TYPE_INTEGER = 0x0002,
    // Редактируемое значение - целое знаковое число.
    IFM_TYPE_SIGNED_INTEGER = 0x0004,
    // Редактируемое значение - вещественное знаковое число.
    IFM_TYPE_DOUBLE = 0x0008,
} input_frame_mode_t;


// Данные уведомления IN_INIT
typedef struct init_notification_t
{
    // Стандартный заголовок уведомления.
    notification_header_t   hdr;

    // Адрес буфера в который нужно скопировать редактируемое значение.
    uint8_t*                szBuffer;
    
    // Максимальный размер буфера, инициализован значением 
    // максимального размера буффера фрейма.
    // Для IFM_TYPE_TEXT нужно установить максимальное количество символов редактируемого значения.
    uint32_t                nLengthMax;

    // Режимы редактирования.
    input_frame_mode_t      bmMode;

    // Адрес последовательности допустимых символов.
    uint8_t*                pszKeysMap;

    // Индекс кнопки фрейма источника уведомления.
    uint8_t                 nFromButtonIndex;
} init_notification_t;


// Данные уведомления IN_UPDATE
typedef struct update_notification_t
{
    // Стандартный заголовок уведомления.
    notification_header_t   hdr;

    // Адрес буфера который содержит отредактированное значение.
    uint8_t*                szBuffer;

    // Адрес строки сообщения в случае невалидного значения.
    uint8_t*                szMessage;
} update_notification_t;


result_t FrameInputProc(
    frame_message_t eMsg,
    param_t param);


#endif // !__FRAME_INPUT_H

/* END OF FILE */

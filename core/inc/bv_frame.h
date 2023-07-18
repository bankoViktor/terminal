/*
 * File     bv_frame.h
 * Date     28.12.2021
 */

#ifndef __BV_FRAME_H
#define __BV_FRAME_H


#include "bv_config.h"
#include "bv_point.h"
#include "bv_tools.h"


 /* Frame Messages */
typedef enum frame_message_t
{
    FM_NONE,
    FM_CREATE,
    FM_DESTROY,
    FM_ERASEBKGND,
    FM_PAINT,
    FM_NOTIFY,
    FM_BUTTONDOWN,
    FM_BUTTONUP,
} frame_message_t;


typedef result_t(*frame_proc_f)(frame_message_t nMsg, param_t param);


// Заголовок сообщения/уведомления
typedef struct notification_header_t
{
    frame_proc_f    fFromProc;      // Адрес функции-фрейма от которого пришло уведомление
    uint32_t        nCode;          // Код уведомления
    uint8_t         nButtonIndex;   // Индекс кнопки
} notification_header_t;


typedef enum button_type_t
{
    BT_NORMAL,
    BT_ACTION,
    BT_INPUT,
    BT_TOGGLE,
    BT_SELECTABLE,
    BT_UP,
    BT_DOWN,
    BT_LEFT,
    BT_RIGHT,
} button_type_t;


typedef struct button_t
{
    const uint8_t*  szTitle;
    button_type_t   type;
} button_t;


#endif // !__BV_FRAME_H

/* END OF FILE */

/*
 * File     frame_input.h
 * Date     05.01.2022
 */

#ifndef __FRAME_INPUT_H
#define __FRAME_INPUT_H

#include "bv_types.h"
#include "bv_frame.h"


#define IN_BASE                         100
#define IN_INIT                         (IN_BASE+1)
#define IN_UPDATE                       (IN_BASE+2)


#define INPUT_BUFFER_LENGTH             16


typedef enum input_frame_mode_t
{
    IFM_NUMBER,
    IFM_TEXT,
} input_frame_mode_t;


typedef enum input_frame_lang_t
{
    IFL_ALL,
    IFL_EN,
    IFL_RU,
} input_frame_lang_t;


typedef enum input_frame_case_t
{
    IFC_BOTH,
    IFC_LOWER,
    IFC_UPPER,
} input_frame_case_t;


/* Notification data for IN_INIT */
typedef struct init_notification_t
{
    notification_header_t   hdr;
    uint8_t*                szValue;
    uint32_t                nLengthMax;
    input_frame_mode_t      mode;
} init_notification_t;


/* Notification data for IN_UPDATE */
typedef struct update_notification_t
{
    notification_header_t   hdr;
    uint8_t*                szValue;
    uint8_t*                szMessage;
} update_notification_t;


extern result_t FrameInputProc(
    frame_message_t nMsg,
    param_t param);


#endif // !__FRAME_INPUT_H

/* END OF FILE */

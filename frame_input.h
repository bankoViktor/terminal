/*
 * File     frame_input.h
 * Date     05.01.2022
 */

#ifndef __FRAME_INPUT_H
#define __FRAME_INPUT_H

#include "bv_types.h"
#include "bv_frame.h"


#define IN_BASE             10
#define IN_INIT             (IN_BASE+1)
#define IN_UPDATE           (IN_BASE+2)


typedef struct init_notification_t
{
    notification_header_t   hdr;
    uint8_t*                szValue;
    uint32_t                nLengthMax;
} init_notification_t;


extern result_t FrameInputProc(
    frame_message_t nMsg,
    param_t param);


#endif // !__FRAME_INPUT_H

/* END OF FILE */

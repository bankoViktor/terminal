/*
 * File     bv_debug.h
 * Date     08.01.2022
 */

#ifndef __BV_DEBUG_H
#define __BV_DEBUG_H


#include "bv_types.h"
#include "bv_frame.h"


extern const uint8_t* GetMessageDescription(
    frame_message_t nMsg);

extern void DebugOutput(
    frame_proc_f proc,
    frame_message_t nMsg,
    uint32_t nNotificationCode);

extern void PrintFrameMessage(
    frame_proc_f proc,
    frame_message_t nMsg,
    param_t param);


#endif // !__BV_DEBUG_H


 /* END OF FILE */

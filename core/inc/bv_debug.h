/*
 * File     bv_debug.h
 * Date     08.01.2022
 */

#ifndef __BV_DEBUG_H
#define __BV_DEBUG_H


#include "bv_types.h"
#include "bv_frame.h"


const uint8_t* GetMessageDescription(frame_message_t eMsg);

void DebugOutput(
    frame_proc_f fProc,
    frame_message_t eMsg,
    uint32_t nNotificationCode);

void PrintFrameMessage(
    frame_proc_f fProc,
    frame_message_t eMsg,
    param_t param);


#endif // !__BV_DEBUG_H


 /* END OF FILE */

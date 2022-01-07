/*
 * File     frame_input.h
 * Date     05.01.2022
 */

#ifndef __FRAME_INPUT_H
#define __FRAME_INPUT_H

#include "bv_types.h"
#include "bv_frame.h"


typedef enum input_type_t
{
    IT_NUMBER,
    IT_TEXT,
} input_type_t;


extern result_t FrameInputProc(
    frame_message_t nMsg,
    param_t param);


#endif // !__FRAME_INPUT_H

/* END OF FILE */

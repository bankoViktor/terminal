/*
 * File     shared.h
 * Date     30.01.2022
 */

#ifndef __SHARED_H
#define __SHARED_H

#include "bv_types.h"
#include "bv_frame.h"


extern result_t DefFrameProc(
    frame_message_t nMsg,
    param_t param,
    frame_proc_f proc
);


#endif // !__SHARED_H

/* END OF FILE */

/*
 * File     bvg_frame.h
 * Date     28.12.2021
 */

#ifndef __BVG_FRAME_H
#define __BVG_FRAME_H

#include "bvg_types.h"

typedef void (*frame_proc_f)(uint16_t nMsg);

typedef struct frame_t
{
    const char*     szTitle;
    frame_proc_f    fFrmProc;
} frame_t;

void BVG_CreateFrame(
    frame_t* pfrm,
    void* pSomeParameter);

#endif // !__BVG_FRAME_H

/* END OF FILE */

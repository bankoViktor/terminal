/*
 * File     bvg_frame.c
 * Date     28.12.2021
 */

#include "bvg_frame.h"
#include <string.h>


void BVG_CreateFrame(frame_t* pfrm, void* pSomeParameter)
{
    memset(pfrm, 0, sizeof(frame_t));
    pfrm->szTitle = "Example";
}


/* END OF FILE */

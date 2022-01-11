/*
 * File     bv_keyboard.h
 * Date     09.01.2022
 */

#ifndef __BV_KEYBOARD_H
#define __BV_KEYBOARD_H

#include "bv_types.h"


typedef enum button_state_t
{
    BS_NONE,
    BS_DOWN,
    BS_UP,
} button_state_t;


extern void BVK_SetState(
    uint32_t nIndex,
    button_state_t nState);

extern void BVK_Clear(
    uint32_t nIndex);

extern button_state_t BVK_GetState(
    uint32_t nIndex);


#endif // !__BV_KEYBOARD_H

 /* END OF FILE */

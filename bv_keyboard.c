/*
 * File     bv_keyboard.h
 * Date     09.01.2022
 */

#include "bv_keyboard.h"
#include "bv_config.h"

#include <time.h>

typedef struct button_info_t
{
    uint32_t        nTime;
    button_state_t  nState;
} button_info_t;


static volatile button_info_t buttons_states[BUTTON_COUNT] = { 0 };


uint32_t GetTimeMs()
{
    clock_t uptime = clock() / (CLOCKS_PER_SEC / 1000);
    return (uint32_t)uptime;
}

void BVK_SetState(
    uint32_t nIndex,
    button_state_t nState)
{
    if (nIndex >= 0 && nIndex < BUTTON_COUNT)
    {
        buttons_states[nIndex].nTime = GetTimeMs();
        buttons_states[nIndex].nState = nState;
    }
}

void BVK_Clear(
    uint32_t nIndex)
{
    if (nIndex >= 0 && nIndex < BUTTON_COUNT)
    {
        buttons_states[nIndex].nTime = 0;
        buttons_states[nIndex].nState = BS_NONE;
    }
}

button_state_t BVK_GetState(
    uint32_t nIndex)
{
    return nIndex >= 0 && nIndex < BUTTON_COUNT 
        ? buttons_states[nIndex].nState 
        : BS_NONE;
}


 /* END OF FILE */

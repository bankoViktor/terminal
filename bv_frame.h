/*
 * File     bv_frame.h
 * Date     28.12.2021
 */

#ifndef __BV_FRAME_H
#define __BV_FRAME_H


#include "bv_types.h"
#include "bv_point.h"
#include "bv_config.h"


#define _MAKEDWORD(hw,lw)       ((uint32_t)(((uint16_t)((lw) & 0xffff)) | ((uint16_t)((hw) & 0xffff)) << 16))
#define _MAKEWORD(hb,lb)        ((uint16_t)(((uint8_t)((lb) & 0xff)) | ((uint8_t)((hb) & 0xff)) << 8))
#define _LOWORD(dw)             ((uint16_t)((dw) & 0xffff))
#define _HIWORD(dw)             ((uint16_t)(((dw) >> 16) & 0xffff))
#define _LOBYTE(w)              ((uint8_t)((w) & 0xff))
#define _HIBYTE(w)              ((uint8_t)(((w) >> 8) & 0xff))


/* Frame Messages */
#define FM_BASE                 ((uint16_t)(0x0000))
#define FM_PAINT                ((uint16_t)(FM_BASE + 1))
#define FM_BGERASE              ((uint16_t)(FM_BASE + 2))
#define FM_NOTIFICATION         ((uint16_t)(FM_BASE + 3))

/* Frame Notification Codes */
#define BN_BASE                 ((uint8_t)(0x00))
#define BN_UP                   ((uint8_t)(BN_BASE + 1))

#define NP_CLICK(buttonIndex)   _MAKEWORD(buttonIndex, BN_UP)


typedef enum button_type_t
{
    BT_NORMAL,
    BT_ACTION,
    BT_INPUT,
    BT_TOGGLE,
    BT_ACTIVE,
    BT_UP,
    BT_DOWN,
    BT_LEFT,
    BT_RIGHT,
} button_type_t;


typedef struct button_t
{
    const char* szTitle;
    button_type_t type;
} button_t;


#ifdef TERMINAL_DEBUG

extern void BVG_DrawClientRect();

extern void BVG_DrawButtonGrid();

#endif // !TERMINAL_DEBUG


extern void BVG_DrawMarker(
    const point_t* ppt,
    color_t color);

extern void BVG_DrawButtonLabel(
    uint8_t buttonIndex,
    const point_t* ppt,
    const char* szText);

extern void BVG_DrawButtonMarker(
    uint8_t buttonIndex,
    const point_t* ppt,
    button_type_t type);


#endif // !__BV_FRAME_H

/* END OF FILE */

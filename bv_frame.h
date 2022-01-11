/*
 * File     bv_frame.h
 * Date     28.12.2021
 */

#ifndef __BV_FRAME_H
#define __BV_FRAME_H


#include "bv_config.h"
#include "bv_point.h"
#include "bv_tools.h"


typedef enum frame_message_t
{
    FM_NONE,
    FM_CREATE,
    FM_DESTROY,
    FM_ERASEBKGND,
    FM_PAINT,
    FM_NOTIFY,
    FM_BUTTONDOWN,      // param     nButtonId
    FM_BUTTONUP,        // param     nButtonId
} frame_message_t;


typedef result_t(*frame_proc_f)(frame_message_t nMsg, param_t param);


typedef struct notification_header_t
{
    frame_proc_f            fFromProc;
    uint16_t                nCode;
} notification_header_t;


typedef enum button_type_t
{
    BT_NORMAL,
    BT_ACTION,
    BT_INPUT,
    BT_TOGGLE,
    BT_SELECTABLE,
    BT_UP,
    BT_DOWN,
    BT_LEFT,
    BT_RIGHT,
} button_type_t;


typedef struct button_t
{
    const uint8_t*  szTitle;
    button_type_t   nType;
} button_t;


#ifdef TERMINAL_DEBUG

extern void BVG_DrawClientRect();

extern void BVG_DrawButtonGrid();

extern void BVG_DrawMarker(
    const point_t* ppt,
    color_t color);

#endif // !TERMINAL_DEBUG


extern void BVG_GetAlignByIndex(
    uint8_t bIndex,
    horizontal_aligment_t* phAlign,
    vertical_aligment_t* pvAlign);

extern void BVG_DrawButtonText(
    uint8_t bIndex,
    coord_t nOffset,
    const uint8_t* szText,
    color_t foreColor,
    color_t backColor);

extern void BVG_DrawButtonMarker(
    uint8_t bIndex,
    const point_t* ppt,
    button_type_t nType);


#endif // !__BV_FRAME_H


/* END OF FILE */

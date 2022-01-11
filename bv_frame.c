/*
 * File     bv_frame.c
 * Date     28.12.2021
 */

#include "bv_frame.h"
#include "bv_hwdriver.h"
#include "bv_terminal.h"
#include <string.h>
#include "main.h"


#ifdef TERMINAL_DEBUG

void BVG_DrawClientRect()
{
    rect_t rc = { 0 };
    BVT_GetClientRect(&rc);
    BVG_DrawRect(&rc, 1, CLIENT_RECT_COLOR, TRANSPARENT_COLOR);
}

void BVG_DrawButtonGrid()
{
    rect_t rc = { 0 };
    BVT_GetClientRect(&rc);

    point_t pt = { 0 };
    point_t pt0 = { 0 };
    point_t pt1 = { 0 };

    for (uint8_t i = 0; i < BUTTON_COUNT_X; ++i)
    {
        BVT_CalcButtonPos(&pt, i, 0);
        pt0.x = pt.x;
        pt0.y = rc.top;
        pt1.x = pt.x;
        pt1.y = rc.bottom;
        BVG_DrawLine(&pt0, &pt1, 1, GRID_COLOR);
    }

    for (uint8_t i = BUTTON_COUNT_X; i < (BUTTON_COUNT_X + BUTTON_COUNT_Y); ++i)
    {
        BVT_CalcButtonPos(&pt, i, 0);
        pt0.x = rc.left;
        pt0.y = pt.y;
        pt1.x = rc.right;
        pt1.y = pt.y;
        BVG_DrawLine(&pt0, &pt1, 1, GRID_COLOR);
    }
}

void BVG_DrawMarker(
    const point_t* ppt,
    color_t color)
{
    point_t pt0 = { 0 };
    point_t pt1 = { 0 };

    pt0.x = ppt->x - MARKER_SIZE;
    pt0.y = ppt->y;
    pt1.x = ppt->x + MARKER_SIZE;
    pt1.y = ppt->y;
    BVG_DrawLine(&pt0, &pt1, 1, MARKER_COLOR);

    pt0.x = ppt->x;
    pt0.y = ppt->y - MARKER_SIZE;
    pt1.x = ppt->x;
    pt1.y = ppt->y + MARKER_SIZE;
    BVG_DrawLine(&pt0, &pt1, 1, MARKER_COLOR);
}

#endif // TERMINAL_DEBUG


void BVG_GetAlignByIndex(
    uint8_t nIndex,
    horizontal_aligment_t* phAlign,
    vertical_aligment_t* pvAlign)
{
    if (nIndex < BUTTONS_TOP)
    {
        // Top
        *phAlign = H_ALIGN_CENTER;
        *pvAlign = V_ALIGN_TOP;
    }
    else if (nIndex < BUTTONS_RIGHT)
    {
        // Right
        *phAlign = H_ALIGN_RIGHT;
        *pvAlign = V_ALIGN_MIDDLE;
    }
    else if (nIndex < BUTTONS_BOTTOM)
    {
        // Bottom
        *phAlign = H_ALIGN_CENTER;
        *pvAlign = V_ALIGN_BOTTOM;
    }
    else if (nIndex < BUTTONS_LEFT)
    {
        // Left
        *phAlign = H_ALIGN_LEFT;
        *pvAlign = V_ALIGN_MIDDLE;
    }
}

void BVG_DrawButtonText(
    uint8_t nIndex,
    coord_t nOffset,
    const uint8_t* szText,
    color_t foreColor,
    color_t backColor)
{
    // Get button text aligment
    horizontal_aligment_t hAlign;
    vertical_aligment_t vAlign;
    BVG_GetAlignByIndex(nIndex, &hAlign, &vAlign);

    // Calc button text rectangle
    rect_t rc = { 0 };
    BVG_CalcText(&rc, szText, hAlign, vAlign);

    // Calc button position
    point_t pt = { 0 };
    BVT_CalcButtonPos(&pt, nIndex, nOffset);

    // Offset text rectangle to button position
    BVT_OffsetButton(&rc, &pt, nIndex);

    // Drawing
    BVG_DrawText(&rc, szText, (uint16_t)strlen(szText), foreColor, backColor, hAlign, vAlign);
}

void BVG_DrawButtonMarker(
    uint8_t nIndex,
    const point_t* ppt,
    button_type_t nType)
{
    const char* szText = 0;

    switch (nType)
    {
    case BT_ACTION:
        break;
    case BT_INPUT:
        szText = "[ ]";
        break;
    case BT_TOGGLE:
        szText = " T ";
        break;
    case BT_UP:
        szText = " U ";
        break;
    case BT_DOWN:
        szText = " D ";
        break;
    case BT_LEFT:
        szText = " L ";
        break;
    case BT_RIGHT:
        szText = " R ";
        break;
    }

    //if (szText)
    //    BVG_DrawButtonText(buttonIndex, ppt, szText);
}


/* END OF FILE */

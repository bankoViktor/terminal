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
    uint8_t bIndex,
    horizontal_aligment_t* phAlign,
    vertical_aligment_t* pvAlign)
{
    if (bIndex < BUTTONS_TOP)
    {
        // Top
        *phAlign = H_ALIGN_CENTER;
        *pvAlign = V_ALIGN_TOP;
    }
    else if (bIndex < BUTTONS_RIGHT)
    {
        // Right
        *phAlign = H_ALIGN_RIGHT;
        *pvAlign = V_ALIGN_MIDDLE;
    }
    else if (bIndex < BUTTONS_BOTTOM)
    {
        // Bottom
        *phAlign = H_ALIGN_CENTER;
        *pvAlign = V_ALIGN_BOTTOM;
    }
    else if (bIndex < BUTTONS_LEFT)
    {
        // Left
        *phAlign = H_ALIGN_LEFT;
        *pvAlign = V_ALIGN_MIDDLE;
    }
}

void BVG_OffsetButton(
    rect_t* prc,
    const point_t* ppt,
    uint8_t bIndex)
{
    coord_t dx = 0;
    coord_t dy = 0;

    if (bIndex < BUTTONS_TOP)
    {
        // Top
        dx = -RECT_GetWidth(prc) / 2;
        dy = 0;
    }
    else if (bIndex < BUTTONS_RIGHT)
    {
        // Right
        dx = -RECT_GetWidth(prc) + 1;
        dy = -RECT_GetHeight(prc) / 2;
    }
    else if (bIndex < BUTTONS_BOTTOM)
    {
        // Bottom
        dx = -RECT_GetWidth(prc) / 2;
        dy = -RECT_GetHeight(prc) + 1;
    }
    else if (bIndex < BUTTONS_LEFT)
    {
        // Left
        dx = 0;
        dy = -RECT_GetHeight(prc) / 2;
    }

    RECT_Offset(prc, ppt->x + dx, ppt->y + dy);
}

void BVG_DrawButtonText(
    uint8_t bIndex,
    coord_t nOffset,
    const uint8_t* szText,
    color_t foreColor,
    color_t backColor)
{
    // Get button text aligment
    horizontal_aligment_t hAlign;
    vertical_aligment_t vAlign;
    BVG_GetAlignByIndex(bIndex, &hAlign, &vAlign);

    // Calc button text rectangle
    rect_t rc = { 0 };
    BVG_CalcText(&rc, szText, hAlign, vAlign);

    // Calc button position
    point_t pt = { 0 };
    BVT_CalcButtonPos(&pt, bIndex, nOffset);

    // Offset text rectangle to button position
    BVG_OffsetButton(&rc, &pt, bIndex);

    // Drawing
    BVG_DrawText(&rc, szText, (uint16_t)strlen(szText), foreColor, backColor, hAlign, vAlign);
}

void BVG_DrawButtonMarker(
    uint8_t bIndex,
    const point_t* ppt,
    button_type_t type)
{
    const char* szText = 0;

    switch (type)
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

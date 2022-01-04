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

    pt0.x = ppt->x - BUTTON_MARKER_SIZE;
    pt0.y = ppt->y;
    pt1.x = ppt->x + BUTTON_MARKER_SIZE;
    pt1.y = ppt->y;
    BVG_DrawLine(&pt0, &pt1, 1, MARKER_COLOR);

    pt0.x = ppt->x;
    pt0.y = ppt->y - BUTTON_MARKER_SIZE;
    pt1.x = ppt->x;
    pt1.y = ppt->y + BUTTON_MARKER_SIZE;
    BVG_DrawLine(&pt0, &pt1, 1, MARKER_COLOR);
}

#endif // TERMINAL_DEBUG

void BVG_GetAlignByIndex(
    uint8_t buttonIndex,
    horizontal_aligment_t* phAlign,
    vertical_aligment_t* pvAlign)
{
    if (buttonIndex < BUTTONS_TOP)
    {
        // Top
        *phAlign = H_ALIGN_CENTER;
        *pvAlign = V_ALIGN_TOP;
    }
    else if (buttonIndex < BUTTONS_RIGHT)
    {
        // Right
        *phAlign = H_ALIGN_RIGHT;
        *pvAlign = V_ALIGN_MIDDLE;
    }
    else if (buttonIndex < BUTTONS_BOTTOM)
    {
        // Bottom
        *phAlign = H_ALIGN_CENTER;
        *pvAlign = V_ALIGN_BOTTOM;
    }
    else if (buttonIndex < BUTTONS_LEFT)
    {
        // Left
        *phAlign = H_ALIGN_LEFT;
        *pvAlign = V_ALIGN_MIDDLE;
    }
}

void BVG_OffsetButton(
    rect_t* prc,
    const point_t* ppt,
    uint8_t buttonIndex)
{
    coord_t dx = 0;
    coord_t dy = 0;

    if (buttonIndex < BUTTONS_TOP)
    {
        // Top
        dx = -RECT_GetWidth(prc) / 2;
        dy = 0;
    }
    else if (buttonIndex < BUTTONS_RIGHT)
    {
        // Right
        dx = -RECT_GetWidth(prc) + 1;
        dy = -RECT_GetHeight(prc) / 2;
    }
    else if (buttonIndex < BUTTONS_BOTTOM)
    {
        // Bottom
        dx = -RECT_GetWidth(prc) / 2;
        dy = -RECT_GetHeight(prc) + 1;
    }
    else if (buttonIndex < BUTTONS_LEFT)
    {
        // Left
        dx = 0;
        dy = -RECT_GetHeight(prc) / 2;
    }

    RECT_Offset(prc, ppt->x + dx, ppt->y + dy);
}

void BVG_DrawButtonText(
    uint8_t index,
    coord_t offset,
    const char* szText,
    color_t clrForeground,
    color_t clrBackground)
{
    // Get button text aligment
    horizontal_aligment_t hAlign;
    vertical_aligment_t vAlign;
    BVG_GetAlignByIndex(index, &hAlign, &vAlign);

    // Calc button text rectangle
    rect_t rc = { 0 };
    BVG_CalcText(&rc, szText, hAlign, vAlign);

    // Calc button position
    point_t pt = { 0 };
    BVT_CalcButtonPos(&pt, index, offset);

    // Offset text rectangle to button position
    BVG_OffsetButton(&rc, &pt, index);

    // Drawing
    BVG_DrawText(&rc, szText, clrForeground, clrBackground, hAlign, vAlign);
}

void BVG_DrawButtonMarker(
    uint8_t buttonIndex,
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

void BVP_DrawTriangle(
    const rect_t* prc,
    triangle_orientation_t orientation)
{
    point_t pt[3] = { 0 };

    coord_t hx = RECT_GetWidth(prc) / 2;
    coord_t hy = RECT_GetHeight(prc) / 2;

    switch (orientation)
    {
    case TO_LEFT:
        pt[0].x = prc->left;
        pt[0].y = prc->top + hy;
        pt[1].x = prc->right;
        pt[1].y = prc->top;
        pt[2].x = prc->right;
        pt[2].y = prc->bottom;
        break;
    case TO_UP:
        pt[0].x = prc->left + hx;
        pt[0].y = prc->top;
        pt[1].x = prc->right;
        pt[1].y = prc->bottom;
        pt[2].x = prc->left;
        pt[2].y = prc->bottom;
        break;
    case TO_RIGHT:
        pt[0].x = prc->right;
        pt[0].y = prc->top + hy;
        pt[1].x = prc->left;
        pt[1].y = prc->bottom;
        pt[2].x = prc->left;
        pt[2].y = prc->top;
        break;
    case TO_DOWN:
        pt[0].x = prc->left + hx;
        pt[0].y = prc->bottom;
        pt[1].x = prc->left;
        pt[1].y = prc->top;
        pt[2].x = prc->right;
        pt[2].y = prc->top;
        break;
    }
    
    BVG_Polygon(pt, 3, 1, TEXT_COLOR, TEXT_COLOR);
}


/* END OF FILE */

/*
 * File     bv_primitives.c
 * Date     04.01.2022
 */

#include "bv_primitives.h"
#include "bv_hwdriver.h"
#include "bv_config.h"
#include "bv_terminal.h"
#include "frame_input.h"

#include <string.h>


#define INPUT_TEMP_BUFFER_LENGHT        (INPUT_BUFFER_LENGTH + 2)
#define ONE_CHAR                        " "


void BVP_DrawDirectionSymbol(
    const rect_t* prc,
    triangle_orientation_t orientation,
    color_t color)
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

    BVG_Polygon(pt, 3, 1, color, color);
}

void BVP_DrawInput(
    const uint8_t* szValue,
    uint16_t nLengthMax,
    uint16_t nCursorPos)
{
    const uint8_t* szPrefix = "[";
    const uint8_t* zsPostfix = "]";
    const uint8_t chPlaceholder = ' ';

    uint8_t nPrefixLen = (uint8_t)strlen(szPrefix);
    uint8_t nPostfixLen = (uint8_t)strlen(zsPostfix);
    uint8_t nLen = nLengthMax + nPrefixLen + nPostfixLen;
    uint8_t nPos = nCursorPos + nPrefixLen;
    uint8_t szBuffer[INPUT_TEMP_BUFFER_LENGHT + 1] = { 0 };

    strcat_s(szBuffer, INPUT_TEMP_BUFFER_LENGHT + 1, szPrefix);
    strcat_s(szBuffer, INPUT_TEMP_BUFFER_LENGHT + 1, szValue);

    size_t nValueLen = strlen(szValue);
    for (uint8_t i = 0; i < nLengthMax - nValueLen; i++)
        szBuffer[nPrefixLen + nValueLen + i] = chPlaceholder;
    szBuffer[nPrefixLen + nLengthMax] = '\0';

    strcat_s(szBuffer, INPUT_TEMP_BUFFER_LENGHT + 1, zsPostfix);

    rect_t rc = { 0 };
    BVT_GetRect(&rc);

    // Char Rect
    rect_t rcText = { 0 };
    BVG_CalcText(&rcText, ONE_CHAR, H_ALIGN_LEFT, V_ALIGN_TOP);
    RECT_Offset(&rcText, 0, (coord_t)(TERMINAL_HEIGHT * 0.30));
    coord_t charWidth = RECT_GetWidth(&rcText);
    coord_t left = rc.left + (TERMINAL_WIDTH - charWidth * (nLen - 1)) / 2;

    for (uint8_t j = 0; j < nLen;)
    {
        color_t fore = j == nPos ? TEXT_BGCOLOR : TEXT_COLOR;
        color_t back = j == nPos ? TEXT_COLOR : TEXT_BGCOLOR;
        uint8_t count = 1;

        if (j < nPos)
            count = nPos == 0 ? 0 : nPos;               // left
        else if (j > nPos)
            count = nPos < nLen ? nLen - nPos - 1 : 0;  // right

        if (count)
        {
            rcText.left = left + charWidth * j;
            rcText.right = rcText.left + charWidth * count;
            BVG_DrawText(&rcText, szBuffer + j, count, fore, back, H_ALIGN_LEFT, V_ALIGN_TOP);
        }

        j += count;
    }
}

void BVP_DrawMessage(
    const uint8_t* szMessage,
    color_t foreColor,
    color_t backColor)
{
    const coord_t topOffset = (coord_t)(TERMINAL_HEIGHT * 0.5);
    const coord_t height = 60;
    const coord_t sideOffset = 80;

    // Background
    rect_t rc = { 0 };
    BVT_GetRect(&rc);

    rc.top = topOffset;
    rc.bottom = rc.top + height;
    RECT_Inflate(&rc, -sideOffset, 0);

    BVG_DrawFill(&rc, backColor);

    // Draw Message
    horizontal_aligment_t hAlign = H_ALIGN_CENTER;
    vertical_aligment_t vAlign = V_ALIGN_TOP;
    BVG_CalcText(&rc, szMessage, hAlign, vAlign);

    RECT_Offset(&rc,
        RECT_GetWidth(&rc) / 2,
        (height - RECT_GetHeight(&rc)) / 2);

    BVG_DrawText(&rc, szMessage, (uint16_t)strlen(szMessage),
        foreColor, backColor, hAlign, vAlign);
}

void BVP_DrawErrorMessage(
    const uint8_t* szMessage)
{
    BVP_DrawMessage(szMessage, MSG_ERROR_FORE_COLOR, MSG_ERROR_BACK_COLOR);
}

void BVP_DrawClientRect()
{
    rect_t rcClient = { 0 };
    BVT_GetClientRect(&rcClient);

    RECT_Inflate(&rcClient, 1, 1);

    BVG_DrawRect(&rcClient, 1, CLIENT_RECT_COLOR, TRANSPARENT_COLOR);
}

void BVP_DrawButtonGrid()
{
    rect_t rc = { 0 };
    BVT_GetRect(&rc);

    point_t pt = { 0 };
    point_t pt0 = { 0 };
    point_t pt1 = { 0 };

    for (uint8_t i = 0; i < BUTTON_COUNT_X; ++i)
    {
        BVT_GetButtonPos(&pt, i, 0);
        pt0.x = pt.x;
        pt0.y = rc.top;
        pt1.x = pt.x;
        pt1.y = rc.bottom;
        BVG_DrawLine(&pt0, &pt1, 1, GRID_COLOR);
    }

    for (uint8_t i = BUTTON_COUNT_X; i < (BUTTON_COUNT_X + BUTTON_COUNT_Y); ++i)
    {
        BVT_GetButtonPos(&pt, i, 0);
        pt0.x = rc.left;
        pt0.y = pt.y;
        pt1.x = rc.right;
        pt1.y = pt.y;
        BVG_DrawLine(&pt0, &pt1, 1, GRID_COLOR);
    }
}

void BVP_DrawMarker(
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

void BVP_DrawButtonText(
    uint8_t nIndex,
    coord_t nOffset,
    const uint8_t* szText,
    color_t foreColor,
    color_t backColor)
{
    // Get button text aligment
    horizontal_aligment_t hAlign;
    vertical_aligment_t vAlign;
    BVT_GetAlignByIndex(nIndex, &hAlign, &vAlign);

    // Calc button text rectangle
    rect_t rc = { 0 };
    BVG_CalcText(&rc, szText, hAlign, vAlign);

    // Calc button position
    point_t pt = { 0 };
    BVT_GetButtonPos(&pt, nIndex, SAFE_OFFSET + nOffset);

    // Offset text rectangle to button position
    BVT_OffsetByButton(&rc, &pt, nIndex);

    // Drawing
    BVG_DrawText(&rc, szText, (uint16_t)strlen(szText), foreColor, backColor, hAlign, vAlign);
}

void BVP_DrawButtonMarker(
    uint8_t nIndex,
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
    //    BVP_DrawButtonText(buttonIndex, ppt, szText);
}

void BVP_DrawDSMS(
    const rect_t* prc)
{
    coord_t nWidth = 85;
    coord_t nHeight = 40;
    coord_t nOffset = 50;

    for (uint8_t i = BUTTONS_TOP; i < BUTTONS_LEFT; i++)
    {
        coord_t nSideOffset = i < BUTTONS_BOTTOM ? nOffset + nWidth - 2 : nOffset;

        point_t pt = { 0 };
        BVT_GetButtonPos(&pt, i, nSideOffset);

        rect_t rc = { 0 };
        RECT_SetWithSize(&rc, pt.x, pt.y - (coord_t)(nHeight / 2), nWidth, nHeight);

        BVG_DrawRect(&rc, 2, TEXT_COLOR, TRANSPARENT_COLOR);

        if (i == BUTTONS_RIGHT - 1)
            i = BUTTONS_BOTTOM - 1;
    }
}

/* END OF FILE */

/*
 * File     bv_primitives.c
 * Date     04.01.2022
 */

#include "bv_primitives.h"
#include "bv_hwdriver.h"
#include "bv_config.h"
#include "bv_terminal.h"

#include <string.h>
#include <stdio.h>


#define INPUT_TEMP_BUFFER_LENGHT        (INPUT_BUFFER_LENGTH + 4)


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
    const uint8_t* szPrefix = "[ ";
    const uint8_t* zsPostfix = " ]";
    const char chPlaceholder = ' ';

    uint8_t nPrefixLen = (uint8_t)strlen(szPrefix);
    uint8_t nPostfixLen = (uint8_t)strlen(zsPostfix);
    uint8_t nLen = nLengthMax + nPrefixLen + nPostfixLen;
    uint8_t nPos = nCursorPos + nPrefixLen;
    uint8_t szBuffer[INPUT_TEMP_BUFFER_LENGHT + 1] = { 0 };

    strcat_s(szBuffer, INPUT_TEMP_BUFFER_LENGHT, szPrefix);
    strcat_s(szBuffer, INPUT_TEMP_BUFFER_LENGHT, szValue);

    size_t nValueLen = strlen(szValue);
    for (uint8_t i = 0; i < nLengthMax - nValueLen; i++)
        szBuffer[nPrefixLen + nValueLen + i] = chPlaceholder;
    szBuffer[nPrefixLen + nLengthMax] = '\0';
    
    strcat_s(szBuffer, INPUT_TEMP_BUFFER_LENGHT, zsPostfix);

    rect_t rc = { 0 };
    BVT_GetClientRect(&rc);

    // Char Rect
    rect_t rcText = { 0 };
    BVG_CalcText(&rcText, " ", H_ALIGN_LEFT, V_ALIGN_TOP);
    RECT_Offset(&rcText, 0, (coord_t)(TERMINAL_HEIGHT * 0.30));
    coord_t charWidth = RECT_GetWidth(&rcText);
    coord_t left = rc.left + (TERMINAL_WIDTH - charWidth * (nLen - 1)) / 2;

    for (uint8_t j = 0; j < nLen;)
    {
        color_t fore = j == nPos ? TEXT_BGCOLOR : TEXT_COLOR;
        color_t back = j == nPos ? TEXT_COLOR : TEXT_BGCOLOR;
        uint8_t count = 1;

        if (j < nPos)
            count = nPos == 0 ? 0 : nPos;           // left
        else if (j > nPos)
            count = nPos < nLen ? nLen - nPos - 1 : 0;  // right

        if (count)
        {
            rcText.left = left + charWidth * j;
            rcText.right = rcText.left + charWidth * count;
            BVG_DrawText(&rcText , szBuffer + j, count, fore, back, H_ALIGN_LEFT, V_ALIGN_TOP);
        }

        j += count;
    }
}

void BVP_DrawMessage(
    uint8_t* szMessage,
    color_t foreColor,
    color_t backColor)
{

}


 /* END OF FILE */

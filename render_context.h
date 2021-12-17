#pragma once

#include "framework.h"


enum HorizontalAligment
{
    Left,
    Center,
    Right
};


enum VerticalAligment
{
    Top,
    Middle,
    Bottom
};


struct render_context_t
{
    using moveTo_t = void(*)(int x, int y);
    using lineTo_t = void(*)(int x, int y, int color);
    using rect_t = void(*)(int x, int y, int cx, int cy, int color);
    using calcText_t = void(*)(RECT* pRC, const char* text, HorizontalAligment horizAlign, VerticalAligment vertAlign);
    using text_t = void(*)(RECT* pRC, const char* text, int color, int bgColor, HorizontalAligment horizAlign, VerticalAligment vertAlign);

    int width;
    int height;

    moveTo_t moveTo;
    lineTo_t lineTo;
    rect_t rect;
    calcText_t calcText;
    text_t text;
};

void initRenderContext(render_context_t& rc);

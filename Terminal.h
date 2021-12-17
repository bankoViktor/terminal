#pragma once

#include "resource.h"
#include "framework.h"
#include "render_context.h"
#include "TerminalPage.h"


const int buttonSize = 30;
const int buttonOffset = 50;


#define RECT_WIDTH(rc)          ((rc).right  - (rc).left)
#define RECT_HEIGHT(rc)         ((rc).bottom - (rc).top)



const int defSize = 480;
const int offset = 15;
const int gridColor = RGB(100, 0, 100);
const int color = RGB(0, 180, 0);
const int bgColor = RGB(50, 50, 50);
const int btnTitleLen = 16;
const int fontOffset = 5;



class Terminal
{
public:
    render_context_t m_context;

    void Init(const render_context_t& context);
    void DrawButtonGrid() const;
    void Render() const;
    void DrawButtons(const TerminalPage& page) const;
    void GetButtonPos(int index, POINT* ppt) const;
    void Active(int index);
};

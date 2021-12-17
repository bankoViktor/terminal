#include "pages.h"
#include "framework.h"

/* PAGE 1---------------------------------------------------------------- */

static const button_t page1_buttons[BUTTON_COUNT] = {
    // top
    { "RTN" },
    { "A-G" },
    { "STBY" },
    { "A-A" },
    { "TST" },
    // right
    { "SLEW\n5.0" },
    { "OSB 07" },
    { "LATCH\nOFF" },
    { "OSB 09" },
    { "OSB 10" },
    // bottom
    { "DCLT" },
    { "MSG" },
    { "CDU" },
    { "MAV" },
    { "TGP" },
    // left
    { "ON&OFF" },
    { "OSB 17" },
    { "LSS\n1688" },
    { "OSB 19" },
    { "INT\nCOLD" },
};

Page1::Page1() : TerminalPage(page1_buttons)
{
    m_pszTitle = "PAGE\nONE";
}

void Page1::Render() const
{
    RECT rc = { 0 };

    /*renderContext.calcText(&rc, this->pszTitle, NULL);

    auto x = renderContext.width / 2;
    auto y = (int)(renderContext.height * .25);

    renderContext.text(&rc, this->pszTitle, color, bgColor, DT_CENTER);*/
}

void Page1::Active(const button_t *pSender, void* param) const
{

}

/* PAGE 2 ---------------------------------------------------------------- */

static const button_t page2_buttons[BUTTON_COUNT] = {
    // top
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    // right
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    // bottom
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    // left
    NULL,
    NULL,
    NULL,
    NULL,
    { "BACK" },
};

Page2::Page2() : TerminalPage(page2_buttons)
{
    m_pszTitle = "PAGE\nTWO";
}

void Page2::Render() const
{
}

void Page2::Active(const button_t* pSender, void* param) const
{
}

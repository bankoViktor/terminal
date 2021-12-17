#include "DTSUploadPage.h"
#include "framework.h"



DTSUploadPage::DTSUploadPage() : TerminalPage()
{
    int index = 0;

    // top
    m_buttons[index++].pszTitle = "OSB 00";
    m_buttons[index++].pszTitle = "OSB 01";
    m_buttons[index++].pszTitle = "OSB 02";
    m_buttons[index++].pszTitle = "OSB 03";
    m_buttons[index++].pszTitle = "OSB 04";
    // right
    m_buttons[index++].pszTitle = "OSB 05";
    m_buttons[index++].pszTitle = "OSB 06";
    m_buttons[index++].pszTitle = "OSB 07";
    m_buttons[index++].pszTitle = "OSB 08";
    m_buttons[index++].pszTitle = "LOAD\nALL";
    // bottom
    m_buttons[index++].pszTitle = "OSB 10";
    m_buttons[index++].pszTitle = "LOAD";
    m_buttons[index++].pszTitle = "DSMS";
    m_buttons[index++].pszTitle = "TAD";
    m_buttons[index++].pszTitle = "RGP";
    // right
    m_buttons[index++].pszTitle = "LOAD\nPAGE";
    m_buttons[index++].pszTitle = "LOAD\nTGP";
    m_buttons[index++].pszTitle = "LOAD\nDSMS";
    m_buttons[index++].pszTitle = "LOAD\nTAD";
    m_buttons[index++].pszTitle = "OSB 19";

    pszTitle = "DTS\nPAGE";
}


void DTSUploadPage::Render() const
{
    RECT rc = { 0 };

    /*renderContext.calcText(&rc, this->pszTitle, NULL);

    auto x = renderContext.width / 2;
    auto y = (int)(renderContext.height * .25);

    renderContext.text(&rc, this->pszTitle, color, bgColor, DT_CENTER);*/
}


void DTSUploadPage::Active(const button_t sender, void* param) const
{

}

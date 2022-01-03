/*
 * File     bvg_frame_home.c
 * Date     29.12.2021
 */

#include "frame_tab1.h"
#include "bvg_frame.h"
#include "bvg_config.h"
#include "bvg_terminal.h"


static const button_t g_frame_tab1_buttons[BUTTON_COUNT] = {
    // top
    { 0 },
    { "A-G" },
    { "STBY" },
    { "A-A" },
    { "TST" },
    // right
    { "SLEW\n%4.1f" },
    { "OSB 07" },
    { "LATCH\n%s" },
    { "OSB 09" },
    { 0 },
    // bottom
    { "DCLT" },
    { "MSG" },
    { "CDU" },
    { "MAV" },
    { "TGP" },
    // left
    { "PAGE2" },
    { 0 },
    { "LSS\n % 4i" },
    { "OSB 19" },
    { "INT\nCOLD" },
};


void FrameTab1Proc(uint16_t nMsg, uint32_t param)
{
    switch (nMsg)
    {

    case FM_PAINT:
    {
        point_t pt = { 0 };

        for (uint8_t i = 0; i < BUTTON_COUNT; i++)
        {
            // Marker
            BVT_CalcButtonPos(&pt, i, 5);
            BVG_DrawButtonMarker(i, &pt, BT_NORMAL);

            // Label
            BVT_CalcButtonPos(&pt, i, 25);
            BVG_DrawButtonLabel(i, &pt, "OSB\nXXX");
        }

        break;
    }

    case FM_NOTIFICATION:
    {
        uint8_t buttonIndex = _HIBYTE(param);
        uint8_t notificationCode = _LOBYTE(param);
        switch (notificationCode)
        {
        case BN_UP:
            break;
        }
        break;
    }

    }
}


/* END OF FILE */

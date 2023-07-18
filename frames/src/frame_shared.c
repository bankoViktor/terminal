/*
 * File     frame_shared.c
 * Date     30.01.2022
 */


#include "frame_shared.h"
#include "../../core/inc/bv_terminal.h"
#include "../../core/inc/bv_primitives.h"

#include "frame_tab1.h"
#include "frame_tab2.h"
#include "frame_dsms.h"


#define BUTTON_TAB_1        (BUTTONS_BOTTOM_OFFSET + 4)
#define BUTTON_TAB_2        (BUTTONS_BOTTOM_OFFSET + 3)
#define BUTTON_TAB_3        (BUTTONS_BOTTOM_OFFSET + 2)
#define BUTTON_TAB_4        (BUTTONS_BOTTOM_OFFSET + 1)
#define BUTTON_TAB_5        (BUTTONS_BOTTOM_OFFSET + 0)

static void OnPaint(frame_proc_f proc)
{
    for (uint8_t nButtonIndex = BUTTONS_BOTTOM_OFFSET; nButtonIndex < BUTTONS_LEFT_OFFSET; nButtonIndex++)
    {
        color_t fore = TEXT_COLOR;
        color_t back = TEXT_BGCOLOR;
        const uint8_t* szLabel = _NULL;

        switch (nButtonIndex)
        {

        case BUTTON_TAB_1:
            szLabel = "ONE\nTAB";
            if (proc == FrameTab1Proc)
            {
                fore = TEXT_BGCOLOR;
                back = TEXT_COLOR;
            }
            break;

        case BUTTON_TAB_2:
            szLabel = "TWO\nTAB";
            if (proc == FrameTab2Proc)
            {
                fore = TEXT_BGCOLOR;
                back = TEXT_COLOR;
            }
            break;

        case BUTTON_TAB_3:
            szLabel = "THREE\nTAB";
            if (proc == FrameDSMSProc)
            {
                fore = TEXT_BGCOLOR;
                back = TEXT_COLOR;
            }
            break;

        case BUTTON_TAB_4:
            break;

        case BUTTON_TAB_5:
            break;

        }

        if (szLabel)
            BVP_DrawButtonText(nButtonIndex, BUTTON_LABEL_OFFSET, szLabel, fore, back);
    }
}


static void OnButtonUp(uint8_t nButtonIndex)
{
    switch (nButtonIndex)
    {

    case BUTTON_TAB_1:
        BVT_ChangeFrame(FrameTab1Proc);
        break;

    case BUTTON_TAB_2:
        BVT_ChangeFrame(FrameTab2Proc);
        break;

    case BUTTON_TAB_3:
        BVT_ChangeFrame(FrameDSMSProc);
        break;

    case BUTTON_TAB_4:
        break;

    case BUTTON_TAB_5:
        break;

    }

    BVT_InvalidateRect(_NULL, _TRUE);
}


result_t BV_DefFrameProc(
    frame_message_t eMsg,
    param_t param,
    frame_proc_f proc)
{
    result_t result = _NULL;

    switch (eMsg)
    {

    case FM_PAINT:
        OnPaint(proc);
        break;

    case FM_BUTTONUP:
        OnButtonUp((uint8_t)param);
        break;

    }

    return result;
}


 /* END OF FILE */

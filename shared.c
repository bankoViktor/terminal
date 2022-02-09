/*
 * File     shared.c
 * Date     30.01.2022
 */


#include "shared.h"
#include "bv_terminal.h"
#include "bv_primitives.h"

#include "frame_tab1.h"
#include "frame_tab2.h"


#define BUTTON_TAB_1        (BUTTONS_BOTTOM - 1)
#define BUTTON_TAB_2        (BUTTONS_BOTTOM - 2)
#define BUTTON_TAB_3        (BUTTONS_BOTTOM - 3)
#define BUTTON_TAB_4        (BUTTONS_BOTTOM - 4)
#define BUTTON_TAB_5        (BUTTONS_BOTTOM - 5)


result_t DefFrameProc(
    frame_message_t nMsg,
    param_t param,
    frame_proc_f proc)
{
    result_t result = _NULL;

    switch (nMsg)
    {

    case FM_PAINT:
    {
        for (uint8_t nButtonIndex = BUTTONS_RIGHT; nButtonIndex < BUTTONS_LEFT; nButtonIndex++)
        {
            color_t fore = TEXT_COLOR;
            color_t back = TEXT_BGCOLOR;
            const uint8_t* szLabel = _NULL;

            switch (nButtonIndex)
            {

            case BUTTON_TAB_1:
                szLabel = "ONE";
                if (proc == FrameTab1Proc)
                {
                    fore = TEXT_BGCOLOR;
                    back = TEXT_COLOR;
                }
                break;

            case BUTTON_TAB_2:
                szLabel = "TWO";
                if (proc == FrameTab2Proc)
                {
                    fore = TEXT_BGCOLOR;
                    back = TEXT_COLOR;
                }
                break;

            case BUTTON_TAB_3:
                break;

            case BUTTON_TAB_4:
                break;

            case BUTTON_TAB_5:
                break;

            }

            if (szLabel)
                BVP_DrawButtonText(nButtonIndex, BUTTON_LABEL_OFFSET, szLabel, fore, back);
        }
        break;
    }

    case FM_BUTTONUP:
    {
        uint8_t nButtonIndex = (uint8_t)param;
        switch (nButtonIndex)
        {

        case BUTTON_TAB_1:
            BVT_ChangeFrame(FrameTab1Proc);
            break;

        case BUTTON_TAB_2:
            BVT_ChangeFrame(FrameTab2Proc);
            break;

        case BUTTON_TAB_3:
            break;

        case BUTTON_TAB_4:
            break;

        case BUTTON_TAB_5:
            break;

        }

        BVT_InvalidateRect(_NULL, _TRUE);
        break;
    }

    }

    return result;
}


 /* END OF FILE */

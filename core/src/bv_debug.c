/*
 * File     bv_debug.c
 * Date     08.01.2022
 */

#include "../inc/bv_debug.h"
#include "../inc/bv_frame.h"

#include <stdio.h>


#ifdef _WINDOWS
#include <Windows.h>
#endif


#define BUFFER_LENGTH       1023


typedef struct msg_info_t
{
    frame_message_t nMsg;
    const uint8_t* szName;
} msg_info_t;


static const msg_info_t msgTexts[] = {
    { FM_NONE, "FM_NONE" },
    { FM_CREATE, "FM_CREATE" },
    { FM_DESTROY, "FM_DESTROY" },
    { FM_ERASEBKGND, "FM_ERASEBKGND" },
    { FM_PAINT, "FM_PAINT" },
    { FM_NOTIFY, "FM_NOTIFY" },
    { FM_BUTTONDOWN, "FM_BUTTONDOWN" },
    { FM_BUTTONUP, "FM_BUTTONUP" },
};


static uint8_t g_szBuffer[BUFFER_LENGTH + 1] = { 0 };


const uint8_t* GetMessageDescription(
    frame_message_t nMsg)
{
    for (uint8_t i = 0; i < (sizeof(msgTexts) / sizeof(msgTexts[0])); i++)
    {
        if (msgTexts[i].nMsg == nMsg)
        {
            return msgTexts[i].szName;
        }
    }
    return _NULL;
}

void DebugOutput(
    frame_proc_f fProc,
    frame_message_t eMsg,
    uint32_t nNotificationCode)
{
    uint8_t szBuffer[BUFFER_LENGTH + 1] = { 0 };

    sprintf_s(szBuffer, BUFFER_LENGTH, "0x%8p %s (%i) - %s %i\n",
        (void*)fProc, GetMessageDescription(eMsg), eMsg, "", nNotificationCode);

#ifdef _WINDOWS
    OutputDebugStringA(szBuffer);
#endif
}

void PrintFrameMessage(
    frame_proc_f fProc,
    frame_message_t eMsg,
    param_t param)
{
    uint32_t chars = sprintf_s(g_szBuffer, BUFFER_LENGTH,
        "0x%8p %-20s", fProc, GetMessageDescription(eMsg));

    switch (eMsg)
    {

    case FM_BUTTONDOWN:
    case FM_BUTTONUP:
    {
        uint8_t index = (uint8_t)param;
        chars += sprintf_s(g_szBuffer + chars, BUFFER_LENGTH - chars, "button=%i", index);
        break;
    }

    case FM_NOTIFY:
    {
        notification_header_t* pNTF = (notification_header_t*)param;
        chars += sprintf_s(g_szBuffer + chars, BUFFER_LENGTH - chars, "code=%i", pNTF->nCode);
        break;
    }

    }

    strcat_s(g_szBuffer + chars, BUFFER_LENGTH - chars, "\n");

#ifdef _WINDOWS
    OutputDebugStringA(g_szBuffer);
#endif
}


 /* END OF FILE */
/*
 * File     frame_input.h
 * Date     05.01.2022
 */

#ifndef __FRAME_INPUT_H
#define __FRAME_INPUT_H

#include "../../core/inc/bv_types.h"
#include "../../core/inc/bv_frame.h"


// Notification codes
#define IN_BASE                         100
#define IN_INIT                         (IN_BASE + 1)
#define IN_UPDATE                       (IN_BASE + 2)


#define INPUT_BUFFER_LENGTH_MAX         32

// ������ ��������������
typedef enum input_frame_mode_t
{
    // ������������� �������� - �����.
    IFM_TYPE_TEXT = 0x0001,
    // ������������� �������� - ����� ��� �������� �����.
    IFM_TYPE_INTEGER = 0x0002,
    // ������������� �������� - ����� �������� �����.
    IFM_TYPE_SIGNED_INTEGER = 0x0004,
    // ������������� �������� - ������������ �������� �����.
    IFM_TYPE_DOUBLE = 0x0008,
} input_frame_mode_t;


// ������ ����������� IN_INIT
typedef struct init_notification_t
{
    // ����������� ��������� �����������.
    notification_header_t   hdr;

    // ����� ������ � ������� ����� ����������� ������������� ��������.
    uint8_t*                szBuffer;
    
    // ������������ ������ ������, ������������� ��������� 
    // ������������� ������� ������� ������.
    // ��� IFM_TYPE_TEXT ����� ���������� ������������ ���������� �������� �������������� ��������.
    uint32_t                nLengthMax;

    // ������ ��������������.
    input_frame_mode_t      bmMode;

    // ����� ������������������ ���������� ��������.
    uint8_t*                pszKeysMap;

    // ������ ������ ������ ��������� �����������.
    uint8_t                 nFromButtonIndex;
} init_notification_t;


// ������ ����������� IN_UPDATE
typedef struct update_notification_t
{
    // ����������� ��������� �����������.
    notification_header_t   hdr;

    // ����� ������ ������� �������� ����������������� ��������.
    uint8_t*                szBuffer;

    // ����� ������ ��������� � ������ ����������� ��������.
    uint8_t*                szMessage;
} update_notification_t;


result_t FrameInputProc(
    frame_message_t eMsg,
    param_t param);


#endif // !__FRAME_INPUT_H

/* END OF FILE */

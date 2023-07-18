/*
 * File     frame_shared.h
 * Date     30.01.2022
 */

#ifndef __FRAME_SHARED_H
#define __FRAME_SHARED_H


#include "../../core/inc/bv_types.h"
#include "../../core/inc/bv_frame.h"


/// <summary>
/// ����������� ������� ��������� ��������� ������.
/// </summary>
/// <param name="eMsg">���������.</param>
/// <param name="param">��������, ������� �� ���������.</param>
/// <param name="proc">������� ��������� ��������� ������.</param>
/// <returns>���������, ������� �� ���������.</returns>
result_t BV_DefFrameProc(
    frame_message_t eMsg,
    param_t param,
    frame_proc_f proc);


#endif // !__FRAME_SHARED_H

/* END OF FILE */

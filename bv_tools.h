/*
 * File     bv_tools.h
 * Date     06.01.2022
 */

#ifndef __BV_TOOLS_H
#define __BV_TOOLS_H


#define _NULL                   0
#define _TRUE                   1
#define _FALSE                  0

#define max(a,b)                (((a) > (b)) ? (a) : (b))
#define min(a,b)                (((a) < (b)) ? (a) : (b))

#define _MAKEDWORD(hw,lw)       ((uint32_t)(((uint16_t)((lw) & 0xffff)) | ((uint16_t)((hw) & 0xffff)) << 16))
#define _MAKEWORD(hb,lb)        ((uint16_t)(((uint8_t)((lb) & 0xff)) | ((uint8_t)((hb) & 0xff)) << 8))
#define _LOWORD(dw)             ((uint16_t)((dw) & 0xffff))
#define _HIWORD(dw)             ((uint16_t)(((dw) >> 16) & 0xffff))
#define _LOBYTE(w)              ((uint8_t)((w) & 0xff))
#define _HIBYTE(w)              ((uint8_t)(((w) >> 8) & 0xff))
#define DW2B(dw,n)              ((uint8_t)(((dw) >> (n*8)) & 0xff))

#define _RGB(r,g,b)             ((uint32_t)(((uint8_t)(r) | ((uint16_t)((uint8_t)(g)) << 8)) | (((uint16_t)(uint8_t)(b)) << 16)))


#define _SendMessage(proc,msg,param)                (proc)((frame_message_t)(msg),(param_t)(param))

#define _SendMsgCreate(proc,index)                  _SendMessage(proc,FM_CREATE,(index))
#define _SendMsgDestroy(proc)                       _SendMessage(proc,FM_DESTROY,_NULL)
#define _SendMsgEraseBackground(proc)               _SendMessage(proc,FM_ERASEBKGND,_NULL)
#define _SendMsgPaint(proc)                         _SendMessage(proc,FM_PAINT,_NULL)
#define _SendMsgButtonUp(proc,index)                _SendMessage(proc,FM_BUTTONUP,(index))
#define _SendMsgButtonDown(proc,index)              _SendMessage(proc,FM_BUTTONDOWN,(index))
#define _SendMsgNotification(proc,notify)           _SendMessage(proc,FM_NOTIFY,(notify))


#endif // !__BV_TOOLS_H

/* END OF FILE */

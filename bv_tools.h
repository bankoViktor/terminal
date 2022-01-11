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


#define _SendMessage(proc,msg,param)            (proc)((frame_message_t)(msg),(param_t)(param))
#define _SendMessage4(proc,msg,bhh,bhl,blh,bll) _SendMessage ((proc),(msg),_MAKEDWORD(_MAKEWORD((uint8_t)(bhh),(uint8_t)(bhl)),_MAKEWORD((uint8_t)(blh),(uint8_t)(bll))))
#define _SendMessage2(proc,msg,wh,wl)           _SendMessage ((proc),(msg),_MAKEDWORD(_NULL,_MAKEDWORD((wh),(wl))))

#define _SendMsgCreate(proc)                    _SendMessage ((proc),FM_CREATE,_NULL)
#define _SendMsgDestroy(proc)                   _SendMessage ((proc),FM_DESTROY,_NULL)
#define _SendMsgEraseBackground(proc)           _SendMessage ((proc),FM_ERASEBKGND,_NULL)
#define _SendMsgPaint(proc)                     _SendMessage ((proc),FM_PAINT,_NULL)
#define _SendMsgNotify(proc,pntf)               _SendMessage ((proc),FM_NOTIFY,(pntf))
#define _SendMsgButtonUp(proc,idx)              _SendMessage2((proc),FM_BUTTONUP,_NULL,(idx))
#define _SendMsgButtonDown(proc,idx)            _SendMessage2((proc),FM_BUTTONDOWN,_NULL,(idx))


#endif // !__BV_TOOLS_H

/* END OF FILE */

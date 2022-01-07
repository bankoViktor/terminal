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


#define _SendMessage4(proc,msg,b0,b1,b2,b3)     proc((uint16_t)(msg),_MAKEDWORD(_MAKEWORD((uint8_t)(b3),(uint8_t)(b2)),_MAKEWORD((uint8_t)(b1),(uint8_t)(b0))))
#define _SendMessage3(proc,msg,b0,b1,b2)        _SendMessage4(proc,(msg),(b0),(b1),(b2),_NULL)
#define _SendMessage2(proc,msg,b0,b1)           _SendMessage4(proc,(msg),(b0),(b1),_NULL,_NULL)
#define _SendMessage1(proc,msg,b0)              _SendMessage4(proc,(msg),(b0),_NULL,_NULL,_NULL)
#define _SendMessage0(proc,msg)                 _SendMessage4(proc,(msg),_NULL,_NULL,_NULL,_NULL)

#define _SendMsgPaint(proc)                        _SendMessage0(proc,FM_PAINT)
#define _SendMsgNotification(proc,btn,ncode)       _SendMessage2(proc,FM_NOTIFICATION,(btn),(ncode))
#define _SendMsgUpdate(proc,btn)                   _SendMessage1(proc,FM_UPDATE,(btn))


#endif // !__BV_TOOLS_H

/* END OF FILE */

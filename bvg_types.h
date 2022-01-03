/*
 * File     bvg_types.h
 * Date     28.12.2021
 */

#ifndef __BVG_TYPES_H
#define __BVG_TYPES_H


typedef signed char     int8_t;
typedef signed short    int16_t;
typedef signed int      int32_t;
typedef signed long     int64_t;

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
typedef unsigned long   uint64_t;

typedef int16_t         coord_t;
typedef int32_t         color_t;

typedef enum horizontal_aligment_t
{
    H_ALIGN_LEFT,
    H_ALIGN_CENTER,
    H_ALIGN_RIGHT
} horizontal_aligment_t;

typedef enum vertical_aligment_t
{
    V_ALIGN_TOP,
    V_ALIGN_MIDDLE,
    V_ALIGN_BOTTOM
} vertical_aligment_t;

typedef void (*frame_proc_f)(uint16_t nMsg, uint32_t param);

#endif // !__BVG_TYPES_H

/* END OF FILE */


//typedef enum button_type_t
//{
//    None,       // Не используется.
//    NoActive,   // Цвета надписи нет инвертируются.
//    Active,     // Цвета надписи инвертируются.
//    Action,     // Действие.
//    Goto,       // Передох на другую страницу.
//    Up,         // Вместо надписи отображается большой треугольник ориентированный вверх.
//    Down,       // Вместо надписи отображается большой треугольник ориентированный вниз.
//    Left,       // Вместо надписи отображается большой треугольник ориентированный влево.
//    Right,      // Вместо надписи отображается большой треугольник ориентированный вправо.
//    Toggle,     // Циклически переключает заданные варианты.
//    Input,      // Переход на страницу ввода.
//} button_type_t;


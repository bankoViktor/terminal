/*
 * File     bvg_types.h
 * Date     28.12.2021
 */

#ifndef __BVG_TYPES_H
#define __BVG_TYPES_H


//#include "terminal_config.h


//#define BUTTON_COUNT                ((BUTTON_COUNT_X + BUTTON_COUNT_Y) * 2)
//#define BUTTONS_TOP                 (BUTTON_COUNT_X)
//#define BUTTONS_RIGHT               (BUTTON_COUNT_X + BUTTON_COUNT_Y)
//#define BUTTONS_BOTTOM              (BUTTON_COUNT_X * 2 + BUTTON_COUNT_Y)
//#define BUTTONS_LEFT                (BUTTON_COUNT)


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

#endif // !__BVG_TYPES_H

/* END OF FILE */


//typedef enum horizontal_aligment_t
//{
//    Left,
//    Center,
//    Right
//} horizontal_aligment_t;
//
//typedef enum vertical_aligment_t
//{
//    Top,
//    Middle,
//    Bottom
//} vertical_aligment_t;

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

//typedef struct button_t
//{
//    //button_type_t   type;
//    char            szTitle[16];
//} button_t;



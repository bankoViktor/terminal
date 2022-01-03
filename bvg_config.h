/*
 * File     bvg_config.h
 * Date     28.12.2021
 */

#ifndef __BVG_CONFIG_H
#define __BVG_CONFIG_H


/* Флаг режима отладки */
#define TERMINAL_DEBUG

#ifdef TERMINAL_DEBUG

#define _RGB(r,g,b)                  ((uint32_t)(((uint8_t)(r) | ((uint16_t)((uint8_t)(g)) << 8)) | (((uint16_t)(uint8_t)(b)) << 16)))

/* Цвет сетки-разметки */
#define GRID_COLOR                  _RGB(40, 40, 40)

/* Размер маркеров реферных точек */
#define BUTTON_MARKER_SIZE          8

/* Рамка клиентской области */
#define CLIENT_RECT_COLOR           _RGB(0, 100, 200)

/* Цвет маркеров реферных точек */
#define MARKER_COLOR                _RGB(255, 0, 0)


#endif // !TERMINAL_DEBUG

/* Ширина экрана (в пикселях) */
#define TERMINAL_WIDTH              480

/* Высота экрана (в пикселях) */
#define TERMINAL_HEIGHT             480

/* Размер стека переходов фреймов */
#define TERMINAL_STACK_SIZE         8

/* Количество кнопок по горизонтали */
#define BUTTON_COUNT_X              5

/* Количество кнопок по вертикали */
#define BUTTON_COUNT_Y              5

/* Смещение (в пикселях) в качестве рамки безопасности (сещение от края экрана) */
#define SAFE_OFFSET                 10

/* Смещение (в пикселях) надписей кнопок относительно рамки безопасности */
#define BUTTON_OFFSET               15

/* */
#define BUTTON_STRECH_X             0

/* */
#define BUTTON_STRECH_Y             0

/* */
#define BUTTON_MOVE_X               0

/* */
#define BUTTON_MOVE_Y               0

/* Цвет текста */
#define TEXT_COLOR                  _RGB(50, 200, 50)

/* Цвет фона текста */
#define TEXT_BGCOLOR                _RGB(10, 10, 10)
//#define TEXT_BGCOLOR                _RGB(60, 60, 60)

#define BUTTON_COUNT                ((BUTTON_COUNT_X + BUTTON_COUNT_Y) * 2)
#define BUTTONS_TOP                 (BUTTON_COUNT_X)
#define BUTTONS_RIGHT               (BUTTON_COUNT_X + BUTTON_COUNT_Y)
#define BUTTONS_BOTTOM              (BUTTON_COUNT_X * 2 + BUTTON_COUNT_Y)
#define BUTTONS_LEFT                (BUTTON_COUNT)


#endif // !__BVG_CONFIG_H

/* END OF FILE */

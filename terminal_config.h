#pragma once

#include "types.h"

/* Флаг режима отладки */
#define TERMINAL_DEBUG

#ifdef TERMINAL_DEBUG

/* Цвет сетки-разметки */
#define GRID_COLOR                  color_t(40, 40, 40)

/* Размер маркеров реферных точек */
#define BUTTON_MARKER_SIZE          8

/* Рамка клиентской области */
#define CLIENT_RECT_COLOR           color_t(0, 100, 200)

/* Цвет маркеров реферных точек */
#define MARKER_COLOR                color_t(100, 0, 0)


#endif // !TERMINAL_DEBUG


#ifndef TERMINAL_CONFIG
#define TERMINAL_CONFIG

/* Ширина экрана (в пикселях) */
#define TERMINAL_WIDTH              480

/* Высота экрана (в пикселях) */
#define TERMINAL_HEIGHT             480

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
#define TEXT_COLOR                  color_t(50, 200, 50)

/* Цвет фона текста */
#define TEXT_BGCOLOR                color_t(10, 10, 10)
//#define TEXT_BGCOLOR                _RGB(60, 60, 60)

#endif // !TERMINAL_CONFIG

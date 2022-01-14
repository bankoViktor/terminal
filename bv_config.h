/*
 * File     bv_config.h
 * Date     28.12.2021
 */

#ifndef __BV_CONFIG_H
#define __BV_CONFIG_H

#include "bv_tools.h"


/* Флаг режима отладки */
#define TERMINAL_DEBUG


/* Цвет сетки-разметки */
#define GRID_COLOR                  _RGB(40, 40, 40)

/* Рамка клиентской области */
#define CLIENT_RECT_COLOR           _RGB(0, 100, 200)

/* Размер маркеров реферных точек (отладочная) для визцализации координаты точки */
#define MARKER_SIZE                 8

/* Цвет маркеров реферных точек */
#define MARKER_COLOR                _RGB(255, 0, 0)


/* Ширина экрана (в пикселях) */
#define TERMINAL_WIDTH              480

/* Высота экрана (в пикселях) */
#define TERMINAL_HEIGHT             480

/* Размер стека переходов по фреймам */
#define TERMINAL_STACK_SIZE         4

/* Количество кнопок по горизонтали */
#define BUTTON_COUNT_X              5

/* Количество кнопок по вертикали */
#define BUTTON_COUNT_Y              5


/* Смещение (в пикселях) от края экрана (в качестве рамки безопасности) */
#define SAFE_OFFSET                 10

/* Смещение (в пикселях) слева от края экрана (в качестве рамки безопасности) */
#define SAFE_OFFET_LEFT             10

/* Смещение (в пикселях) справа от края экрана (в качестве рамки безопасности) */
#define SAFE_OFFSET_RIGHT           10

/* Смещение (в пикселях) сверху от края экрана (в качестве рамки безопасности) */
#define SAFE_OFFSET_TOP             10

/* Смещение (в пикселях) снизу от края экрана (в качестве рамки безопасности) */
#define SAFE_OFFSET_BOTTOM          10


/* Смещение (в пикселях) надписей кнопок относительно рамки безопасности */
#define BUTTON_OFFSET               15

/* Сжатие межкнопочного растояния по горизонтали */
#define BUTTON_STRECH_X             0

/* Сжатие межкнопочного растояния по вертикали */
#define BUTTON_STRECH_Y             0

/* Смещение верхней и нижней групп кнопок по горизонтали */
#define BUTTON_MOVE_X               0

/* Смещение левой и правой групп кнопок по вертикали */
#define BUTTON_MOVE_Y               0


/* Цвет текста */
#define TEXT_COLOR                  _RGB(50, 200, 50)

/* Цвет фона текста */
#define TEXT_BGCOLOR                _RGB(0, 0, 0)

/* Цвет фона */
#define BACKGROUND_COLOR            _RGB(30, 30, 30)


/* Размер треугольников UP/DOWN/LEFT/RIGHT */
#define TRIANGLE_SIZE               15


/* Размер буфера текста сообщения. */
#define MESSAGE_BUFFER_SIZE         128

/* Цвет текста сообщения об ошибке */
#define MSG_ERROR_FORE_COLOR        _RGB(255, 0, 0)

/* Цвет фона сообщения об ошибке */
#define MSG_ERROR_BACK_COLOR        _RGB(255, 255, 255)


/* НЕ МЕНЯТЬ */
#define BUTTON_COUNT                ((BUTTON_COUNT_X + BUTTON_COUNT_Y) * 2)
#define BUTTONS_TOP                 (BUTTON_COUNT_X)
#define BUTTONS_RIGHT               (BUTTON_COUNT_X + BUTTON_COUNT_Y)
#define BUTTONS_BOTTOM              (BUTTON_COUNT_X * 2 + BUTTON_COUNT_Y)
#define BUTTONS_LEFT                (BUTTON_COUNT)


#endif // !__BV_CONFIG_H

/* END OF FILE */

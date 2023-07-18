#pragma once

#include <bv_types.h>


/* Длина тестового текста */
#define TEST_TEXT_LENGHT             16


typedef struct user_data_t {
    uint8_t     bBool;
    uint8_t     bSelectable;
    uint8_t     bMode;
    uint16_t    wInputNumber;
    uint8_t     szTestTextValue[TEST_TEXT_LENGHT];
    double      dInputDouble;
} user_data_t;


extern user_data_t g_UserData;

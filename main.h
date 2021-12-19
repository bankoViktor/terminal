#pragma once

#include "framework.h"
#include "resource.h"



#define MAX_LOADSTRING              100

#define WINDOW_BG_COLOR             RGB(61, 61, 61)

#define CTL_BUTTON_SIZE             30
#define CTL_BUTTON_ZONE_SIZE        40
#define CTL_BUTTON_BASE_ID          1000

#define RECT_WIDTH(rc)              ((rc).right  - (rc).left)
#define RECT_HEIGHT(rc)             ((rc).bottom - (rc).top)

#define _torect(rc)                 (rect_t((coord_t)rc.left, (coord_t)rc.top, (coord_t)rc.right, (coord_t)rc.bottom))

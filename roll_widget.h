#pragma once

#include "render_context.h"


using roll_widget_t = struct roll_widget_t
{
    void operator()(const render_context_t& context, const rect_t& rc, int8_t angle, color_t color)
    {
        const uint16_t thickness = 3;
        auto wings = rc.width() * 0.15;

        rect_t circle = rc;
        circle.inflate(-wings);

        point_t pt = { rc.left, rc.top };
        pt.y += rc.height() * 0.35f;
        context.moveTo(pt.x, pt.y);
        pt.x = circle.left;
        context.lineTo(pt.x, pt.y, color, thickness);
        auto radius = circle.width() / 2;
        pt.x += radius;
        context.angleArc(pt, radius, 180, 180, color, thickness);
        context.lineTo(rc.right, pt.y, color, thickness);

        context.moveTo(circle.left + 10, pt.y);
        context.lineTo(circle.right - 10, pt.y, color, thickness);
    }
};


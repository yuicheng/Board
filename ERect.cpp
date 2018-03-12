//
// Created by yype on 3/10/18.
//

#include "ERect.h"

bool ERect::IsTouched(Point TouchPoint) {
    Point point1, point2;
    point1 = point.at(0);
    point2.x = point.at(0).x;
    point2.y = point.at(1).y;//左上、左下
    if (IsAtLineForRectAndPolygon(point1, point2, TouchPoint)) {
        return true;
    }
    point1.x = point.at(0).x;
    point1.y = point.at(1).y;
    point2 = point.at(1);//左下、右下
    if (IsAtLineForRectAndPolygon(point1, point2, TouchPoint)) {
        return true;
    }
    point1 = point.at(1);
    point2.x = point.at(1).x;
    point2.y = point.at(0).y;//右下、右上
    if (IsAtLineForRectAndPolygon(point1, point2, TouchPoint)) {
        return true;
    }
    point1.x = point.at(1).x;
    point1.y = point.at(0).y;//右上、左上
    point2 = point.at(0);
    if (IsAtLineForRectAndPolygon(point1, point2, TouchPoint)) {
        return true;
    }
    return false;
}

int ERect::GetShapeType() {
    return SHAPE_RECTANGLE;
}

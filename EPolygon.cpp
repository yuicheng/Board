//
// Created by yype on 3/10/18.
//

#include "EPolygon.h"

bool EPolygon::IsTouched(Point TouchPoint) {
    Point point1, point2;
    int NumberOfPoints = this->point.size();
    for (int i = 0; i < NumberOfPoints; i++) {
        point1 = point.at(i);
        point2 = point.at((i + 1) % NumberOfPoints);
        if (IsAtLineForRectAndPolygon(point1, point2, TouchPoint)) {
            return true;
        }
    }
    return false;
}

int EPolygon::GetShapeType() {
    return SHAPE_POLYGON;
}

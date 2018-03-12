//
// Created by yype on 3/10/18.
//

#ifndef BOARD_EPOLYGON_H
#define BOARD_EPOLYGON_H


#include "EShape.h"

class EPolygon : public EShape {
public:
    bool IsTouched(Point TouchPoint) override;

    int GetShapeType() override;

    EPolygon() : EShape() {}

    EPolygon(vector<Point> point, Color color) : EShape(point, color) {}
};


#endif //BOARD_EPOLYGON_H

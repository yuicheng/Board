//
// Created by yype on 3/10/18.
//

#ifndef BOARD_ELINE_H
#define BOARD_ELINE_H


#include "EShape.h"

class ELine: public EShape {
public:
    bool IsTouched(Point TouchPoint) override;
    int GetShapeType() override ;
    ELine():EShape(){}
    ELine(vector<Point> point, Color color):EShape(point,color){}
};


#endif //BOARD_ELINE_H

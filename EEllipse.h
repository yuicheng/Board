//
// Created by yype on 3/10/18.
//

#ifndef BOARD_EELLIPSE_H
#define BOARD_EELLIPSE_H


#include "EShape.h"

class EEllipse : public EShape{
public:
    bool IsTouched(Point TouchPoint) override ;
    int GetShapeType() override ;
    EEllipse():EShape(){}
    EEllipse(vector<Point> point, Color color):EShape(point,color){}
};


#endif //BOARD_EELLIPSE_H

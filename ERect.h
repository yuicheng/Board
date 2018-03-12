//
// Created by yype on 3/10/18.
// ERect!Funny name!
//

#ifndef BOARD_ERECT_H
#define BOARD_ERECT_H


#include "EShape.h"

class ERect : public EShape {
public:
    bool IsTouched(Point TouchPoint) override ;
    int GetShapeType() override ;
    ERect():EShape(){}
    ERect(vector<Point> point, Color color):EShape(point,color){}
};


#endif //BOARD_ERECT_H

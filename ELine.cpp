//
// Created by yype on 3/10/18.
//

#include "ELine.h"

bool ELine::IsTouched(Point TouchPoint) {
    int ErrorRange = 5;   //误差范围

    if (((TouchPoint.x - point.at(0).x) * (TouchPoint.x - point.at(1).x)) <= 0 ||
        abs(TouchPoint.x - point.at(0).x) <= ErrorRange ||
        abs(TouchPoint.x - point.at(1).x) <= ErrorRange) {       //x坐标符合范围

        if (point.at(0).x == point.at(1).x) {
            if ((TouchPoint.y - point.at(0).y) * (TouchPoint.y - point.at(1).y) <= 0 ||
                abs(TouchPoint.y - point.at(0).y) <= ErrorRange ||
                abs(TouchPoint.y - point.at(1).y) <= ErrorRange) {
                return true;
            }
        } else {
            double k = (point.at(0).y - point.at(1).y) / (point.at(0).x - point.at(1).x);
            //y=kx+b,b=y-kx
            double b = (point.at(0).y - k * point.at(0).x);
            //y-kx-b=0(in ErrorRange)
            int ErrorRangePrev=ErrorRange;;
            if (abs(k) >= 1) {
                ErrorRange *= abs(k);    //乘k更精准
            }
            if (abs(TouchPoint.y - k * TouchPoint.x - b) <= ErrorRange) {   //y符合范围
                if(((TouchPoint.y - point.at(0).y) * (TouchPoint.y - point.at(1).y)) <= 0 ||
                   abs(TouchPoint.y - point.at(0).y) <= ErrorRangePrev ||
                   abs(TouchPoint.y - point.at(1).y) <= ErrorRangePrev){
                    return true;
                }

            }

        }
    }
    return false;
}

int ELine::GetShapeType() {
    return SHAPE_LINE;
}

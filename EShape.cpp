//
// Created by yype on 3/6/18.
//

#include "EShape.h"


bool EShape::IsAtLineForRectAndPolygon(Point point1, Point point2, Point TouchPoint) {
    int ErrorRange = 5;   //误差范围

    if (((TouchPoint.x - point1.x) * (TouchPoint.x - point2.x)) <= 0 ||
        abs(TouchPoint.x - point1.x) <= ErrorRange ||
        abs(TouchPoint.x - point2.x) <= ErrorRange) {       //x坐标符合范围

        if (point1.x == point2.x) {   //斜率不存在
            if ((TouchPoint.y - point1.y) * (TouchPoint.y - point2.y) <= 0 ||
                abs(TouchPoint.y - point1.y) <= ErrorRange ||
                abs(TouchPoint.y - point2.y) <= ErrorRange) {
                return true;
            }
        } else {
            double k = (point1.y - point2.y) / (point1.x - point2.x);
            //y=kx+b,b=y-kx
            double b = (point1.y - k * point1.x);
            int ErrorRangePrev = ErrorRange;;
            if (abs(k) >= 1) {
                ErrorRange *= abs(k);
            }
            //y-kx-b=0(in ErrorRange)
            if (abs(TouchPoint.y - k * TouchPoint.x - b) <= ErrorRange) {
                if (((TouchPoint.y - point1.y) * (TouchPoint.y - point2.y)) <= 0 ||
                    abs(TouchPoint.y - point1.y) <= ErrorRangePrev ||
                    abs(TouchPoint.y - point2.y) <= ErrorRangePrev) {
                    return true;
                }
            }
        }
    }
    return false;
}


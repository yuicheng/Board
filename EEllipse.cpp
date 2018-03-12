//
// Created by yype on 3/10/18.
//

#include "EEllipse.h"

bool EEllipse::IsTouched(Point TouchPoint) {
    int ErrorRange = 5;
    double a, b, c;
    a = (point.at(1).x - point.at(0).x) / 2;
    b = (point.at(1).y - point.at(0).y) / 2;
    Point CentralPoint = {point.at(0).x + a, point.at(0).y + b};
    a = abs(a);
    b = abs(b);

    Point focus1, focus2;
    if (a >= b) {   //焦点在水平线上
        c = sqrt(a * a - b * b);
        focus1 = {CentralPoint.x + c, CentralPoint.y};
        focus2 = {CentralPoint.x - c, CentralPoint.y};
    } else {      //焦点在垂直线上
        c = sqrt(b * b - a * a);
        focus1 = {CentralPoint.x, CentralPoint.y + c};
        focus2 = {CentralPoint.x, CentralPoint.y - c};
    }
    double d1 = (TouchPoint.x - focus1.x) * (TouchPoint.x - focus1.x)
                + (TouchPoint.y - focus1.y) * (TouchPoint.y - focus1.y);
    double d2 = (TouchPoint.x - focus2.x) * (TouchPoint.x - focus2.x)
                + (TouchPoint.y - focus2.y) * (TouchPoint.y - focus2.y);
    double DistenceSum = sqrt(d1) + sqrt(d2);

    if (((abs(DistenceSum - 2 * a) <= ErrorRange) && a >= b) ||
        ((abs(DistenceSum - 2 * b) <= ErrorRange) && a < b)) {
        return true;
    }
    return false;
}

int EEllipse::GetShapeType() {
    return SHAPE_ELLIPSE;
}

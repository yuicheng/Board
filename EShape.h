//
// Created by yype on 3/6/18.
//

#ifndef BOARD_ESHAPE_H  //编译一次
#define BOARD_ESHAPE_H

#include <vector>
#include <cmath>
#include "math.h"

using namespace std;

#define SHAPE_LINE 1
#define SHAPE_RECTANGLE 2
#define SHAPE_ELLIPSE 3
#define SHAPE_POLYGON 4


struct Color {
    unsigned char Red;
    unsigned char Green;
    unsigned char Blue;
};

struct Point {  //定义成double方便提高判定精确度，实际保存为int
    double x;
    double y;
};

class EShape {

public:
    virtual bool IsTouched(Point TouchPoint) {}; //虚函数必须带有{}定义，否则会link_error
    virtual int GetShapeType() {};

    vector<Point> point;    //点集
    Color color;            //该图形绘制颜色

    EShape(vector<Point> point, Color color) {   //带参构造函数
        this->point.assign(point.begin(), point.end());
        this->color = color;
    }

    EShape() {}

    ~EShape() {}

    //给矩形与多边形子类共有
    bool IsAtLineForRectAndPolygon(Point point1, Point point2, Point TouchPoint);

    bool IsSelected = false;


};


#endif //BOARD_ESHAPE_H
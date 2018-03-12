//
// Created by yype on 3/5/18.
//

#ifndef BOARD_BOARD_H
#define BOARD_BOARD_H

#endif //BOARD_BOARD_H

#include "EShape.h"

class Board
{
private:
    void AddCharsToVector(vector<char> &yourVector,char* yourChars,int length);
    bool IsShapeValid(vector<Point> point);
public:
    vector<EShape*> EShapes;
    EShape *ShapeBeingDrawn; //正在被绘制的图形，动态修改

    void AddEShape(int ShapeType,vector<Point> point,Color color);
    void DeleteSelectedShapes();
    void DeleteAllShapes();
    void LoadShapesFromFile(char *fileName);
    void SaveShapesToFile(char *fileName);
    void AddCurrentShape();
    void ClearSelection();
    void MovingShapes(Point StartingPoint,Point CurrentPoint);
    Board();
};
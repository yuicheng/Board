//
// Created by yype on 3/6/18.
//

#ifndef BOARD_MAIN_H
#define BOARD_MAIN_H

#endif //BOARD_MAIN_H

#include <iostream>
#include "Board.h"
#include "ELine.h"
#include "EEllipse.h"
#include "ERect.h"
#include "EPolygon.h"

#define FONT_SIZE 14


#define IDM_LINE 11
#define IDM_RECT 12
#define IDM_ELLIPSE 13
#define IDM_POLYGON 14
#define IDM_DRAWMODE 15
#define IDM_DRAGMODE 16
#define IDM_SAVEFILE 17
#define IDM_LOADFILE 18
#define IDM_COLOR 19
#define IDM_SHOWHELP 20


#define MODE_DRAW 21
#define MODE_DRAG 22
#define MODE_COLOR 23
#define MODE_SHOWHELP 24
#define PI 3.1415926535

void SaveEShapeCurrentPoint(vector<vector<Point>> &points);

void ResetEShapePoint(vector<vector<Point>> points);

//正在绘制的图形类
ELine eLine;
ERect eRect;
EEllipse eEllipse;
EPolygon ePolygon;

Color CurrentColor = {0, 0, 0};
bool IsDrawing = false;
Point StartingPoint;
int halfWidth, halfHeight;
char Mode = MODE_DRAW;
Board MainBoard;
bool IsAtMenuWindow;
vector<Point> previous_poly;
vector<vector<Point> > EShape_Points_bak;
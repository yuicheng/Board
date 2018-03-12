//
// Created by yype on 3/5/18.
//

#include <cstdio>
#include <cstring>
#include "Board.h"
#include "ERect.h"
#include "EEllipse.h"
#include "EPolygon.h"
#include "ELine.h"

EShape *eShape;

void Board::AddEShape(int ShapeType, vector<Point> point, Color color) {

    switch (ShapeType) {
        case SHAPE_LINE:
            eShape = new ELine(point, color);  //分配新内存给新类
            this->EShapes.push_back(eShape);
            break;
        case SHAPE_RECTANGLE:
            eShape = new ERect(point, color);
            this->EShapes.push_back(eShape);
            break;
        case SHAPE_ELLIPSE:
            eShape = new EEllipse(point, color);
            this->EShapes.push_back(eShape);
            break;
        case SHAPE_POLYGON:
            eShape = new EPolygon(point, color);
            this->EShapes.push_back(eShape);
            break;
    }

}

void Board::DeleteSelectedShapes() {
    vector<EShape *> newEShapes;
    for (int i = 0; i < EShapes.size(); i++) {
        if (EShapes.at(i)->IsSelected == false) {
            newEShapes.push_back(EShapes.at(i));
        } else {
            delete EShapes.at(i);//释放内存
        }
    }
    this->EShapes.assign(newEShapes.begin(), newEShapes.end());
}

void Board::LoadShapesFromFile(char *fileName) {
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("Opening file failed in LoadShapesFromFile.\n");
        return;
    }
    fseek(file, 0, SEEK_END);
    int fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (feof(file)) {
        printf("Error Proceeding in LoadShapesFromFile(Empty file content).\n");
        return;
    }

    DeleteAllShapes();

    char *readData = new char[fileSize];
    fread(readData, fileSize, 1, file);
    fclose(file);
    if (strcmp(&readData[fileSize - 4], "END") != 0) {
        printf("Corrupted file!\n");
        return;
    }
    readData += 16;
    int ShapesToLoad = *(int *) readData;
    if (ShapesToLoad == 0) {
        printf("Blank project file.\n");
        return;
    }
    readData += 4;
    while (true) {

        int shapeType = readData[0];
        if (shapeType != SHAPE_POLYGON) {
            readData++;
            Point point1, point2;
            vector<Point> point;
            int *coordinate = (int *) readData;
            point1.x = *coordinate;

            readData += 4;
            coordinate = (int *) readData;
            point1.y = *coordinate;

            readData += 4;
            coordinate = (int *) readData;
            point2.x = *coordinate;

            readData += 4;
            coordinate = (int *) readData;
            point2.y = *coordinate;

            readData += 4;
            point.push_back(point1);
            point.push_back(point2);
            Color color = {(unsigned char) readData[0],
                           (unsigned char) readData[1],
                           (unsigned char) readData[2]};
            switch (shapeType) {
                case SHAPE_LINE:
                    AddEShape(SHAPE_LINE, point, color);
                    break;
                case SHAPE_RECTANGLE:
                    AddEShape(SHAPE_RECTANGLE, point, color);
                    break;
                case SHAPE_ELLIPSE:
                    AddEShape(SHAPE_ELLIPSE, point, color);
                    break;
            }
            point.clear();
            readData += 3;
        } else {
            readData++;
            int *numberofpoints = (int *) readData;
            readData += 4;
            Point point;
            vector<Point> points;
            int coordinate;
            for (int i = 0; i < *numberofpoints; i++) {
                coordinate = ((int *) readData)[2 * i];
                point.x = coordinate;
                coordinate = ((int *) readData)[2 * i + 1];
                point.y = coordinate;
                points.push_back(point);
            }
            readData += 4 * 2 * (*numberofpoints);
            Color color = {(unsigned char) readData[0],
                           (unsigned char) readData[1],
                           (unsigned char) readData[2]};
            AddEShape(SHAPE_POLYGON, points, color);
            points.clear();
            readData += 3;
        }
        if (strcmp(readData, "END") == 0) {
            break;
        }
    }
    printf("Successfully loaded shapes from file. Number:%d.\n", (int) this->EShapes.size());


}

//文件格式：
//EBoard_Prj_File开头
//NumberOfShapes(int)
//ShapeInformationLoop:
////-01 00000000 00000000 00000000 00000000 00 00 00 //直线两点四个坐标+RGB
////-02 00000000 00000000 00000000 00000000 00 00 00 //矩形两点四个坐标+RGB
////-03 00000000 00000000 00000000 00000000 00 00 00 //椭圆两点四个坐标+RGB
////-04 00000000 point1.x point1.y point2.x point2.y ... 00 00 00 //多边形点个数+n点2n个坐标+RGB

void Board::SaveShapesToFile(char *fileName) {
    int NumberOfShapes = EShapes.size();

    vector<char> Data;
    AddCharsToVector(Data, "EBoard_Prj_File", 15);
    Data.push_back(0);
    int Number = NumberOfShapes;
    AddCharsToVector(Data, (char *) &Number, 4);
    if (NumberOfShapes == 0) {
        printf("(Blank file) ");
        goto label3;
    }
    int coordinate;
    for (int i = 0; i < NumberOfShapes; i++) {
        switch (EShapes.at(i)->GetShapeType()) {
            case SHAPE_LINE: //line
                Data.push_back(01);
            label1:
                coordinate = (int) EShapes.at(i)->point.at(0).x;
                AddCharsToVector(Data, (char *) &coordinate, 4);
                coordinate = (int) EShapes.at(i)->point.at(0).y;
                AddCharsToVector(Data, (char *) &coordinate, 4);
                coordinate = (int) EShapes.at(i)->point.at(1).x;
                AddCharsToVector(Data, (char *) &coordinate, 4);
                coordinate = (int) EShapes.at(i)->point.at(1).y;
                AddCharsToVector(Data, (char *) &coordinate, 4);
            label2:
                Data.push_back(EShapes.at(i)->color.Red);
                Data.push_back(EShapes.at(i)->color.Green);
                Data.push_back(EShapes.at(i)->color.Blue);
                break;
            case SHAPE_RECTANGLE: //rectangle
                Data.push_back(02);
                goto label1;
            case SHAPE_ELLIPSE: //ellipse
                Data.push_back(03);
                goto label1;
            case SHAPE_POLYGON: //polygon
                Data.push_back(04);
                int numberofpoints = EShapes.at(i)->point.size();
                AddCharsToVector(Data, (char *) &numberofpoints, 4);
                for (int ii = 0; ii < numberofpoints; ii++) {
                    coordinate = (int) EShapes.at(i)->point.at(ii).x;
                    AddCharsToVector(Data, (char *) &coordinate, 4);
                    coordinate = (int) EShapes.at(i)->point.at(ii).y;
                    AddCharsToVector(Data, (char *) &coordinate, 4);
                }
                goto label2;
        }
    }
    label3:
    AddCharsToVector(Data, "END", 4);
    char *newData = new char[Data.size()];
    for (int i = 0; i < Data.size(); i++) {
        newData[i] = Data.at(i);
    }
    //newData为文件内容
    FILE *file = fopen(fileName, "wb");
    if (file == NULL) {
        printf("Opening file failed!\n");
        return;
    }
    fwrite(newData, sizeof(char), Data.size(), file);
    fclose(file);
    printf("Successfully saved.\n");

}

void Board::AddCharsToVector(vector<char> &yourVector, char *yourChars, int length) {
    for (int i = 0; i < length; i++) {
        yourVector.push_back(yourChars[i]);
    }
}

Board::Board() {
}

void Board::DeleteAllShapes() {
    for (int i = 0; i < EShapes.size(); i++) {
        delete EShapes.at(i);   //释放内存
    }
    this->EShapes.clear();
}

void Board::AddCurrentShape() {
    if (IsShapeValid(ShapeBeingDrawn->point)) {
        AddEShape(ShapeBeingDrawn->GetShapeType(), ShapeBeingDrawn->point, ShapeBeingDrawn->color);
    }
}


//该函数检查图形的点是否过于密集
bool Board::IsShapeValid(vector<Point> point) {
    int Range = 2;
    for (int i = 0; i < point.size(); i++) {
        Point NextPoint = point.at((i + 1) % point.size());
        Point CurrentPoint = point.at(i);
        if (abs(NextPoint.x - CurrentPoint.x) > Range ||
            abs(NextPoint.y - CurrentPoint.y) > Range) {
            return true;
        }

    }
    return false;
}

void Board::ClearSelection() {
    for (int i = 0; i < EShapes.size(); i++) {
        if (EShapes.at(i)->IsSelected) {
            EShapes.at(i)->IsSelected = false;
        }
    }
}

void Board::MovingShapes(Point StartingPoint, Point CurrentPoint) {
    int delta_x = CurrentPoint.x - StartingPoint.x;
    int delta_y = CurrentPoint.y - StartingPoint.y;
    for (int i = 0; i < EShapes.size(); i++) {
        if (EShapes.at(i)->IsSelected) {
            for (int ii = 0; ii < EShapes.at(i)->point.size(); ii++) {
                EShapes.at(i)->point.at(ii).x += delta_x;
                EShapes.at(i)->point.at(ii).y += delta_y;
            }
        }
    }
}

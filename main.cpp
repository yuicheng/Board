#include "main.h"

#ifndef GLUT_DISABLE_ATEXIT_HACK
#define GLUT_DISABLE_ATEXIT_HACK
#endif

#include <GL/glut.h>


void DrawEllipse(vector<Point> points) {    //利用参数方程绘制闭合多边形，增大密度使之看起来像椭圆
    int density = 100;    //密度
    glBegin(GL_LINE_LOOP);
    double a = abs(points.at(0).x - points.at(1).x) / 2;
    double b = abs(points.at(0).y - points.at(1).y) / 2;
    double x0 = (points.at(0).x + points.at(1).x) / 2;
    double y0 = (points.at(0).y + points.at(1).y) / 2;
    double x, y;
    double nowAngle = 0;
    for (int i = 0; i < density; i++) {
        nowAngle = 2 * PI / density * i;
        x = a * sin(nowAngle) + x0;
        y = b * cos(nowAngle) + y0;
        glVertex2d(x, y);
    }
    glEnd();
}

void DrawRectange(vector<Point> points) {
    glBegin(GL_LINE_LOOP);
    glVertex2d(points.at(0).x, points.at(0).y);
    glVertex2d(points.at(0).x, points.at(1).y);
    glVertex2d(points.at(1).x, points.at(1).y);
    glVertex2d(points.at(1).x, points.at(0).y);
    glEnd();
}

void DrawPolygon(vector<Point> points) {
    int number = points.size();
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < number; i++) {
        glVertex2d(points.at(i).x, points.at(i).y);
    }
    glEnd();
}

void DrawLine(vector<Point> points) {
    glBegin(GL_LINE_STRIP);
    glVertex2d(points.at(0).x, points.at(0).y);
    glVertex2d(points.at(1).x, points.at(1).y);
    glEnd();
}

void CorrectCoordinate2(int &x, int &y) {   //矫正glReadPixels坐标,opengl的坐标不统一也是够了。。。
    y = -y + halfHeight * 2;

}

void CorrectCoordinate(int &x, int &y) {
    x -= halfWidth;
    y = -y + halfHeight;

}


void SaveProject() {
    MainBoard.SaveShapesToFile("project.bdf");
    glutSetWindowTitle("Board - Project saved.");
}

void LoadProject() {
    MainBoard.LoadShapesFromFile("project.bdf");
    if (!MainBoard.EShapes.size()) {
        glutSetWindowTitle("Board - Empty project loaded.");
        Mode = MODE_SHOWHELP;
    } else {
        glutSetWindowTitle("Board - Project loaded.");
    }

    glutPostRedisplay();
}

void ChangeSize(GLsizei w, GLsizei h) {
    halfHeight = h / 2;
    halfWidth = w / 2;

    /* 定义视口大小，宽高一致 */
    glViewport(0, 0, w, h);
    /* 重置坐标系统，使投影变换复位 */
    glMatrixMode(GL_PROJECTION);
    /* 将当前的用户坐标系的原点移到了屏幕中心 */
    glLoadIdentity();
    /* 定义正交视域体 */
    glOrtho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1, 1);

}

void OnMouse(int button, int state, int x, int y) {

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        IsAtMenuWindow = true;
    }
    if (!IsAtMenuWindow) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (Mode == MODE_DRAW) {
                IsDrawing = true;
                CorrectCoordinate(x, y);
                StartingPoint = {(double) x, (double) y};
                if (MainBoard.ShapeBeingDrawn->GetShapeType() == SHAPE_POLYGON) {
                    previous_poly.push_back({(double) x, (double) y});
                    glutSetWindowTitle("Board - Press [space] to end adding points.");
                }
            } else if (Mode == MODE_DRAG) {  //拖动模式
                CorrectCoordinate(x, y);
                for (int i = 0; i < MainBoard.EShapes.size(); i++) {
                    if (MainBoard.EShapes.at(i)->IsTouched({(double) x, (double) y})) {
                        MainBoard.EShapes.at(i)->IsSelected = !MainBoard.EShapes.at(i)->IsSelected;
                    }
                }
                StartingPoint = {(double) x, (double) y};
                SaveEShapeCurrentPoint(EShape_Points_bak);  //保存一份原始坐标集合
                glutPostRedisplay();
            } else if (Mode == MODE_COLOR) {      //调色板模式
                int x_temp = x, y_temp = y;
                CorrectCoordinate(x, y);
                CorrectCoordinate2(x_temp, y_temp);
                if (x > -100 && x < 100 && y > -100 && y < 100) {
                    glReadPixels(x_temp, y_temp, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &CurrentColor);
                }
                //将正在绘制的图形颜色设置为所取颜色
                eLine.color = CurrentColor;
                eRect.color = CurrentColor;
                eEllipse.color = CurrentColor;
                ePolygon.color = CurrentColor;

                Mode = MODE_DRAW;
                MainBoard.ClearSelection();
                glutSetWindowTitle("Board - Drawing Mode");
                glutPostRedisplay();
            } else {   //Help模式下，单击任何地方都切换到DRAW模式
                Mode = MODE_DRAW;
            }

        }
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            if (Mode != MODE_DRAG) {
                if (MainBoard.ShapeBeingDrawn->GetShapeType() != SHAPE_POLYGON) {
                    IsDrawing = false;
                    MainBoard.AddCurrentShape();
                    glutPostRedisplay();
                }
            } else {  //拖动模式
                EShape_Points_bak.clear();
            }

        }
    }

}

void bitmapString(void *fontId, std::string strText) {
    for (unsigned int i = 0; i < (size_t) strText.size(); ++i) {
        glutBitmapCharacter(fontId, strText.at(i));
    }
}

void SaveEShapeCurrentPoint(vector<vector<Point>> &points) {
    points.clear();
    for (int i = 0; i < MainBoard.EShapes.size(); i++) {
        points.push_back(MainBoard.EShapes.at(i)->point);
    }
}

void OnMouseDownMove(int x, int y) {
    if (!IsAtMenuWindow) {
        if (Mode == MODE_DRAW) {
            if (MainBoard.ShapeBeingDrawn->GetShapeType() != SHAPE_POLYGON) {
                vector<Point> points;

                if (IsDrawing) {
                    CorrectCoordinate(x, y);
                    points.clear();
                    points.push_back(StartingPoint);
                    points.push_back({(double) x, (double) y});
                    MainBoard.ShapeBeingDrawn->point.assign(points.begin(), points.end());
                    MainBoard.ShapeBeingDrawn->color = CurrentColor;
                    glutPostRedisplay();
                }
            }
        } else if (Mode == MODE_DRAG) {  //拖动模式
            CorrectCoordinate(x, y);
            ResetEShapePoint(EShape_Points_bak);//首先设置到初始点集
            MainBoard.MovingShapes(StartingPoint, {(double) x, (double) y});//再移动
            glutPostRedisplay();
        }
    }

}

void ResetEShapePoint(vector<vector<Point>> points) {
    for (int i = 0; i < points.size(); i++) {
        MainBoard.EShapes.at(i)->point.assign(points.at(i).begin(), points.at(i).end());
        //将MainBoard中EShapes的各个元素的点集设置为备份的点集
    }
}

float To3f(unsigned char value) {
    return float(value) / 0xff;
}

void ShowColorPicker() {
    glBegin(GL_POLYGON);
    glColor3f(0, 0, 0);
    glVertex2d(-100, 100);
    glColor3f(1, 0, 0);
    glVertex2d(-100, 0);
    glColor3f(0, 1, 0);
    glVertex2d(-100, -100);
    glColor3f(0, 0, 1);
    glVertex2d(0, -100);
    glColor3f(1, 1, 1);
    glVertex2d(100, -100);
    glColor3f(0, 1, 1);
    glVertex2d(100, 0);
    glColor3f(1, 0, 0);
    glVertex2d(100, 100);
    glEnd();
}

void ShowHelp() {
    glColor3f(0, 0, 0);

    int top = 50;
    glRasterPos2f(-95, top);
    bitmapString(GLUT_BITMAP_HELVETICA_18,
                 "Board OpenGL Edition");
    glRasterPos2f(-140, top - 20);
    bitmapString(GLUT_BITMAP_HELVETICA_18,
                 "From uestc->bitman.entrance_test");
    glRasterPos2f(-240, top - 40);
    bitmapString(GLUT_BITMAP_HELVETICA_12,
                 "You have many options for drawing in the right_click pop-up menu, check it out ~");
    glRasterPos2f(-280, top - 60);
    bitmapString(GLUT_BITMAP_HELVETICA_12,
                 "You can select multiple shapes, press \"d\" to delete selected shapes, press \"c\" to delete all shapes.");
    glRasterPos2f(-260, top - 80);
    bitmapString(GLUT_BITMAP_HELVETICA_12,
                 "In polygon mode,each click will add a new point to the polygon, press [space] to end adding.");
    glRasterPos2f(-100, top - 100);
    bitmapString(GLUT_BITMAP_HELVETICA_12,
                 "Press Alt+s to save current project.");
    glRasterPos2f(-75, top - 120);
    bitmapString(GLUT_BITMAP_HELVETICA_10,
                 "Released by yypE on 2018-3-8.");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(2);

    glEnable(GL_BLEND);//开启混合
    glutInitDisplayMode(GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE_ARB);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//设置混合因子
    glEnable(GL_LINE_SMOOTH);//开启line平滑
    glEnable(GL_NICEST);//设置质量优先

    //绘制已保存图形
    for (int i = 0; i < MainBoard.EShapes.size(); i++) {
        glColor3f(To3f(MainBoard.EShapes.at(i)->color.Red),
                  To3f(MainBoard.EShapes.at(i)->color.Green),
                  To3f(MainBoard.EShapes.at(i)->color.Blue));
        if (MainBoard.EShapes.at(i)->IsSelected) {
            glLineWidth(4);
        } else {
            glLineWidth(2);
        }
        switch (MainBoard.EShapes.at(i)->GetShapeType()) {
            case SHAPE_LINE:
                DrawLine(MainBoard.EShapes.at(i)->point);
                break;
            case SHAPE_RECTANGLE:
                DrawRectange(MainBoard.EShapes.at(i)->point);
                break;
            case SHAPE_ELLIPSE:
                DrawEllipse(MainBoard.EShapes.at(i)->point);
                break;
            case SHAPE_POLYGON:
                DrawPolygon(MainBoard.EShapes.at(i)->point);
                break;
        }
    }

    //绘制用户正在进行绘制的图形
    if (IsDrawing) {
        glColor3f(To3f(MainBoard.ShapeBeingDrawn->color.Red),
                  To3f(MainBoard.ShapeBeingDrawn->color.Green),
                  To3f(MainBoard.ShapeBeingDrawn->color.Blue));
        switch (MainBoard.ShapeBeingDrawn->GetShapeType()) {

            case SHAPE_LINE:
                DrawLine(MainBoard.ShapeBeingDrawn->point);
                break;
            case SHAPE_RECTANGLE:
                DrawRectange(MainBoard.ShapeBeingDrawn->point);
                break;
            case SHAPE_ELLIPSE:
                DrawEllipse(MainBoard.ShapeBeingDrawn->point);
                break;
            case SHAPE_POLYGON:
                DrawPolygon(MainBoard.ShapeBeingDrawn->point);
                break;
        }
    }

    //再绘制其他内容
    if (Mode == MODE_SHOWHELP)
        ShowHelp();
    if (Mode == MODE_COLOR)
        ShowColorPicker();
    //刷新
    glFlush();
}

void MenuCallback(int value) {
    StartingPoint = {0, 0};
    MainBoard.ShapeBeingDrawn->point.clear();
    switch (value) {
        case IDM_SAVEFILE:
            SaveProject();
            break;
        case IDM_LOADFILE:
            LoadProject();
            break;
        case IDM_LINE:
            MainBoard.ShapeBeingDrawn = &eLine; //动态类切换
            goto label4;
            break;
        case IDM_RECT:
            MainBoard.ShapeBeingDrawn = &eRect;
            goto label4;
            break;
        case IDM_ELLIPSE:
            MainBoard.ShapeBeingDrawn = &eEllipse;
            goto label4;
            break;
        case IDM_POLYGON:
            MainBoard.ShapeBeingDrawn = &ePolygon;
            goto label4;
            break;
        case IDM_COLOR:
            Mode = MODE_COLOR;
            glutSetWindowTitle("Board - Color Picking...");
            glutPostRedisplay();
            break;
        case IDM_DRAGMODE:
            MainBoard.ClearSelection();
            Mode = MODE_DRAG;
            glutSetWindowTitle("Board - Drag Mode");
            break;
        case IDM_DRAWMODE:
        label4:
            MainBoard.ClearSelection();
            Mode = MODE_DRAW;
            glutSetWindowTitle("Board - Drawing Mode");
            break;
        case IDM_SHOWHELP:
            Mode = MODE_SHOWHELP;
            glutPostRedisplay();
            break;
    }
    IsAtMenuWindow = false;
}

void KeyCallback(unsigned char k, int x, int y) {
    if (k == 32) {  //[space]
        if (MainBoard.ShapeBeingDrawn->GetShapeType() == SHAPE_POLYGON) {
            MainBoard.ShapeBeingDrawn->point.assign(previous_poly.begin(), previous_poly.end());
            MainBoard.AddCurrentShape();
            previous_poly.clear();
            glutPostRedisplay();
            glutSetWindowTitle("Board - Drawing Mode.");
            IsDrawing = false;
        }

    }
    if (k == 'c' || k == 'C') {
        MainBoard.DeleteAllShapes();
        glutSetWindowTitle("Board - All cleaned.");
        glutPostRedisplay();
    }
    if (k == 's' || k == 'S') {
        if (glutGetModifiers() == GLUT_ACTIVE_ALT) {
            SaveProject();
        }
    }
    if (k == 'd' || k == 'D') {
        MainBoard.DeleteSelectedShapes();
        glutSetWindowTitle("Board - Selected shapes deleted.");
        glutPostRedisplay();
    }
}


//鼠标移动事件，控制正在被绘制的多边形的实时输出
//该移动事件在鼠标按下时不会被触发
void MouseMove(int x, int y) {
    if (!previous_poly.size() ||
        MainBoard.ShapeBeingDrawn->GetShapeType() != SHAPE_POLYGON) {
        return;
    }
    CorrectCoordinate(x, y);
    MainBoard.ShapeBeingDrawn->point.assign(previous_poly.begin(), previous_poly.end());
    MainBoard.ShapeBeingDrawn->point.push_back({(double) x, (double) y});
    glutPostRedisplay();
}

void InitialOpenGL(int argc, char **argv) {
    glutInit(&argc, argv);
    glHint(GL_POLYGON_SMOOTH, GL_NICEST);
    glutInitWindowSize(600, 600);
    int scw = glutGet(GLUT_SCREEN_WIDTH);
    int sch = glutGet(GLUT_SCREEN_HEIGHT);
    glutInitWindowPosition((scw - 600) / 2, (sch - 600) / 2);//center window
    halfWidth = 300;
    halfHeight = 300;

    glutCreateWindow("Board - Drawing Mode");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//白色底色

    glutDisplayFunc(&display);//绘图回调

    glutReshapeFunc(ChangeSize);//resize回调

    glutMouseFunc(&OnMouse);
    glutMotionFunc(&OnMouseDownMove);
    glutPassiveMotionFunc(&MouseMove);
    glutKeyboardUpFunc(&KeyCallback);

    glutCreateMenu(&MenuCallback);

    int shapetype_submenus = glutCreateMenu(&MenuCallback);
    glutAddMenuEntry("Line", IDM_LINE);
    glutAddMenuEntry("Rect", IDM_RECT);
    glutAddMenuEntry("Ellipse", IDM_ELLIPSE);
    glutAddMenuEntry("Polygon", IDM_POLYGON);

    glutCreateMenu(&MenuCallback);
    glutAddSubMenu("Shape ", shapetype_submenus);
    glutAddMenuEntry("Color ", IDM_COLOR);
    glutAddMenuEntry("Draw Mode", IDM_DRAWMODE);
    glutAddMenuEntry("Drag Mode", IDM_DRAGMODE);
    glutAddMenuEntry("Load Project ", IDM_LOADFILE);
    glutAddMenuEntry("Save Project ", IDM_SAVEFILE);
    glutAddMenuEntry("Help ", IDM_SHOWHELP);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    LoadProject();
    MainBoard.ShapeBeingDrawn = &eLine;
    /* 该函数让GLUT框架开始运行，所有设置的回调函数开始工作，直到用户终止程序为止 */
    glutMainLoop();
}

int main(int argc, char **argv) {
    InitialOpenGL(argc, argv);
    return 0;
}
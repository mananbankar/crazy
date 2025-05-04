#include <iostream>
#include <GL/glut.h>
#include <math.h>

using namespace std;

int m[20][3], n = 0;

void setpcolor(double r, double g, double b)
{
    glColor3f(r, g, b);
}

void conect(int x, int y, int px, int py)
{
    glPointSize(2);
    glBegin(GL_LINE_STRIP);
    glVertex2i(x, y);
    glVertex2i(px, py);
    glEnd();
    glFlush();
}

void drawPolygon()
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < n; i++)
    {
        int ni = (i + 1) % n;
        setpcolor(1, 1, 0); // yellow
        conect(m[i][0], m[i][1], m[ni][0], m[ni][1]);
    }
    glFlush();
}

void rotation(double rot, int xm, int ym)
{
    double pi = 3.14159265;
    double rad = (pi / 180.00) * rot;
    double rm[3][3] = {
        {cos(rad), sin(rad), 0},
        {-sin(rad), cos(rad), 0},
        {-xm * cos(rad) + ym * sin(rad) + xm, -xm * sin(rad) - ym * cos(rad) + ym, 1}};
    int ne[3] = {};

    for (int i = 0; i < n; i++)
    {
        ne[0] = rm[0][0] * m[i][0] + rm[0][1] * m[i][1] + rm[0][2] * m[i][2];
        ne[1] = rm[1][0] * m[i][0] + rm[1][1] * m[i][1] + rm[1][2] * m[i][2];
        ne[2] = rm[2][0] * m[i][0] + rm[2][1] * m[i][1] + rm[2][2] * m[i][2];

        m[i][0] = ne[0];
        m[i][1] = ne[1];
        m[i][2] = ne[2];
    }

    drawPolygon();
}

void scale(int sx, int sy)
{
    int sm[3][3] = {{sx, 0, 0}, {0, sy, 0}, {0, 0, 1}};
    int ne[3] = {};
    for (int i = 0; i < n; i++)
    {
        ne[0] = sm[0][0] * m[i][0] + sm[0][1] * m[i][1] + sm[0][2] * m[i][2];
        ne[1] = sm[1][0] * m[i][0] + sm[1][1] * m[i][1] + sm[1][2] * m[i][2];
        ne[2] = sm[2][0] * m[i][0] + sm[2][1] * m[i][1] + sm[2][2] * m[i][2];
        m[i][0] = ne[0];
        m[i][1] = ne[1];
        m[i][2] = ne[2];
    }

    drawPolygon();
}

void reflect(char axis)
{
    int sm[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    if (axis == 'x' || axis == 'X')
        sm[1][1] = -1;
    else if (axis == 'y' || axis == 'Y')
        sm[0][0] = -1;

    int ne[3] = {};
    for (int i = 0; i < n; i++)
    {
        ne[0] = sm[0][0] * m[i][0] + sm[0][1] * m[i][1] + sm[0][2] * m[i][2];
        ne[1] = sm[1][0] * m[i][0] + sm[1][1] * m[i][1] + sm[1][2] * m[i][2];
        ne[2] = sm[2][0] * m[i][0] + sm[2][1] * m[i][1] + sm[2][2] * m[i][2];
        m[i][0] = ne[0];
        m[i][1] = ne[1];
        m[i][2] = ne[2];
    }

    drawPolygon();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-350, 350, -350, 350);
}

void menu(int choice)
{
    switch (choice)
    {
    case 1:
        int sx, sy;
        cout << "Enter horizontal scaling factor: ";
        cin >> sx;
        cout << "Enter vertical scaling factor: ";
        cin >> sy;
        scale(sx, sy);
        break;
    case 2:
        double angle;
        int cx, cy;
        cout << "Enter arbitrary point (x, y): ";
        cin >> cx >> cy;
        cout << "Enter angle (degrees): ";
        cin >> angle;
        rotation(angle, cx, cy);
        break;
    case 3:
        char axis;
        cout << "Enter axis to reflect (X or Y): ";
        cin >> axis;
        reflect(axis);
        break;
    }
}

void mouse(int button, int state, int cx, int cy)
{
    cx -= 350;
    cy = -(cy - 350);

    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
        m[n][0] = cx;
        m[n][1] = cy;
        m[n][2] = 1;
        n++;
        if (n > 1)
        {
            setpcolor(1, 0, 0); // red
            conect(m[n - 2][0], m[n - 2][1], m[n - 1][0], m[n - 1][1]);
        }
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawPolygon();
    glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2D Transformations - Scaling, Rotation, Reflection");

    init();

    cout << "[Instructions]\n";
    cout << "- Left-click to define polygon vertices.\n";
    cout << "- Right-click to open transformation menu.\n";

    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutCreateMenu(menu);
    glutAddMenuEntry("Scaling", 1);
    glutAddMenuEntry("Rotation about arbitrary point", 2);
    glutAddMenuEntry("Reflection (X or Y axis)", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}


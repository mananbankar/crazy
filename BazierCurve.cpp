
#include<bits/stdc++.h>
#include <stdio.h>
#include<iostream>
#include<GL/glut.h>
#include<math.h>

class Point
{
public:
    int x, y;
    void setxy(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
};


static int POINTSNUM = 0;


static Point points[4];

void init(void)
{
    glClearColor(0.0, 0.0, 0.0,0);
    glColor3f(1.0, 1.0, 1.0); 
    glPointSize(4.0); 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluOrtho2D(0.0, 600.0, 0.0, 480.0); 
}


void setPoint(Point p)
{
    glBegin(GL_POINTS);
    glVertex2f(p.x, p.y);
    glEnd();
    glFlush();
}


void setline(Point p1, Point p2)
{
    glBegin(GL_LINES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();
    glFlush();
}

// Draw Bezier curve
Point setBezier(Point p1, Point p2, Point p3, Point p4, double t)
{
    Point p;
    double a1 = pow((1 - t), 3);
    double a2 = pow((1 - t), 2) * 3 * t;
    double a3 = 3 * t*t*(1 - t);
    double a4 = t * t*t;
    p.x = a1 * p1.x + a2 * p2.x + a3 * p3.x + a4 * p4.x;
    p.y = a1 * p1.y + a2 * p2.y + a3 * p3.y + a4 * p4.y;
    return p;
}


//display function
void display()
{
    //glClear(GL_COLOR_BUFFER_BIT);
    //glFlush();
}



// mouse event
void mymouseFunction(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) 
    {
        points[POINTSNUM].setxy(x, 480 - y); 
        glColor3f(1.0, 0.0, 0.0);
        setPoint(points[POINTSNUM]);
        
        glColor3f(1.0, 0.0, 0.0);
        if (POINTSNUM > 0) setline(points[POINTSNUM - 1], points[POINTSNUM]);

        
        if (POINTSNUM == 3)
        {
           
            glColor3f(1.0, 1.0, 0.0); 
            Point p_current = points[0]; 
            for (double t = 0.0; t <= 1.0; t += 0.05)
            {
                Point P = setBezier(points[0], points[1], points[2], points[3], t);
                setline(p_current, P);
                p_current = P;
            }

            POINTSNUM = 0;
        }
        else
        {
            POINTSNUM++;
        }
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);   
    glutInitWindowSize(600, 480);   
    glutInitWindowPosition(100, 100); 
    glutCreateWindow("Bezier curve");

    init(); 
    glutMouseFunc(mymouseFunction); 
    glutDisplayFunc(display); 
    glutMainLoop();
    return 0;
}

#include <GL/glut.h>
#include <cmath>
#include <iostream>

int centerX = 0, centerY = 0;
int radius = 0;
int selectedQuadrant = 0; // 1: Q1, 2: Q2, 3: Q3, 4: Q4
bool isCenterSet = false;  // Flag to check if center is set
bool isRadiusSet = false;  // Flag to check if radius is set

void plotPoint(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void bresenhamCircle(int radius) {
    int x = 0;
    int y = radius;
    int p = 3 - 2 * radius;

 
    while (x <= y) {
        if (selectedQuadrant == 1) { // Quadrant 1
            plotPoint(centerX + x, centerY + y);
            plotPoint(centerX - x, centerY + y);
            plotPoint(centerX + x, centerY - y);
            plotPoint(centerX - x, centerY - y);
            plotPoint(centerX + y, centerY + x);
            plotPoint(centerX - y, centerY + x);
            plotPoint(centerX + y, centerY - x);
            plotPoint(centerX - y, centerY - x);
        } else if (selectedQuadrant == 2) { // Quadrant 2
            plotPoint(centerX - x, centerY + y);
            plotPoint(centerX + x, centerY + y);
            plotPoint(centerX - x, centerY - y);
            plotPoint(centerX + x, centerY - y);
            plotPoint(centerX - y, centerY + x);
            plotPoint(centerX + y, centerY + x);
            plotPoint(centerX - y, centerY - x);
            plotPoint(centerX + y, centerY - x);
        } else if (selectedQuadrant == 3) { // Quadrant 3
            plotPoint(centerX - x, centerY - y);
            plotPoint(centerX + x, centerY - y);
            plotPoint(centerX - x, centerY + y);
            plotPoint(centerX + x, centerY + y);
            plotPoint(centerX - y, centerY - x);
            plotPoint(centerX + y, centerY - x);
            plotPoint(centerX - y, centerY + x);
            plotPoint(centerX + y, centerY + x);
        } else if (selectedQuadrant == 4) { // Quadrant 4
            plotPoint(centerX + x, centerY - y);
            plotPoint(centerX - x, centerY - y);
            plotPoint(centerX + x, centerY + y);
            plotPoint(centerX - x, centerY + y);
            plotPoint(centerX + y, centerY - x);
            plotPoint(centerX - y, centerY - x);
            plotPoint(centerX + y, centerY + x);
            plotPoint(centerX - y, centerY + x);
        }
        if (p < 0) {
            p = p + 4 * x + 6;
        } else {
            p = p + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}


void drawQuadrants() {
    glColor3f(1.0f, 1.0f, 1.0f);  // Set color to white

    
    glBegin(GL_LINES);
        glVertex2i(0, 300); glVertex2i(0, -300);  // Vertical line
        glVertex2i(-300, 0); glVertex2i(300, 0);  // Horizontal line
    glEnd();
}


void mouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int window_width = 600;
        int window_height = 600;
        int screenCenterX = window_width / 2;
        int screenCenterY = window_height / 2;

    
        int openglY = screenCenterY - y;  

        if (!isCenterSet) {
            // First click to set the center of the circle
            centerX = x - screenCenterX;  
            centerY = openglY;
            isCenterSet = true; 
            std::cout << "Center Set at: (" << centerX << ", " << centerY << ")\n";
        } else if (!isRadiusSet) {
            // Second click to set the radius of the circle
            int dx = x - screenCenterX - centerX;  
            int dy = openglY - centerY;            
            radius = static_cast<int>(sqrt(dx * dx + dy * dy));  
            isRadiusSet = true; // Radius has been set
            std::cout << "Radius Set: " << radius << "\n";
        } else {
            
            if (x < screenCenterX && y > screenCenterY) {
                selectedQuadrant = 2;  // Quadrant 2 (top-left)
            } else if (x > screenCenterX && y > screenCenterY) {
                selectedQuadrant = 1;  // Quadrant 1 (top-right)
            } else if (x < screenCenterX && y < screenCenterY) {
                selectedQuadrant = 3;  // Quadrant 3 (bottom-left)
            } else if (x > screenCenterX && y < screenCenterY) {
                selectedQuadrant = 4;  // Quadrant 4 (bottom-right)
            }

            
            glutPostRedisplay();
        }
    }
}


void keyboardFunc(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {
        // Reset center, radius, and selected quadrant on 'r' key press
        isCenterSet = false;
        isRadiusSet = false;
        selectedQuadrant = 0;
        radius = 0;
        centerX = 0;
        centerY = 0;
        std::cout << "Resetting Circle\n";  
        glutPostRedisplay(); 
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT); 

    
    drawQuadrants();

   
    glColor3f(1.0f, 1.0f, 1.0f);  // Set color to white
    if (isCenterSet && isRadiusSet && selectedQuadrant != 0) {
        bresenhamCircle(radius);  
    }

    glFlush(); 
}


void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-300, 300, -300, 300, -1.0, 1.0);  
    glPointSize(2.0);  
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Bresenham Circle Drawing with Mouse & Quadrant Selection");

    initOpenGL();  
    glutDisplayFunc(display); 
    glutMouseFunc(mouseFunc); 
    glutKeyboardFunc(keyboardFunc);  /
    glutMainLoop();  

    return 0;
}

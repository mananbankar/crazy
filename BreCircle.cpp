#include <GL/glut.h>
#include <cmath>
#include <iostream>

// Global variables for center, radius, and selected quadrant
int centerX = 0, centerY = 0;
int radius = 0;
int selectedQuadrant = 0; // 1: Q1, 2: Q2, 3: Q3, 4: Q4
bool isCenterSet = false;  // Flag to check if center is set
bool isRadiusSet = false;  // Flag to check if radius is set

// Function to plot points at the given (x, y)
void plotPoint(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Bresenham's Circle Drawing Algorithm
void bresenhamCircle(int radius) {
    int x = 0;
    int y = radius;
    int p = 3 - 2 * radius;

    // Draw the circle in the selected quadrant
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

// Function to draw the division lines (quadrants)
void drawQuadrants() {
    glColor3f(1.0f, 1.0f, 1.0f);  // Set color to white

    // Draw the vertical and horizontal lines to divide the window into 4 quadrants
    glBegin(GL_LINES);
        glVertex2i(0, 300); glVertex2i(0, -300);  // Vertical line
        glVertex2i(-300, 0); glVertex2i(300, 0);  // Horizontal line
    glEnd();
}

// Function to handle mouse clicks and capture center, radius, and quadrant selection
void mouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int window_width = 600;
        int window_height = 600;
        int screenCenterX = window_width / 2;
        int screenCenterY = window_height / 2;

        // Convert mouse click position to OpenGL coordinates
        int openglY = screenCenterY - y;  // Adjust for OpenGL coordinate system

        if (!isCenterSet) {
            // First click to set the center of the circle
            centerX = x - screenCenterX;  // Translate click to OpenGL coordinates
            centerY = openglY;
            isCenterSet = true; // Center has been set
            std::cout << "Center Set at: (" << centerX << ", " << centerY << ")\n";
        } else if (!isRadiusSet) {
            // Second click to set the radius of the circle
            int dx = x - screenCenterX - centerX;  // Distance from center in x
            int dy = openglY - centerY;            // Distance from center in y
            radius = static_cast<int>(sqrt(dx * dx + dy * dy));  // Calculate the distance (radius)
            isRadiusSet = true; // Radius has been set
            std::cout << "Radius Set: " << radius << "\n";
        } else {
            // Third click to select the quadrant (divide the window into 4 quadrants)
            if (x < screenCenterX && y > screenCenterY) {
                selectedQuadrant = 2;  // Quadrant 2 (top-left)
            } else if (x > screenCenterX && y > screenCenterY) {
                selectedQuadrant = 1;  // Quadrant 1 (top-right)
            } else if (x < screenCenterX && y < screenCenterY) {
                selectedQuadrant = 3;  // Quadrant 3 (bottom-left)
            } else if (x > screenCenterX && y < screenCenterY) {
                selectedQuadrant = 4;  // Quadrant 4 (bottom-right)
            }

            // Redraw the window after setting the quadrant
            glutPostRedisplay();
        }
    }
}

// Function to handle keyboard input
void keyboardFunc(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {
        // Reset center, radius, and selected quadrant on 'r' key press
        isCenterSet = false;
        isRadiusSet = false;
        selectedQuadrant = 0;
        radius = 0;
        centerX = 0;
        centerY = 0;
        std::cout << "Resetting Circle\n";  // Inform user of reset
        glutPostRedisplay();  // Redraw the window
    }
}

// Display function to set up the OpenGL window
void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen

    // Draw the quadrants first
    drawQuadrants();

    // Draw the circle if center and radius are set
    glColor3f(1.0f, 1.0f, 1.0f);  // Set color to white
    if (isCenterSet && isRadiusSet && selectedQuadrant != 0) {
        bresenhamCircle(radius);  // Draw the circle in the selected quadrant
    }

    glFlush();  // Render the scene
}

// Initialize OpenGL settings
void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Set background color to black
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-300, 300, -300, 300, -1.0, 1.0);  // Set orthographic view with origin at center
    glPointSize(2.0);  // Set point size
}

// Main function
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Bresenham Circle Drawing with Mouse & Quadrant Selection");

    initOpenGL();  // Initialize OpenGL settings
    glutDisplayFunc(display);  // Set display callback function
    glutMouseFunc(mouseFunc);  // Set mouse click callback function
    glutKeyboardFunc(keyboardFunc);  // Set keyboard input callback function
    glutMainLoop();  // Enter the main loop

    return 0;
}

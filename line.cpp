#include <graphics.h>
#include <iostream>
#include <cmath>

// Function declarations
void drawDDA(int x1, int y1, int x2, int y2, int lineType);
void drawBresenham(int x1, int y1, int x2, int y2, int lineType);
void drawSolidLine(int x1, int y1, int x2, int y2, void (*drawFunc)(int, int, int, int, int));
void drawDottedLine(int x1, int y1, int x2, int y2, void (*drawFunc)(int, int, int, int, int));
void drawDashedLine(int x1, int y1, int x2, int y2, void (*drawFunc)(int, int, int, int, int));
void mouseClickHandler(int &x1, int &y1, int &x2, int &y2);

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, nullptr);

    int x1, y1, x2, y2;
    int choice;

    // Divide the screen into four quadrants
    line(getmaxx() / 2, 0, getmaxx() / 2, getmaxy()); // Vertical line
    line(0, getmaxy() / 2, getmaxx(), getmaxy() / 2); // Horizontal line

    // Mouse click to get start and end points
    mouseClickHandler(x1, y1, x2, y2);

    // Display menu for line types
    outtextxy(10, 10, (char*)"Select Line Type: 1. DDA 2. Bresenham");
    outtextxy(10, 30, (char*)"3. Dotted Line 4. Dashed Line 5. Solid Line");

    std::cout << "Enter your choice (1-5): ";
    std::cin >> choice;

    // Call respective functions
    switch (choice) {
        case 1:
            drawDDA(x1, y1, x2, y2, 0);
            break;
        case 2:
            drawBresenham(x1, y1, x2, y2, 0);
            break;
        case 3:
            drawDottedLine(x1, y1, x2, y2, drawDDA);
            break;
        case 4:
            drawDashedLine(x1, y1, x2, y2, drawDDA);
            break;
        case 5:
            drawSolidLine(x1, y1, x2, y2, drawDDA);
            break;
        default:
            outtextxy(10, 50, (char*)"Invalid choice");
    }

    std::cin.ignore(); // Flush leftover input
    std::cin.get();    // Wait for Enter key
    closegraph();
    return 0;
}

// Mouse click handler
void mouseClickHandler(int &x1, int &y1, int &x2, int &y2) {
    outtextxy(10, 60, (char*)"Click first point with left mouse button...");
    while (!ismouseclick(WM_LBUTTONDOWN));
    getmouseclick(WM_LBUTTONDOWN, x1, y1);

    outtextxy(10, 80, (char*)"Click second point with left mouse button...");
    while (!ismouseclick(WM_LBUTTONDOWN));
    getmouseclick(WM_LBUTTONDOWN, x2, y2);
}

// DDA Line Drawing
void drawDDA(int x1, int y1, int x2, int y2, int lineType) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    int steps = (std::abs(dx) > std::abs(dy)) ? std::abs(dx) : std::abs(dy);
    float xIncrement = dx / steps;
    float yIncrement = dy / steps;
    float x = x1;
    float y = y1;

    for (int k = 0; k <= steps; k++) {
        if (lineType == 0 || (lineType == 1 && k % 2 == 0) || (lineType == 2 && k % 5 == 0)) {
            putpixel(std::round(x), std::round(y), WHITE);
        }
        x += xIncrement;
        y += yIncrement;
    }
}

// Bresenham Line Drawing
void drawBresenham(int x1, int y1, int x2, int y2, int lineType) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int p = 2 * dy - dx;
    int twoDy = 2 * dy;
    int twoDyMinusDx = 2 * (dy - dx);
    int x = x1, y = y1;

    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    for (int i = x1; i <= x2; ++i) {
        if (lineType == 0 || (lineType == 1 && i % 2 == 0) || (lineType == 2 && i % 5 == 0)) {
            putpixel(x, y, WHITE);
        }

        if (p < 0) {
            p += twoDy;
        } else {
            y += (y1 < y2) ? 1 : -1;
            p += twoDyMinusDx;
        }
        x++;
    }
}

void drawSolidLine(int x1, int y1, int x2, int y2, void (*drawFunc)(int, int, int, int, int)) {
    drawFunc(x1, y1, x2, y2, 0);
}

void drawDottedLine(int x1, int y1, int x2, int y2, void (*drawFunc)(int, int, int, int, int)) {
    drawFunc(x1, y1, x2, y2, 1);
}

void drawDashedLine(int x1, int y1, int x2, int y2, void (*drawFunc)(int*


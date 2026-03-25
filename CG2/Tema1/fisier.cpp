#include <cstdlib>
#include <cmath>
#include <iostream>
#include "glut.h"

int defaultW = 800, defaultH = 800;
unsigned char prevKey;

int gridCols = 40;
int gridRows = 40;
float margin = 0.05f;

void drawGrid() {
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_LINES);
    float stepX = (2.0f - 2 * margin) / gridCols;
    float stepY = (2.0f - 2 * margin) / gridRows;

    for (int i = 0; i <= gridCols; i++) {
        float x = -1.0f + margin + i * stepX;
        glVertex2f(x, -1.0f + margin);
        glVertex2f(x, 1.0f - margin);
    }
    for (int i = 0; i <= gridRows; i++) {
        float y = -1.0f + margin + i * stepY;
        glVertex2f(-1.0f + margin, y);
        glVertex2f(1.0f - margin, y);
    }
    glEnd();
}

void writePixel(int x, int y) {
    float stepX = (2.0f - 2 * margin) / gridCols;
    float stepY = (2.0f - 2 * margin) / gridRows;

    float cx = -1.0f + margin + x * stepX + stepX / 2.0f;
    float cy = -1.0f + margin + y * stepY + stepY / 2.0f;
    float radius = (stepX < stepY ? stepX : stepY) * 0.35f;

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 36; i++) {
        float theta = i * 3.14159f * 2.0f / 36.0f;
        glVertex2f(cx + radius * cos(theta), cy + radius * sin(theta));
    }
    glEnd();
}

void writePixelThick(int x, int y) {
    writePixel(x, y);
    writePixel(x + 1, y);
    writePixel(x, y + 1);
    writePixel(x - 1, y);
    writePixel(x, y - 1);
}

void drawIdealLine(int x0, int y0, int x1, int y1) {
    float stepX = (2.0f - 2 * margin) / gridCols;
    float stepY = (2.0f - 2 * margin) / gridRows;

    float cx0 = -1.0f + margin + x0 * stepX + stepX / 2.0f;
    float cy0 = -1.0f + margin + y0 * stepY + stepY / 2.0f;
    float cx1 = -1.0f + margin + x1 * stepX + stepX / 2.0f;
    float cy1 = -1.0f + margin + y1 * stepY + stepY / 2.0f;

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(cx0, cy0);
    glVertex2f(cx1, cy1);
    glEnd();
}

void drawIdealCircle(int xc, int yc, int r) {
    float stepX = (2.0f - 2 * margin) / gridCols;
    float stepY = (2.0f - 2 * margin) / gridRows;

    float cx = -1.0f + margin + xc * stepX + stepX / 2.0f;
    float cy = -1.0f + margin + yc * stepY + stepY / 2.0f;
    float realRadius = r * stepX;

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++) {
        float theta = i * 3.14159f * 2.0f / 100.0f;
        glVertex2f(cx + realRadius * cos(theta), cy + realRadius * sin(theta));
    }
    glEnd();
}

void drawLineOctant0(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int d = 2 * dy - dx;
    int dE = 2 * dy;
    int dNE = 2 * (dy - dx);
    int x = x0;
    int y = y0;

    writePixelThick(x, y);
    while (x < x1) {
        if (d <= 0) {
            d += dE;
            x++;
        } else {
            d += dNE;
            x++;
            y++;
        }
        writePixelThick(x, y);
    }
}

void drawLineOctant1(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int d = 2 * dx - dy;
    int dN = 2 * dx;
    int dNE = 2 * (dx - dy);
    int x = x0;
    int y = y0;

    writePixelThick(x, y);
    while (y < y1) {
        if (d <= 0) {
            d += dN;
            y++;
        } else {
            d += dNE;
            x++;
            y++;
        }
        writePixelThick(x, y);
    }
}

void drawLineOctant7(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y0 - y1;
    int d = 2 * dy - dx;
    int dE = 2 * dy;
    int dSE = 2 * (dy - dx);
    int x = x0;
    int y = y0;

    writePixelThick(x, y);
    while (x < x1) {
        if (d <= 0) {
            d += dE;
            x++;
        } else {
            d += dSE;
            x++;
            y--;
        }
        writePixelThick(x, y);
    }
}

void drawLineOctant6(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y0 - y1;
    int d = 2 * dx - dy;
    int dS = 2 * dx;
    int dSE = 2 * (dx - dy);
    int x = x0;
    int y = y0;

    writePixelThick(x, y);
    while (y > y1) {
        if (d <= 0) {
            d += dS;
            y--;
        } else {
            d += dSE;
            x++;
            y--;
        }
        writePixelThick(x, y);
    }
}

void drawLine(int x0, int y0, int x1, int y1) {
    drawIdealLine(x0, y0, x1, y1);

    if (x0 > x1) {
        int aux = x0; x0 = x1; x1 = aux;
        aux = y0; y0 = y1; y1 = aux;
    }

    int dx = x1 - x0;
    int dy = y1 - y0;

    if (dy >= 0) {
        if (dy <= dx) {
            drawLineOctant0(x0, y0, x1, y1);
        } else {
            drawLineOctant1(x0, y0, x1, y1);
        }
    } else {
        if (-dy <= dx) {
            drawLineOctant7(x0, y0, x1, y1);
        } else {
            drawLineOctant6(x0, y0, x1, y1);
        }
    }
}

void drawCircleQ1(int xc, int yc, int r, int mode) {
    int x = 0;
    int y = r;
    int d = 1 - r;
    int dE = 3;
    int dSE = -2 * r + 5;

    while (y >= x) {
        if (mode == 1) {
            for (int i = xc; i <= xc + x; i++) writePixel(i, yc + y);
            for (int i = xc; i <= xc + y; i++) writePixel(i, yc + x);
        } else {
            writePixel(xc + x, yc + y);
            writePixel(xc + y, yc + x);
        }

        if (d < 0) {
            d += dE;
            dE += 2;
            dSE += 2;
        } else {
            d += dSE;
            dE += 2;
            dSE += 4;
            y--;
        }
        x++;
    }
}

void drawCircleQ2(int xc, int yc, int r, int mode) {
    int x = 0;
    int y = r;
    int d = 1 - r;
    int dE = 3;
    int dSE = -2 * r + 5;

    while (y >= x) {
        if (mode == 1) {
            for (int i = xc - x; i <= xc; i++) writePixel(i, yc + y);
            for (int i = xc - y; i <= xc; i++) writePixel(i, yc + x);
        } else {
            writePixel(xc - x, yc + y);
            writePixel(xc - y, yc + x);
        }

        if (d < 0) {
            d += dE;
            dE += 2;
            dSE += 2;
        } else {
            d += dSE;
            dE += 2;
            dSE += 4;
            y--;
        }
        x++;
    }
}

void drawCircleQ3(int xc, int yc, int r, int mode) {
    int x = 0;
    int y = r;
    int d = 1 - r;
    int dE = 3;
    int dSE = -2 * r + 5;

    while (y >= x) {
        if (mode == 1) {
            for (int i = xc - x; i <= xc; i++) writePixel(i, yc - y);
            for (int i = xc - y; i <= xc; i++) writePixel(i, yc - x);
        } else {
            writePixel(xc - x, yc - y);
            writePixel(xc - y, yc - x);
        }

        if (d < 0) {
            d += dE;
            dE += 2;
            dSE += 2;
        } else {
            d += dSE;
            dE += 2;
            dSE += 4;
            y--;
        }
        x++;
    }
}

void drawCircleQ4(int xc, int yc, int r, int mode) {
    int x = 0;
    int y = r;
    int d = 1 - r;
    int dE = 3;
    int dSE = -2 * r + 5;

    while (y >= x) {
        if (mode == 1) {
            for (int i = xc; i <= xc + x; i++) writePixel(i, yc - y);
            for (int i = xc; i <= xc + y; i++) writePixel(i, yc - x);
        } else {
            writePixel(xc + x, yc - y);
            writePixel(xc + y, yc - x);
        }

        if (d < 0) {
            d += dE;
            dE += 2;
            dSE += 2;
        } else {
            d += dSE;
            dE += 2;
            dSE += 4;
            y--;
        }
        x++;
    }
}

void drawCircle(int xc, int yc, int r, int mode) {
    drawIdealCircle(xc, yc, r);
    drawCircleQ1(xc, yc, r, mode);
    drawCircleQ2(xc, yc, r, mode);
    drawCircleQ3(xc, yc, r, mode);
    drawCircleQ4(xc, yc, r, mode);
}

void Display1() {
    drawGrid();
    drawLine(5, 5, 35, 15);
}

void Display2() {
    drawGrid();

    int points = 16;
    int cx = 20, cy = 20, r = 16;
    int vx[16];
    int vy[16];

    for (int i = 0; i < points; i++) {
        float angle = i * 6.28318f / points + 0.15f;
        vx[i] = cx + r * std::cos(angle);
        vy[i] = cy + r * std::sin(angle);
    }

    for (int i = 0; i < points; i++) {
        drawLine(vx[i], vy[i], vx[(i + 1) % points], vy[(i + 1) % points]);
    }
}

void Display3() {
    drawGrid();
    drawCircle(20, 20, 16, 0);
}

void Display4() {
    drawGrid();
    drawCircle(20, 20, 16, 1);
}

void Display5() {}
void Display6() {}
void Display7() {}
void Display8() {}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLineWidth(2);
    glPointSize(4);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    switch(prevKey) {
    case '1':
        Display1();
        break;
    case '2':
        Display2();
        break;
    case '3':
        Display3();
        break;
    case '4':
        Display4();
        break;
    case '5':
        Display5();
        break;
    case '6':
        Display6();
        break;
    case '7':
        Display7();
        break;
    case '8':
        Display8();
        break;
    default:
        break;
    }

    glFlush();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) {
        gluOrtho2D(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w);
    } else {
        gluOrtho2D(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27)
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(defaultW, defaultH);
    glutInitWindowPosition(-1, -1);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Rasterizare");

    init();

    glutReshapeFunc(Reshape);
    glutKeyboardFunc(KeyboardFunc);
    glutMouseFunc(MouseFunc);
    glutDisplayFunc(Display);

    glutMainLoop();
    return 0;
}
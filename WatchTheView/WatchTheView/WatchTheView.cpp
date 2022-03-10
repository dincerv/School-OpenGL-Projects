/*********
----------
STUDENT : Dincer Velioglu
----------
*********/


#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include<time.h>

#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int currentPosOfX, currentPosOfY;

bool activeTimer = true;

int xB = 0, xS = 0;

int randX, randY;

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle2(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}


void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void drawWall()
{
	glColor3ub(255, 0, 0);
	glBegin(GL_LINES);
	// line 1
	for (int i = -600; i <= 0; i = i + 20)
	{
		glColor3f(0, 0, 0);
		glVertex2f(i, 600);
		glVertex2f(i, -600);
	}
	for (int i = 600; i > 0; i = i - 20)
	{
		glColor3f(0, 0, 0);
		glVertex2f(i, 600);
		glVertex2f(i, -600);
	}
	glEnd();
}

void drawTable()
{
	glBegin(GL_POLYGON);
	glColor3f(0.4f, 0.0f, 0.0f);
	glVertex2f(-125, -250);
	glVertex2f(-75, -200);
	glVertex2f(75, -200);
	glVertex2f(125, -250);
	glEnd();

	glColor3f(0.4f, 0.0f, 0.0f);
	glRectf(-125, -350, -115, -250);

	glColor3f(0.4f, 0.0f, 0.0f);
	glRectf(-75, -300, -65, -200);

	glColor3f(0.4f, 0.0f, 0.0f);
	glRectf(125, -350, 115, -250);

	glColor3f(0.4f, 0.0f, 0.0f);
	glRectf(75, -300, 65, -200);
}

void drawUpperWindow()
{
	glColor3f(1.0f, 0.5f, 0.0f);
	circle(0, 150, 250);

	glColor3f(0.4f, 0.0f, 0.0f);
	circle(0, 150, 235);

	glColor3f(0.4f, 0.0f, 0.0f);
	circle(0, 100, 245);

	glColor3f(0.0, 0.0, 0.8);
	circle(0, 150, 220);
}

void drawWindow()
{
	glColor3f(1.0f, 0.5f, 0.0f);
	glRectf(-250, -150, 250, 150);

	glColor3f(0.4f, 0.0f, 0.0f);
	glRectf(-240, -135, 240, 135);

	glColor3f(0.4f, 0.0f, 0.0f);
	glRectf(-240, -135, 240, 135);

	glColor3f(0.0, 0.0, 0.8);
	glRectf(-220, -125, 220, 125);

}


void drawBackGround()//mountains
{
	glColor3f(0.9f, 1.0, 0.8f);
	circle(-140, 35, 75);
	glColor3f(0.9f, 1.0, 0.8f);
	circle(-50, 35, 75);
	glColor3f(0.9f, 1.0, 0.8f);
	circle(0, 35, 75);
	glColor3f(0.9f, 1.0, 0.8f);
	circle(125, 35, 75);
}

void drawSky()
{

	glColor3f(1, 0.9, 0);
	circle2(-125, 290, 25);//sun

	glColor3f(1, 1, 1);//cloud
	circle2(135, 290, 20);
	circle2(100, 290, 30);
	circle2(70, 285, 20);

	circle2(-30, 220, 20);
	circle2(0, 210, 30);
	circle2(30, 205, 20);

	glColor3f(1, 1, 1);//cloud
	circle2(-120, 280, 20);
	circle2(-90, 280, 30);
	circle2(-65, 275, 20);


	glColor3f(0.4f, 0.0f, 0.0f);
	glRectf(-5, 150, 5, 370);
}

void drawRoof()
{
	glColor3f(0.5, 0.0, 0.0);//roof1
	glBegin(GL_TRIANGLES);
	glVertex2f(50, 70);
	glVertex2f(100, 70);
	glVertex2f(75, 100);

	glColor3f(0.5, 0.0, 0.0);//roof2
	glVertex2f(100, 70);
	glVertex2f(150, 70);
	glVertex2f(125, 100);

	glColor3f(0.5, 0.0, 0.0);//roof3
	glVertex2f(150, 70);
	glVertex2f(200, 70);
	glVertex2f(175, 100);
	glEnd();
}

void drawhighWay()
{
	glColor3f(0.5f, 0.5f, 0.5f);//road
	glRectf(-220, -80, 220, 10);


	for (int i = -200; i < 200; i += 50)
	{
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex2f(i, -37);
		glVertex2f(i + 5, -32);
		glVertex2f(i + 39, -32);
		glVertex2f(i + 35, -37);
		glEnd();
	}

	glColor3f(0.0f, 0.0, 0.0f);//kaldýrým
	glRectf(-220, -80, 220, -90);

	glColor3f(0.0f, 0.0, 0.0f);//kaldýrým
	glRectf(-220, 10, 220, 15);

	glColor3f(0.0f, 0.6, 0.0f);//çim
	glRectf(-220, -90, 220, -125);
}

void drawView()
{
	glColor3f(0.0f, 0.6, 0.0f);//grass
	glRectf(-220, 10, 220, 35);

	glColor3f(0.4, 0.2, 0.1);//1.tree
	glRectf(-200, 10, -190, 50);
	glColor3f(0.0f, 0.6, 0.0f);
	circle2(-195, 60, 20);

	glColor3f(0.4, 0.2, 0.1);//2.tree
	glRectf(-150, 20, -145, 55);
	glColor3f(0.0f, 0.6, 0.0f);
	circle2(-147.5, 60, 10);

	glColor3f(0.4, 0.2, 0.1);//3.tree
	glRectf(-100, 10, -90, 50);
	glColor3f(0.0f, 0.6, 0.0f);
	circle2(-95, 60, 20);

	glColor3f(0.4, 0.2, 0.1);//4.tree
	glRectf(-50, 20, -45, 55);
	glColor3f(0.0f, 0.6, 0.0f);
	circle2(-47.5, 60, 10);

	glColor3f(0.0f, 0.9, 0.5f);//house
	glRectf(50, 10, 100, 70);

	glColor3f(0.2f, 0.3, 0.5f);//house2
	glRectf(100, 10, 150, 70);

	glColor3f(0.9f, 0.9, 0.1f);//house3
	glRectf(150, 10, 200, 70);

	glColor3f(1.0f, 1.0, 1.0f);//window1
	glRectf(60, 50, 90, 60);
	glColor3f(0.0f, 0.0, 0.0f);
	glRectf(74, 50, 76, 60);
	glColor3f(0.0f, 0.0, 0.0f);
	glRectf(60, 54, 90, 56);

	glColor3f(1.0f, 1.0, 1.0f);//window2
	glRectf(110, 50, 140, 60);
	glColor3f(0.0f, 0.0, 0.0f);
	glRectf(124, 50, 126, 60);
	glColor3f(0.0f, 0.0, 0.0f);
	glRectf(110, 54, 140, 56);

	glColor3f(1.0f, 1.0, 1.0f);//window3
	glRectf(160, 50, 190, 60);
	glColor3f(0.0f, 0.0, 0.0f);
	glRectf(174, 50, 176, 60);
	glColor3f(0.0f, 0.0, 0.0f);
	glRectf(160, 54, 190, 56);


	glColor3f(0.5f, 0.0, 0.3f);//door1
	glRectf(67.5, 10, 82.5, 40);

	glColor3f(0.5f, 0.0, 0.3f);//door2
	glRectf(117.5, 10, 132.5, 40);

	glColor3f(0.5f, 0.0, 0.3f);//door3
	glRectf(167.5, 10, 182.5, 40);
}


void drawMiddleWindow()
{

	glColor3f(0.4f, 0.0f, 0.0f);
	glRectf(-5, -135, 5, 135);

	glColor3f(0.4f, 0.0f, 0.0f);
	glRectf(-5, -135, 15, 135);

	glColor3f(0.4f, 0.0f, 0.0f);
	glRectf(-15, -135, -5, 135);

	glColor3f(1.0, 0.9, 0.8);
	glRectf(-245, -130, 245, -145); //middle window

	glColor3f(1.0, 0.9, 0.8);
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex2f(7.5, 15);
	glVertex2f(7.5, -15);
	glVertex2f(-7.5, 15);
	glVertex2f(-7.5, -15);
	glEnd();

	glBegin(GL_LINES);
	// line 1
	glColor3f(1.0, 0.9, 0.8);
	glVertex2f(7.5, 15);
	glVertex2f(-7.5, -15);

	glColor3f(1.0, 0.9, 0.8);
	glVertex2f(7.5, -15);
	glVertex2f(-7.5, 15);
	glEnd();
}

void drawCar()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Bottom

	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.3);
	glVertex2f(xB - 175, -10);
	glVertex2f(xB - 175, 10);
	glVertex2f(xB - 165, 25);
	glVertex2f(xB - 125, 25);
	glVertex2f(xB - 115, 10);
	glVertex2f(xB - 100, 10);
	glVertex2f(xB - 100, -10);
	glEnd();

	//other-window
	glBegin(GL_POLYGON);
	glColor3f(0.7, 0.2, 0.1);
	glVertex2f(xB - 175, 10);
	glVertex2f(xB - 165, 25);
	glVertex2f(xB - 125, 25);
	glVertex2f(xB - 115, 10);
	glEnd();

	//egsos
	glColor3f(0, 0, 0);
	glRectf(xB - 183, -10, xB - 175, -4);

	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2f(xB - 175, -10);
	glVertex2f(xB - 100, -10);
	glVertex2f(xB - 165, 25);
	glVertex2f(xB - 125, 25);
	glVertex2f(xB - 175, -10);
	glVertex2f(xB - 175, 10);
	glVertex2f(xB - 175, 10);
	glVertex2f(xB - 165, 25);
	glVertex2f(xB - 125, 25);
	glVertex2f(xB - 115, 10);
	glVertex2f(xB - 115, 10);
	glVertex2f(xB - 100, 10);
	glVertex2f(xB - 100, 10);
	glVertex2f(xB - 100, -10);
	//Inside--Windows
	glVertex2f(xB - 175, 10);
	glVertex2f(xB - 100, 10);
	glVertex2f(xB - 145, 25);
	glVertex2f(xB - 145, 10);
	glEnd();

	//door
	glColor3f(1, 1, 1);
	glRectf(xB - 140, 6, xB - 135, 4);
	glRectf(xB - 165, 6, xB - 160, 4);
	//wheels
	glColor3f(0.0, 0.0, 0.0);
	circle2(xB - 115, -10, 7);
	circle2(xB - 160, -10, 7);
	glColor3f(0.5f, 0.5f, 0.5f);
	circle2(xB - 115, -10, 3);
	circle2(xB - 160, -10, 3);


	//lights
	glColor3f(1, 1, 1);
	circle(xB - 100, 8, 4);
	glColor3f(1, 0, 0);
	circle(xB - 100, -8, 2);
}


void fakeWindow()
{
	glColor3f(0.9f, 0.6, 0.0f);//pot
	circle2(375, 40, 20);
	glColor3f(0.4f, 0.4, 0.0f);//flower
	circle2(270, 60, 15);
	glColor3f(0.8f, 0.6, 0.7f);
	circle2(350, 50, 10);
	glColor3f(0.2f, 0.6, 0.6f);
	circle2(300, 55, 20);

	glBegin(GL_POLYGON);
	glColor3f(0.4, 0.0, 0.0);
	glVertex2f(220, 40);
	glVertex2f(220, -20);
	glVertex2f(240, -20);
	glVertex2f(240, 40);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.4, 0.0, 0.0);
	glVertex2f(-220, 40);
	glVertex2f(-220, -20);
	glVertex2f(-240, -20);
	glVertex2f(-240, 40);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex2f(250, 40);
	glVertex2f(250, -20);
	glVertex2f(240, -20);
	glVertex2f(240, 40);
	glEnd();

	glColor3f(0.9f, 0.6, 0.0f);//pot
	circle2(-375, 40, 20);
	glColor3f(0.4f, 0.4, 0.0f);//flower
	circle2(-270, 60, 15);
	glColor3f(0.8f, 0.6, 0.7f);//
	circle2(-350, 50, 10);
	glColor3f(0.2f, 0.6, 0.6f);//
	circle2(-300, 55, 20);

	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex2f(-250, 40);
	glVertex2f(-250, -20);
	glVertex2f(-240, -20);
	glVertex2f(-240, 40);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1, 0.8, 0.3);
	glVertex2f(250, 40);
	glVertex2f(250, -40);
	glVertex2f(400, -40);
	glVertex2f(400, 40);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1, 0.8, 0.3);
	glVertex2f(-250, 40);
	glVertex2f(-250, -40);
	glVertex2f(-400, -40);
	glVertex2f(-400, 40);
	glEnd();

	glColor3f(1.0, 0.9, 0.8);
	glPointSize(10);
	glBegin(GL_POINTS);
	glVertex2f(-400, -40);
	glVertex2f(-250, -150);
	glVertex2f(400, -40);
	glVertex2f(250, -150);
	glEnd();


	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex2f(-405, -40);
	glVertex2f(-395, -40);
	glVertex2f(-245, -150);
	glVertex2f(-255, -150);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	glVertex2f(405, -40);
	glVertex2f(395, -40);
	glVertex2f(245, -150);
	glVertex2f(255, -150);
	glEnd();

	glColor3f(0.4f, 0.0f, 0.0f);
	glRectf(-240, 135, 240, 125);

	glColor3f(0.4f, 0.0f, 0.0f);
	glRectf(-200, 240, 200, 245);

	glColor3f(1.0f, 0.5f, 0.0f);
	glRectf(-250, 150, 250, 135);

	glColor3f(0.4f, 0.0f, 0.0f);
	glRectf(-5, 150, 5, 385);

	glColor3ub(0, 0, 0);
	vprint(-365, -5, GLUT_BITMAP_TIMES_ROMAN_24, "City View");
	vprint(265, -5, GLUT_BITMAP_HELVETICA_12, "DINCER VELIOGLU");

}

void snow()
{
	glColor3f(1, 1, 1);

	for (int i = -140; i < -35; i += 5)
	{
		randX = rand() % ((-79) - 100) + 100;
		randY = rand() % ((-189) - 50) + 50;
		circle2(i + randX + -10, xS + 200 + randY, 2);
	}

	for (int i = -140; i < -35; i += 5)
	{
		randX = rand() % ((-79) - 100) + 100;
		randY = rand() % ((-189) - 50) + 50;
		circle2(i + randX - 180, xS + 200 + randY, 2);
	}

}

//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	drawWall();

	drawTable();

	drawUpperWindow();

	drawWindow();

	drawBackGround();

	drawView();

	drawSky();

	drawRoof();

	drawhighWay();

	drawCar();

	snow();

	drawMiddleWindow();

	fakeWindow();

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	if (key == ' ')
		activeTimer = !activeTimer;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.
	currentPosOfX = x - winWidth / 2;
	currentPosOfY = winHeight / 2 - y;


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	if (activeTimer)
	{
		if (xB < 395)
			xB++;

		else
		{
			xB = -115;
		}

	}
	if (xS > -400)
	{
		xS--;
	}
	else {
		xS = 0;
	}





	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Homework 1- City view in cold weather(by Dincer Velioglu)");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}
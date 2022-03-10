/***
----------
STUDENT : Dincer Velioglu
----------
***/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  650
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int currentPosOfX;
int currentPosOfY;

#define START 0
#define RUN 1

int appState = START;

int score = 0;
float AlienX = 0;
float AlienSpeed = 1;

bool moving = false;

bool activeTimer = false;

int randX, randY;

typedef struct {
	float posX, posY;
	bool active = false;
}myTargets_t;

typedef struct {
	float posGunx = 0, posBulletx = 0, posBullety = 0;
	bool fire = false;

}myGunn;

myTargets_t alience[5];

myGunn myShip;

bool isDistanceUnderTheRadious(int bulletX, int bulletY, int targetX, int targetY, int r) {
	float distance = sqrt((targetX - bulletX)*(targetX - bulletX) + (targetY - bulletY) * (targetY - bulletY));

	if (distance < r)
	{

		return true;

	}
	else
		return false;
}

float timer = 20;

bool movement = true;


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


void drawAliens(int x, int y) // targets 
{
	glColor3f(0.3, 0.5, 0.5);
	glBegin(GL_POLYGON);
	glVertex2f(0 + x, 20 + y);
	glVertex2f(-40 + x, -30 + y);
	glVertex2f(40 + x, -30 + y);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(-40 + x, -35 + y);
	glVertex2f(-30 + x, -50 + y);
	glVertex2f(-20 + x, -35 + y);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(-10 + x, -35 + y);
	glVertex2f(0 + x, -55 + y);
	glVertex2f(10 + x, -35 + y);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(40 + x, -35 + y);
	glVertex2f(30 + x, -50 + y);
	glVertex2f(20 + x, -35 + y);
	glEnd();
	//inside
	glColor3f(0.5, 1, 0.5);
	glBegin(GL_POLYGON);
	glVertex2f(-16 + x, 0 + y);
	glVertex2f(-16 + x, -15 + y);
	glVertex2f(-5 + x, -15 + y);
	glVertex2f(-16 + x, 0 + y);
	glEnd();
	glColor3f(0.5, 1, 0.5);
	glBegin(GL_POLYGON);
	glVertex2f(16 + x, y + 0);
	glVertex2f(16 + x, y + -15);
	glVertex2f(5 + x, y + -15);
	glVertex2f(16 + x, y + 0);
	glEnd();
}

void drawSpaceShip(int xB)
{
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex2f(xB - 50, -300);
	glVertex2f(xB + 50, -300);
	glVertex2f(xB + 55, -275);
	glVertex2f(xB + 0, -175);
	glVertex2f(xB - 55, -275);
	glEnd();

	//L-R REDD
	glColor3f(7, 0, 0);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2f(xB + 55, -275);
	glVertex2f(xB + 50, -300);
	//
	glVertex2f(xB - 55, -275);
	glVertex2f(xB - 50, -300);
	glEnd();

	//lines
	glColor3f(0, 0, 0);
	glLineWidth(1);
	glBegin(GL_LINES);
	for (int i = 10; i < 55; i = i + 5)
	{
		glVertex2f(xB + 0, -175);
		glVertex2f(xB + i, -275);
	}
	for (int i = -10; i > -55; i = i - 5)
	{
		glVertex2f(xB + 0, -175);
		glVertex2f(xB + i, -275);
	}
	for (int i = 50; i > 0; i = i - 5)
	{
		for (int j = 45; j > 0; j = j - 5)
		{
			glVertex2f(xB + i, -275);
			glVertex2f(xB + j, -300);
		}
	}
	for (int i = -50; i < 0; i = i + 5)
	{
		for (int j = -45; j < 0; j = j + 5)
		{
			glVertex2f(xB + i, -275);
			glVertex2f(xB + j, -300);
		}
	}
	glEnd();

	glColor3f(1, 1, 1);
	glRectf(xB - 25, -315, xB - 10, -300);
	glRectf(xB + 10, -315, xB + 25, -300);

	glColor3f(0.8, 0, 0);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2f(xB - 25, -300);
	glVertex2f(xB - 25, -315);
	//
	glVertex2f(xB + 25, -300);
	glVertex2f(xB + 25, -315);
	glEnd();

	//middle line
	glColor3f(0.5, 0, 0);
	glLineWidth(7);
	glBegin(GL_LINES);
	glVertex2f(xB + 0, -250);
	glVertex2f(xB + 0, -305);
	glEnd();
	glColor3f(0, 0, 0);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2f(xB + 0, -175);
	glVertex2f(xB + 0, -250);
	glEnd();

	//fire
	/*glColor3f(1, 0, 0);
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(0, -155);
	glVertex2f(0, -175);
	glEnd();
	*/

	//L
	glColor3f(1, 0.3, 0);
	glBegin(GL_POLYGON);
	glVertex2f(xB - 25, -315);
	glVertex2f(xB - 17, -340);
	glVertex2f(xB - 10, -315);
	glEnd();
	glColor3f(1, 0.9, 0);
	glBegin(GL_POLYGON);
	glVertex2f(xB - 25, -315);
	glVertex2f(xB - 17, -325);
	glVertex2f(xB - 10, -315);
	glEnd();

	//R
	glColor3f(1, 0.3, 0);
	glBegin(GL_POLYGON);
	glVertex2f(xB + 25, -315);
	glVertex2f(xB + 17, -340);
	glVertex2f(xB + 10, -315);
	glEnd();
	glColor3f(1, 0.9, 0);
	glBegin(GL_POLYGON);
	glVertex2f(xB + 25, -315);
	glVertex2f(xB + 17, -325);
	glVertex2f(xB + 10, -315);
	glEnd();
} // gun 

void bullet(int x, int y)
{
	glColor3f(1, 0, 0);
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(x + 0, y - 155);
	glVertex2f(x + 0, y - 175);
	glEnd();
}

void spawn(int i) // for initialize all array elements of the targets when they inactive (fired or pass the upper border)
{
	alience[i].active = true;
	alience[i].posX = -325 - 120 * i;
	alience[i].posY = rand() % ((360) - 120) + 120;

}

void display_menu() {
	glColor3f(1, 0, 0);
	vprint(-75, 50, GLUT_BITMAP_TIMES_ROMAN_24, "START GAME");

	glColor3f(0.8, 0, 0);
	vprint(-125, -360, GLUT_BITMAP_TIMES_ROMAN_24, "SpaceBar to Start Game");

	drawSpaceShip(myShip.posGunx);
	drawAliens(-275, 350);
	drawAliens(-275, -300);
	drawAliens(275, 350);
	drawAliens(275, -300);
}

//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//

	switch (appState)
	{
	case START:
		display_menu();
		break;
	case RUN:
		if (timer >= 0)
		{

			glClearColor(0, 0, 0.2, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			glColor3f(1, 1, 0);
			vprint(-157.5, -355, GLUT_BITMAP_9_BY_15, "<SpaceBar> Fire, <F1> Pause/Restart");

			drawSpaceShip(myShip.posGunx);
			bullet(myShip.posBulletx, myShip.posBullety);


			for (int i = 0; i < 5; i++)
			{
				if (alience[i].active == true)
					drawAliens(alience[i].posX, alience[i].posY);

			}


			for (int i = 0; i < 5; i++)
			{
				if (isDistanceUnderTheRadious(myShip.posBulletx, myShip.posBullety - 155, alience[i].posX, alience[i].posY, 45))
				{
					alience[i].active = false;
					spawn(i);
					score++;
					timer += 1.5;
				}
			}

			glColor3f(1, 0, 0);
			vprint(225, 380, GLUT_BITMAP_8_BY_13, "TIME: (%.1f)", timer);
			vprint(-25, 380, GLUT_BITMAP_8_BY_13, "SCORE : %d", score);

		}
		else
		{
			glColor3f(1, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			vprint(-60, 150, GLUT_BITMAP_TIMES_ROMAN_24, "TIME IS UP");
			vprint(-50, -75, GLUT_BITMAP_TIMES_ROMAN_24, "SCORE: %d", score);

			vprint(-35, 75, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS");
			vprint(-25, 50, GLUT_BITMAP_TIMES_ROMAN_24, "-F1-");

			drawSpaceShip(myShip.posGunx);
			drawAliens(0, 0);
			drawAliens(50, 50);
			drawAliens(100, 100);
			drawAliens(150, 150);
			drawAliens(-50, 50);
			drawAliens(-100, 100);
			drawAliens(-150, 150);
		}
		break;
	default:
		break;
	}
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

	if (key == ' ' && appState == START)
	{
		appState = RUN;

		timer = 20;
	}
	else if (key == ' ')
	{
		myShip.fire = true;
	}


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

	switch (key)
	{
	case GLUT_KEY_RIGHT:
		//right = true;
		if (myShip.posBulletx < 270)
		{
			if (!myShip.fire)
				myShip.posBulletx += 7;
			myShip.posGunx += 7;
		}
		break;
	case GLUT_KEY_LEFT:
		//left = true;
		if (myShip.posBulletx > -270)
		{
			if (!myShip.fire)
				myShip.posBulletx -= 7;
			myShip.posGunx -= 7;
		}
		break;
	}

	if (key == GLUT_KEY_F1)
	{
		if (timer <= 0)
		{
			movement = true;
			timer = 20;
			score = 0;
		}
		else if (movement)
			movement = false;
		else
			movement = true;
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

	if (movement && timer >= 0)
	{
		timer -= 0.01;


		for (int i = 0; i < 5; i++)
		{
			if (alience[i].posX <= 420)
				alience[i].posX += 1;
			else
			{
				spawn(i);

			}
		}

		if (myShip.fire)
		{
			if (myShip.posBullety < 580)
				myShip.posBullety += 3;
			else
			{
				myShip.posBullety = 0;
				myShip.posBulletx = myShip.posGunx;
				myShip.fire = false;
			}
		}
	}
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < 5; i++)
		spawn(i);


}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("HW-2: -Shooting Aliens- by Dincer Velioglu");

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
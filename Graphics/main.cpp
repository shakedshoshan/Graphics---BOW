#define _CRT_SECURE_NO_WARNINGS

#include "GLUT.h"
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <stdio.h>

const int GSIZE = 200;
const double PI = 3.14;

const int H = 600;
const int W = 600;

double ground[GSIZE][GSIZE] = { 0 };

double eyex = 0, eyey = 20, eyez = 20;
double dx = 0, dz = 0, dy = 0;
double alpha = PI / 2;
double offset = 0;

// ego-motion
double speed = 0;
double sight_angle = PI, angular_speed = 0;
double dirx = sin(sight_angle), dirz = cos(sight_angle), diry = 0;

bool slider_is_moving = false;
bool slider_is_moving2 = false;
double oldY, oldY2, pitch = 0, pitch2 = 0;

BOOL open = false;

// light0
float l1amb[4] = { 0.4,0.4,0.4,1 }; // ambient
float l1diff[4] = { 0.8,0.8,0.8,1 }; // diffuse
float l1spec[4] = { 0.7,0.7,0.7,1 }; // specular
float l1pos[4] = { -2,1,2,0 }; // 0 for directional light

//ligth1
float l2amb[4] = { 0.4,0.4,0.4 };
float l2diff[4] = { 0.7,0.7,0.3 };
float l2spec[4] = { 0.6,0.6,0.6 };
float l2pos[4] = { -5,-1,3,0 };



void init()
{
	glClearColor(0.8, 0.8, 1, 0); // set background color
	glEnable(GL_DEPTH_TEST); // test of near/far surface


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);


	// light definitions
	glLightfv(GL_LIGHT0, GL_AMBIENT, l1amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l1diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l1spec);
	glLightfv(GL_LIGHT0, GL_POSITION, l1pos);

	// light definitions
	glLightfv(GL_LIGHT1, GL_AMBIENT, l2amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l2diff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, l2spec);
	glLightfv(GL_LIGHT1, GL_POSITION, l2pos);

}

void DrawWall(double r, double g, double b)
{
	
	float amb[4] = { 0.3 * r,0.3 * g,0.3 * b,0 };
	float diff[4] = { 0.7 * r,0.7 * g,0.7 * b,0 };
	float spec[4] = { 0.5 * r,0.5 * g,0.5 * b,0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);

	glNormal3d(0, 1, 0);
	glColor3d(r, g, b);
	glBegin(GL_POLYGON);
	glVertex3d(0,0,0);
	glVertex3d(1,0,0);
	glVertex3d(1,1,0);
	glVertex3d(0,1,0);
	glEnd();
}

void DrawCylinder(int num_pts, double r, double g, double b)
{
	double alpha, teta = 2 * PI / num_pts;

	float amb[4] = { 0.3 * r,0.3 * g,0.3 * b,0 };
	float diff[4] = { 0.7 * r,0.7 * g,0.7 * b,0 };
	float spec[4] = { 0.5 * r,0.5 * g,0.5 * b,0 };


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);


	for (alpha = 0; alpha < PI; alpha += teta)
	{
		glBegin(GL_POLYGON);
		glNormal3d(sin(alpha), 0, cos(alpha));
		glColor3d(r, g, b);
		glVertex3d(sin(alpha), 1, cos(alpha) ); // 1
		glVertex3d(sin(alpha + teta), 1, cos(alpha + teta)); // 2
		glVertex3d(sin(alpha + teta), 0, cos(alpha + teta)); // 3
		glVertex3d(sin(alpha), 0, cos(alpha)); // 4
		glEnd();
	}
}

void DrawCylinder1(int num_pts, double topr, double bottomr, double r, double g, double b)
{
	double alpha, teta = 2 * PI / num_pts;

	float amb[4] = { 0.3 * r,0.3 * g,0.3 * b,0 };
	float diff[4] = { 0.7 * r,0.7 * g,0.7 * b,0 };
	float spec[4] = { 0.5 * r,0.5 * g,0.5 * b,0 };


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);

	for (alpha = 0; alpha < 2 * PI; alpha += teta)
	{
		glBegin(GL_POLYGON);
		glNormal3d(sin(alpha), 0, cos(alpha));
		glColor3d(r, g, b);
		glVertex3d(topr * sin(alpha), 1, topr * cos(alpha)); // 1
		glVertex3d(topr * sin(alpha + teta), 1, topr * cos(alpha + teta)); // 2
		glColor3d(r + 0.1, g + 0.1, b + 0.1);
		glVertex3d(bottomr * sin(alpha + teta), 0, bottomr * cos(alpha + teta)); // 3
		glVertex3d(bottomr * sin(alpha), 0, bottomr * cos(alpha)); // 4
		glEnd();
	}
}

void DrawCylinder2(int num_pts, double r, double g, double b)
{
	double alpha, teta = 2 * PI / num_pts;

	float amb[4] = { 0.3 * r,0.3 * g,0.3 * b,0 };
	float diff[4] = { 0.7 * r,0.7 * g,0.7 * b,0 };
	float spec[4] = { 0.5 * r,0.5 * g,0.5 * b,0 };


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);

	for (alpha = 0; alpha < PI * 2; alpha += teta)
	{
		glBegin(GL_POLYGON);
		glNormal3d(sin(alpha), 0, cos(alpha));
		glColor3d(r, g, b);
		glVertex3d(sin(alpha), 1, cos(alpha) ); // 1
		glVertex3d(sin(alpha + teta), 1, cos(alpha + teta)); // 2
		glVertex3d(sin(alpha + teta), 0, cos(alpha + teta)); // 3
		glVertex3d(sin(alpha), 0, cos(alpha)); // 4
		glEnd();
	}
}

void DrawHalfCircle(int num_pts, double r, double g, double b)
{
	double alpha, teta = 2 * PI / num_pts;

	float amb[4] = { 0.3 * r,0.3 * g,0.3 * b,0 };
	float diff[4] = { 0.7 * r,0.7 * g,0.7 * b,0 };
	float spec[4] = { 0.5 * r,0.5 * g,0.5 * b,0 };


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);

	glBegin(GL_POLYGON);
	glNormal3d(0,1,0);
	glColor3d(r, g, b);
	for (alpha = 0; alpha < PI; alpha += teta)
	{
		glVertex3d(sin(alpha), cos(alpha), 0.0); // 1
	}
	glEnd();
}

void DrawCircle(int num_pts, double r, double g, double b)
{
	double alpha, teta = 2 * PI / num_pts;

	float amb[4] = { 0.3 * r,0.3 * g,0.3 * b,0 };
	float diff[4] = { 0.7 * r,0.7 * g,0.7 * b,0 };
	float spec[4] = { 0.5 * r,0.5 * g,0.5 * b,0 };


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);

	glBegin(GL_POLYGON);
	glNormal3d(0, 1, 0);
	glColor3d(r, g, b);
	for (alpha = 0; alpha < PI * 2; alpha += teta)
	{
		glVertex3d(sin(alpha), cos(alpha), 0.0); // 1
	}
	glEnd();
}

void DrawDonut(int num_pts, int num_slices, double r, double g, double b)
{
	double beta, delta = PI / num_slices;



	for (beta = 0; beta <= 2 * PI; beta += delta)
	{
		glPushMatrix();
		glTranslated(0, sin(beta), 0);
		glScaled(1, sin(beta + delta) - sin(beta), 1);
		DrawCylinder1(num_pts, 5 + cos(beta + delta), 5 + cos(beta), r, g, b);
		glPopMatrix();
	}
}

void DrawFloor() {
	// 1 wall
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslated(-15, 10, -15);
	glScaled(30, 30, 30);
	glRotated(90, 1, 0, 0);
	DrawWall(0.2,0.4,0.2);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void DrawKey() {

	//stick
	glPushMatrix();
	glTranslated(0, 13, 3.5);
	glRotated(90, 0, 0, 1);
	glScaled(0.05, 1, 0.05);
	DrawCylinder2(30, 0.65, 0, 0);
	glPopMatrix();

	//donut
	glPushMatrix();
	glTranslated(-1, 13, 3.5);
	glRotated(pitch*270/PI, 1, 0, 0);
	glScaled(0.05,0.05,0.05);
	DrawDonut(30, 30, 0.65, 0, 0);
	glPopMatrix();

}

void DrawCover(double offset) {
	// cover
	glPushMatrix();
	glTranslated(2.5 + pitch2/5 , 15 + pitch2/1.5, 7 );
	//glTranslated(2.5 , 15, 7 );
	glRotated(90, 0, 1, 0);
	glRotated(90, 0, 0, 1 );
	glRotated(180, 1, 0, 0);

	glRotated(offset/3, 0, 1, 0);
	glScaled(1, 7, 2.5);
	DrawCylinder(30, 0.5, 0.4, 0.4);
	glPopMatrix();

	//cover - sides
	//1
	glPushMatrix();
	glTranslated(2.5 + pitch2 / 5, 15 + pitch2 / 1.5, 7);
	glRotated(90, 0, 0, 1);
	glRotated(offset / 3, 0, 0, -1);
	glScaled(1, 2.5, 1);
	DrawHalfCircle(60, 0.5, 0.4, 0.2);
	glPopMatrix();
	//2
	glPushMatrix();
	glTranslated(2.5 + pitch2 / 5, 15 + pitch2 / 1.5, 0);
	glRotated(90, 0, 0, 1);
	glRotated(offset / 3, 0, 0, -1);
	glScaled(1, 2.5, 1);
	DrawHalfCircle(60, 0.5, 0.4, 0.2);
	glPopMatrix();
}

void drawBox() {

	glDisable(GL_LIGHTING);
	//treasure
	glPushMatrix();
	glTranslated(0, 13.001, 7);
	glRotated(270, 1, 0, 0);
	glScaled(4.99, 6.99, 4.99);
	DrawWall(1,1, 0);
	glPopMatrix();
	glEnable(GL_LIGHTING);

	// 1 wall
	glPushMatrix();
	glTranslated(0, 10, 0);
	glScaled(5,5,5);
	DrawWall(0.7, 0.5, 0.5);
	glPopMatrix();

	// 3 wall
	glPushMatrix();
	glTranslated(0, 10, 7);
	glScaled(5, 5, 5);
	DrawWall(0.7, 0.5, 0.5);
	glPopMatrix();

	// 2 wall
	glPushMatrix();
	glTranslated(0, 10, 7);
	glRotated(90, 0, 1, 0);
	glScaled(7, 5, 5);
	DrawWall(0.7, 0.5, 0.5);
	glPopMatrix();

	// 4 wall
	glPushMatrix();
	glTranslated(5, 10, 7);
	glRotated(90, 0, 1, 0);
	glScaled(7, 5, 5);
	DrawWall(0.7, 0.5, 0.5);
	glPopMatrix();

	// buttom
	glPushMatrix();
	glTranslated(0, 10.001, 7);
	glRotated(270, 1, 0, 0);
	glScaled(5, 7, 5);
	DrawWall(0.7, 0.5, 0.5);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0, -0.05, 0);
	DrawCover(pitch2 * 180 / PI);
	glPopMatrix();
	/*
	if (!open) {
		glPushMatrix();
		glTranslated(0, -0.05, 0);
		DrawCover(0);
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glTranslated(0, -0.05, 0);
		DrawCover(pitch2*180/PI);
		glPopMatrix();
	}
	*/


	// hole for the key
	glPushMatrix();
	glTranslated(-0.01, 12.5, 4);
	glRotated(90, 0, 1, 0);
	glScaled(1,1,1);
	DrawWall(0.65,0.65,0.65);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.011, 13, 3.5);
	glRotated(90, 0, 1, 0);
	glScaled(0.15, 0.15, 0.15);
	DrawCircle(60,0.9,0.9,0.9);
	glPopMatrix();

}

void DrawSlider()
{
	glColor3d(1, 1, 0);
	// background
	glBegin(GL_POLYGON);
	glVertex2d(0, 0);
	glVertex2d(0, 100);
	glVertex2d(100, 100);
	glVertex2d(100, 0);
	glEnd();

	glLineWidth(3);
	glColor3d(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(50, 0);
	glVertex2d(50, 100);
	glEnd();
	glLineWidth(1);

	glMatrixMode(GL_MODELVIEW); // matrix of world
	glLoadIdentity(); // start transformations here

	glPushMatrix();
	glTranslated(0, pitch * 180 / PI, 0); // move in dgrees
	// slider button
	glColor3d(0.5, 0.5, 0.5);
	glBegin(GL_POLYGON);
	glVertex2d(40, 0);
	glVertex2d(40, 20);
	glVertex2d(60, 20);
	glVertex2d(60, 0);
	glEnd();

	glColor3d(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(40, 10);
	glVertex2d(60, 10);
	glEnd();

	glPopMatrix();
}

void DrawSlider2() {
	glColor3d(1, 1, 0);
	// background
	glBegin(GL_POLYGON);
	glVertex2d(0, 0);
	glVertex2d(0, 100);
	glVertex2d(100, 100);
	glVertex2d(100, 0);
	glEnd();

	glLineWidth(3);
	glColor3d(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(50, 0);
	glVertex2d(50, 100);
	glEnd();
	glLineWidth(1);

	glMatrixMode(GL_MODELVIEW); // matrix of world
	glLoadIdentity(); // start transformations here

	glPushMatrix();
	glTranslated(0, pitch2 * 180 / PI, 0); // move in dgrees
	// slider button
	glColor3d(0.5, 0.5, 0.5);
	glBegin(GL_POLYGON);
	glVertex2d(40, 0);
	glVertex2d(40, 20);
	glVertex2d(60, 20);
	glVertex2d(60, 0);
	glEnd();

	glColor3d(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(40, 10);
	glVertex2d(60, 10);
	glEnd();

	glPopMatrix();


	// control led
	if (!open) {
		glColor3d(1, 0, 0);
	}
	else {
		glColor3d(0, 1, 0);
	}
	glBegin(GL_POLYGON);
	glVertex2d(10, 70);
	glVertex2d(10, 80);
	glVertex2d(20, 80);
	glVertex2d(20, 70);
	glEnd();
	
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, W, H); // left+bottom and width+hight
	glMatrixMode(GL_PROJECTION); // glFrustum sets for 3D
	glLoadIdentity();
	// camera model
	glFrustum(-1, 1, -1, 1, 0.7, 300);
	// eyex, eyey, eyez, centerx, centery, centerz, topx, topy, topz
	gluLookAt(eyex, eyey, eyez,
		eyex + dirx, eyey - 0.5, eyez + dirz, 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	DrawFloor();
	drawBox();
	DrawKey();
	
	if (pitch >= 1.1) {
		open = true;
	}
	else {
		open = false;
	}

	// controls sub-window

	glDisable(GL_LIGHTING);
	//slider 1 
	glViewport(0, 0, 100, 100);
	// Now we switch to Orthogonal projection, so we must change the projections matrix
	glMatrixMode(GL_PROJECTION); // vision matrix
	glLoadIdentity(); // start transformations here
	glOrtho(0, 100, 0, 100, -1, 1);
	glDisable(GL_DEPTH_TEST); // return to principles of graphics 2D

	DrawSlider();

	glEnable(GL_DEPTH_TEST);

	
	//slider 2
	glViewport(500, 0, 100, 100);
	// Now we switch to Orthogonal projection, so we must change the projections matrix
	glMatrixMode(GL_PROJECTION); // vision matrix
	glLoadIdentity(); // start transformations here
	glOrtho(0, 100, 0, 100, -1, 1);
	glDisable(GL_DEPTH_TEST); // return to principles of graphics 2D

	DrawSlider2();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glutSwapBuffers();
}


void idle()
{
	sight_angle += angular_speed;
	dirx = sin(sight_angle);
	dirz = cos(sight_angle);

	eyex += (speed * dirx);
	eyez += (speed * dirz);
	eyey += dy;

	
	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		angular_speed += 0.0001;
		break;
	case GLUT_KEY_RIGHT:
		angular_speed -= 0.0001;
		break;
	case GLUT_KEY_UP:
		speed += 0.001;
		break;
	case GLUT_KEY_DOWN:
		speed -= 0.001;
		break;
	case GLUT_KEY_PAGE_UP:
		dy += 0.01;
		break;
	case GLUT_KEY_PAGE_DOWN:
		dy -= 0.01;
		break;
	}


}

void mouse(int button, int state, int x, int y)
{
	double angle_in_degrees = pitch * 180 / PI +10 ;
	bool clickInSlider1 = x > 40 && x<60 && H - y >angle_in_degrees - 10 && H - y < angle_in_degrees + 10;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickInSlider1) // drag slider
	{
		slider_is_moving = true;
		oldY = H - y;
	}

	double angle_in_degrees2 = pitch2 * 180 / PI + 10;
	bool clickInSlider2 = x > 540 && x<560 && H - y >angle_in_degrees2 - 10 && H - y < angle_in_degrees2 + 10;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickInSlider2 && open) // drag slider
	{
		slider_is_moving2 = true;
		oldY2 = H - y;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		slider_is_moving = false;
		//slider_is_moving2 = false;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		slider_is_moving2 = false;
}


void mouseDrag(int x, int y)
{
	double dy = (H - y) - oldY;

	if (slider_is_moving && H - y < 100 && H - y>0)
	{
		pitch += dy * PI / 180; // transform dy to radian
		oldY = (H - y);

	}

	double dy2 = (H - y) - oldY2;

	if (slider_is_moving2 && H - y < 100 && H - y>0)
	{
		pitch2 += dy2 * PI / 180; // transform dy to radian
		oldY2 = (H - y);

	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Exam");

	glutDisplayFunc(display); // display is the refresh function
	glutIdleFunc(idle);
	glutSpecialFunc(special); // for "home", "insert" arrows keys ...
	glutMouseFunc(mouse);
	glutMotionFunc(mouseDrag);

	init();

	glutMainLoop();
}

// Glut_Alap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <set>
#include <vector>
#include <ctime>

#include <GL/glut.h>

#include "Common.h"

using namespace std;


// Define a constant for the value of PI
#define GL_PI 3.141592654f

static int MenuID, IdleMenu;
static int IdlePrint = 0;

static bool RENDER_PAUSE;
static bool RENDER_STEP;


void DrawGraphObj(GraphObject *c)
{
	// invalid pointer eset�n ne sz�lljunk el
	if (!c) return;

	glBegin(GL_LINE_LOOP);

	// sz�n be�ll�t�sa a k�r alapj�n
	glColor3f( c->clr->r, c->clr->g, c->clr->b);

	// vonal sor rajzol�sa
	for(unsigned int i = 0;  i < c->points->size(); ++i)
	{
		glVertex2f( (*(c->points))[i]->x, (*(c->points))[i]->y);
	}

	glEnd();

	/*glBegin(GL_LINE_LOOP);

	// sz�n be�ll�t�sa a k�r alapj�n
	glColor3f( c->clr->g, c->clr->b, c->clr->r);

	// vonal sor rajzol�sa
	glVertex2f(c->XMin(), c->YMin());
	glVertex2f(c->XMin(), c->YMax());
	glVertex2f(c->XMax(), c->YMax());
	glVertex2f(c->XMax(), c->YMin());

	glEnd();*/

	c->crushed = false;
}


// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// >> Modellezo programresz

	for(set<GraphObject*>::iterator it = Common::graphObjSet->begin(); it != Common::graphObjSet->end(); ++it)
	{
		(*it)->clr = Common::CLR_NORMAL;
		Common::DistanceCheck(*it);
		Common::UpDateGraphObjectPosition(*it);
		DrawGraphObj(*it);
	}
	// << Modellezo programresz

	// Flush drawing commands
	glutSwapBuffers();
}


// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glEnable(GL_DEPTH_TEST);
}

void SpecialKeys(int key, int x, int y)
{
	// ...
	int state;


	if(key == GLUT_KEY_UP)
	{
		RENDER_PAUSE = !RENDER_PAUSE;
	}
	if(key == GLUT_KEY_DOWN)
	{
		Common::renderForward = !Common::renderForward;
	}
	if(key == GLUT_KEY_LEFT)
	{
		Common::renderForward = true;
	}
	if(key == GLUT_KEY_RIGHT)
	{
		Common::renderForward = false;
	}


	printf("Funkciobillentyu lenyomva, kodja %d, pozicio (%d,%d). ", key, x, y);
	state = glutGetModifiers();
	if(state & GLUT_ACTIVE_SHIFT)
		printf("SHIFT lenyomva. ");
	if(state & GLUT_ACTIVE_CTRL)
		printf("CTRL lenyomva. ");
	if(state & GLUT_ACTIVE_ALT)
		printf("ALT lenyomva. ");
	printf("\n");
	fflush(stdout);

	// Refresh the Window
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	// ...
	int state;

	printf("Billentyu lenyomva, kodja %c, pozicio (%d,%d). ", key, x, y);
	state = glutGetModifiers();
	if(state & GLUT_ACTIVE_SHIFT)
		printf("SHIFT lenyomva. ");
	if(state & GLUT_ACTIVE_CTRL)
		printf("CTRL lenyomva. ");
	if(state & GLUT_ACTIVE_ALT)
		printf("ALT lenyomva. ");
	printf("\n");
	fflush(stdout);

	glutPostRedisplay();
}

void Timer(int value)
{
	//printf("Timer esemeny (%d)\n", value);

	if (!RENDER_PAUSE || RENDER_STEP)
	{
		glutPostRedisplay();
		RENDER_STEP = false;
	}

	//glutTimerFunc(1000, Timer, value + 1);
	glutTimerFunc(10, Timer, value + 1);
}

void Idle()
{
	if(IdlePrint)
		printf("Idle esemeny.\n");

	glutPostRedisplay();
}

void ChangeSizeOrtho(int w, int h)
{
	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);
	
	// Reset projection matrix stack
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	if (w <= h)
	{
		Common::screenXMin = -Common::nRange;
		Common::screenXMax = Common::nRange;
		Common::screenYMin = -Common::nRange*h/w;
		Common::screenYMax = Common::nRange*h/w;
	}
	else
	{
		Common::screenXMin = -Common::nRange*w/h;
		Common::screenXMax = Common::nRange*w/h;
		Common::screenYMin = -Common::nRange;
		Common::screenYMax = Common::nRange;
	}
	
	glOrtho (Common::screenXMin, Common::screenXMax, Common::screenYMin, Common::screenYMax,
		-Common::nRange, Common::nRange);

	// Reset Model view matrix stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ChangeSizePerspective(GLsizei w, GLsizei h)
{
	GLfloat fAspect;

	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w/(GLfloat)h;

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Produce the perspective projection
	gluPerspective(60.0f,    // fovy
		fAspect,  // aspect
		10.0,     // zNear
		100.0     // zFar
		);
	gluLookAt(0.0, 0.0, 50.0, // eye
		0.0, 0.0, 0.0,  // center
		0.0, 1.0, 0.0   // up
		);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ProcessMenu(int value)
{
	switch(value)
	{
	case 1:
		printf("1. menupont kivalasztva.\n");
		break;

	case 2:
		printf("2. menupont kivalasztva.\n");
		break;

	case 3:
		printf("Idle kiiratas bekapcsolva.\n");
		IdlePrint = 1;
		break;

	case 4:
		printf("Idle kiiratas kikapcsolva.\n");
		IdlePrint = 0;
		break;

	case 5:
		std::exit(0);

	default:
		break;
	}

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	// >> Inicializalas
	//srand(time(NULL));
	Common::nRange = 10;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Fejlett Grafikai Algoritmusok - S�ni");

	// >> Inicializ�l�s

	RENDER_PAUSE = true;

	float radius = 2.0;
	float placeRadius = radius * 4;
	float angle = 0.0;
	int circleNum = 5;
	float xShift = 0.0;
	float yShift = 0.0;
	float vX = 0.074;
	float vY = 0.063;
	for(int i=0; i < circleNum; ++i)
	{
		vX = 0.074 + 0.074 * ((rand() % 10) - 10) * 0.10;
		vY = 0.063 + 0.063 * ((rand() % 10) - 10) * 0.10;

		float objAngle = (rand() % 100) / 100.0 * GL_PI;
		Common::graphObjSet->insert(new GraphObject(placeRadius * cos(angle) + xShift, placeRadius * sin(angle) + yShift,
			radius, /*rand() % 5*/i + 3, objAngle,

			//new MyColor(i*0.1 + 0.3, i*0.1, 0.8),
			Common::CLR_NORMAL,

			vX, vY));
		angle += 2.0 * GL_PI / circleNum;
	}/* */

	/*int circleNum = 10;
	for(int i=0; i < circleNum; ++i)
	{
	graphObjSet->insert(new CircleItem(i*1.2 - 2.0, i * (-1.3) - 2.0, radius,
	new MyColor(i*0.1 + 0.3, i*0.1, 0.8), 0.074, 0.063));
	}/* */



	// << Inicializalas


	// >> Callback fuggvenyek

	glutReshapeFunc(ChangeSizeOrtho); // Parhuzamos vetites
	//glutReshapeFunc(ChangeSizePerspective); // Perspektiv vetites

	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(1000, Timer, 1); // 1 mp mulva meghivodik a Timer() fuggveny
	//glutIdleFunc(Idle); // Idle(), ha nem tortenik semmi

	// << Callback fuggvenyek

	// >> Menu

	IdleMenu = glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Idle kiiratas bekapcsolasa", 3);
	glutAddMenuEntry("Idle kiiratas kikapcsolasa", 4);

	MenuID = glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("1. menupont", 1);
	glutAddMenuEntry("2. menupont", 2);
	glutAddSubMenu("Idle fuggveny", IdleMenu);
	glutAddMenuEntry("Kilepes", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// << Menu

	SetupRC();
	glutMainLoop();

	return 0;
}


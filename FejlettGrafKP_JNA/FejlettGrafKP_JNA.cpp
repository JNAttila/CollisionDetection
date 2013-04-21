// Glut_Alap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <set>
#include <vector>

#include <GL/glut.h>

using namespace std;

// Define a constant for the value of PI
#define GL_PI 3.141592654f

static int MenuID, IdleMenu;
static int IdlePrint = 0;

static bool RENDER_PAUSE;
static bool RENDER_STEP;

static float screenXMin = -30.0;
static float screenYMin = -20.0;
static float screenXMax = 30.0;
static float screenYMax = 20.0;

// szín reprezentáló osztály
class MyColor
{
public:
	float r, g, b;

public:
	MyColor(float r, float g, float b) :
	  r(r), g(g), b(b) {}
};

static MyColor *CLR_NEAR = new MyColor(0.99, 0.0, 0.0);
static MyColor *CLR_NORMAL = new MyColor(0.0, 0.7, 0.0);

// egy pont X és Y koordinátája
class MyPoint
{
public:
	float x, y;

public:
	MyPoint(float X, float Y) :
	  x(X), y(Y) {}
};


// két szakasz egymáshoz képest milyen irányban állnak
static int RotDirct(MyPoint *p0, MyPoint *p1, MyPoint *p2){
// Kimenet: +1 ha p0->p1->p2 balra fordul,
//			0 ha p0,p1 és p2 kollineárisak,
//			-1 ha p0->p1->p2 jobbra fordul.

	double p1xp2 = (p1->x - p0->x) * (p2->y - p0->y) - (p2->x - p0->x) * (p1->y - p0->y);
	int signum = (p1xp2 < 0) ? (-1) : ((p1xp2 > 0) ? (1) : (0));
	return signum;
}

// P1 és P2 által kijelöl téglalapon belül van-e PT
bool IsPointBetween(MyPoint *p1, MyPoint *p2, MyPoint *pt)
{
	float xMin = min<int>(p1->x, p2->x);
	float xMax = max<int>(p1->x, p2->x);
	float yMin = min<int>(p1->y, p2->y);
	float yMax = max<int>(p1->y, p2->y);

	if ((xMin <= pt->x) && (pt->x <= xMax) && (yMin <= pt->y) && (pt->y <= yMax))
		return true;

	return false;
}

// egy grafikai objektum adatai
class GraphObject
{
public:
	// középpont
	float cX, cY;
	// sugár
	float R;
	// kerületi pontok
	int numPoints;
	// orientáció
	float rotAngle;

	// a pontok listája
	vector<MyPoint*> *points;

	// a határoló téglalap koordinátáinak indexe a listában
	int iXMin, iXMax;
	int iYMin, iYMax;

	// szín komponensek
	MyColor *clr;

	// sebesség komponensek
	float vX, vY;

public:
	GraphObject(float x, float y, float r, int nP, float rotA, MyColor *c, float vx, float vy) :
	  cX(x), cY(y), R(r), numPoints(nP), rotAngle(rotA), clr(c), vX(vx), vY(vy)
	  {
		  points = new vector<MyPoint*>();
		  
		  float xMin = 1000;
		  float xMax = -1000;
		  float yMin = 1000;
		  float yMax = -1000;

		  float newX, newY;

		  float angle = rotA;
		  for(int i = 0;  i < nP; i++)
		  {
			  angle += 2.0 * GL_PI / nP;

			  newX = x + r * cos(angle);
			  newY = y + r * sin(angle);

			  // a szélsõ pontok x és y koordinátáinak megjegyzése
			  if (xMax < newX)
			  {
				  xMax = newX;
				  iXMax = i;
			  }
			  if (yMax < newY)
			  {
				  yMax = newY;
				  iYMax = i;
			  }

			  if (xMin > newX)
			  {
				  xMin = newX;
				  iXMin = i;
			  }
			  if (yMin > newY)
			  {
				  yMin = newY;
				  iYMin = i;
			  }

			  points->push_back(new MyPoint( newX, newY));
		  }

		  numPoints = (numPoints < 3) ? (3) : ((numPoints > 15) ? (15) : (numPoints));	  
	  }

	  float XMin()
	  {
		  return (*points)[iXMin]->x;
	  }
	  float XMax()
	  {
		  return (*points)[iXMax]->x;
	  }
	  float YMin()
	  {
		  return (*points)[iYMin]->y;
	  }
	  float YMax()
	  {
		  return (*points)[iYMax]->y;
	  }

	  bool IsNear(GraphObject *obj)
	  {
		  if (!obj) return false;

		  float xMin = XMin();
		  float xMax = XMax();
		  float yMin = YMin();
		  float yMax = YMax();

		  float objxMin = obj->XMin();
		  float objxMax = obj->XMax();
		  float objyMin = obj->YMin();
		  float objyMax = obj->YMax();

		  if (((xMin <= objxMin && objxMin <= xMax) || (xMin <= objxMax && objxMax <= xMax)) &&
			  ((yMin <= objyMin && objyMin <= yMax) || (yMin <= objyMax && objyMax <= yMax)) )
			  return true;

		  return false;
	  }

	  // a két objektum ériintkezik-e egymással?
	  bool IsContact(GraphObject *obj)
	  {
		  // sajátmagam minden pontjára
		  for(int i = 0; i < points->size(); ++i)
		  {
			  int sgn;

			  // a saját körvonalam mentén, az én következõ pontom ebben az irányban van
			  int signumMy = RotDirct((*points)[i % points->size()], (*points)[(i + 1) % points->size()],
				  (*points)[(i + 2) % points->size()]);

			  // a vizsgált pont a másik oldalon van, érdemes folytatni

			  // az összehasonlított objektum minden pontjával
			  for(int j = 0; j < obj->points->size(); ++j)
			  {
				  sgn = RotDirct((*points)[i % points->size()], (*points)[(i + 1) % points->size()],
					  (*(obj->points))[j % obj->points->size()]);

				  // azonos oldalon vannak és az elsõ kettõ által téglalapon belül a harmadik
				  if (signumMy == sgn && IsPointBetween((*points)[i % points->size()], (*points)[(i + 1) % points->size()],
					  (*(obj->points))[j % obj->points->size()]))
				  {
					  // összeért
					  return true;
				  }
			  }

		  }

		  return false;
	  }


	  /*
	  // a két objektum ériintkezik-e egymással?
	  bool IsContact(GraphObject *obj)
	  {
		  // sajátmagam minden pontjára
		  for(int i = 0; i < points->size(); ++i)
		  {
			  bool metszes = true;
			  int sgn;

			  // a saját körvonalam mentén, az én következõ pontom ebben az irányban van
			  int signumMy = RotDirct((*points)[i], (*points)[(i + 1) % points->size()],
				  (*points)[(i + 2) % points->size()]);

			  int signumComp = RotDirct((*points)[i], (*points)[(i + 1) % points->size()],
				  (*(obj->points))[0]);

			  if (signumMy != signumComp)
			  {
				  // a vizsgált pont a másik oldalon van, érdemes folytatni

				  // az összehasonlított objektum minden pontjával
				  for(int j = 1; j < obj->points->size() + 1; ++j)
				  {
					  sgn = RotDirct((*points)[i % points->size()], (*points)[(i + 1) % points->size()],
						  (*(obj->points))[j % obj->points->size()]);

					  // ha bármely két pont 
					  if (signumComp != sgn)
					  {
						  // összeérhet
						  continue;
					  }

					  // nem ér össze
					  return false;
				  }
			  }
		  }

		  return true;
	  }
	  /* */


	  ~GraphObject()
	  {
		  if (clr)
			  delete clr;
		  clr = NULL;
	  }

	  bool operator==(const GraphObject *c)
	  {
		  return (c->cX == cX) && (c->cY == cY) && (c->R == R) && (c->rotAngle == rotAngle);
	  }

	  bool operator!=(const GraphObject *c)
	  {
		  return !(this->operator==(c));
	  }
};

// körök gyûjteménye
static set<GraphObject*> *graphObjSet;
static set<GraphObject*>::const_iterator it;

void DrawGraphObj(const GraphObject *c)
{
	// invalid pointer esetén ne szálljunk el
	if (!c) return;

	glBegin(GL_LINE_LOOP);

	// szín beállítása a kör alapján
	glColor3f( c->clr->r, c->clr->g, c->clr->b);

	// vonal sor rajzolása
	for(int i = 0;  i < c->points->size(); ++i)
	{
		glVertex2f( (*(c->points))[i]->x, (*(c->points))[i]->y);
	}

	glEnd();
}

bool DistanceCheck(GraphObject *c)
{
	set<GraphObject*>::const_iterator i;
	for (i = graphObjSet->begin(); i != graphObjSet->end(); ++i)
	{
		if (((*c) != (*i)) && c->IsNear(*i))
		{
			c->clr = CLR_NEAR;

			if (c->IsContact(*i))
			{
				c->vX *= -1;
				c->vY *= -1;

				(*i)->vX *= -1;
				(*i)->vY *= -1;
			}

			return true;
		}
	}

	return false;
}

void UpDateGraphObjectPosition(GraphObject *c)
{
	if (!c) return;

	c->cX += c->vX;
	c->cY += c->vY;

	///////////////////////////////////////////////////////////////////////////
	// falakról visszaverõdés

	// faltól mért távolság
	float dist;
	// az elmozdulás mértéke
	float shiftX = c->vX;
	float shiftY = c->vY;

	dist = c->vX + c->XMax();
	if (dist > screenXMax)
	{
		shiftX -= 2 * (c->XMax() + c->vX - screenXMax);
		c->vX *= -1;
	}

	dist = c->vX + c->XMin();
	if (dist < screenXMin)
	{
		shiftX -= 2 * (c->XMin() + c->vX - screenXMin);
		c->vX *= -1;
	}

	dist = c->vY + c->YMax();
	if (dist > screenYMax)
	{
		shiftY -= 2 * (c->YMax() + c->vY - screenYMax);
		c->vY *= -1;
	}

	dist = c->vY + c->YMin();
	if (dist < screenYMin)
	{
		shiftY -= 2 * (c->YMin() + c->vY - screenYMin);
		c->vY *= -1;
	}

	///////////////////////////////////////////////////////////////////////////
	// az egymással érintkezés
	set<GraphObject*>::iterator iter;
	for(iter = graphObjSet->begin(); iter != graphObjSet->end(); ++iter)
	{
		if ((*iter)->clr == CLR_NEAR)
		{
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// az elmozdulás alkalmazása
	for(int i = 0; i < c->points->size(); ++i)
	{
		(*(c->points))[i]->x += shiftX;
		(*(c->points))[i]->y += shiftY;
	}
}

// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// >> Modellezo programresz

	// a játszótér kerete
	glBegin(GL_LINE_LOOP);
	glColor3f( 0.5, 1.0, 0.0);
	glVertex2f(screenXMin, screenYMin);
	glVertex2f(screenXMax, screenYMin);
	glVertex2f(screenXMax, screenYMax);
	glVertex2f(screenXMin, screenYMax);
	glEnd();


	for(it = graphObjSet->begin(); it != graphObjSet->end(); ++it)
	{
		(*it)->clr = CLR_NORMAL;
		DistanceCheck(*it);
		UpDateGraphObjectPosition(*it);
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
	/*if(key == GLUT_KEY_DOWN)
	{
	if (CIRCLE_POINTS > 3)
	CIRCLE_POINTS--;
	}*/
	if(key == GLUT_KEY_LEFT)
	{
		RENDER_STEP = true;
	}
	/*if(key == GLUT_KEY_RIGHT)
	...
	*/

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
	GLfloat nRange = 25.0f;

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
		glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
	else 
		glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

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

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1024, 600);
	glutCreateWindow("Fejlett Grafikai Algoritmusok - Süni");

	// >> Inicializálás

	RENDER_PAUSE = false;
	graphObjSet = new set<GraphObject*>();


	float radius = 2.0;
	float placeRadius = radius * 4;
	float angle = 0.0;
	int circleNum = 10;
	float xShift = 0.0;
	float yShift = 0.0;
	float vX = 0.074;
	float vY = 0.063;
	for(int i=0; i < circleNum; ++i)
	{
		vX = 0.074 + 0.074 * ((rand() % 20) - 10) * 0.10;
		vY = 0.063 + 0.063 * ((rand() % 20) - 10) * 0.10;

		float objAngle = (rand() % 100) / 100.0 * GL_PI;
		graphObjSet->insert(new GraphObject(placeRadius * cos(angle) + xShift, placeRadius * sin(angle) + yShift, radius, rand() % 5 + 3, objAngle,
			
			//new MyColor(i*0.1 + 0.3, i*0.1, 0.8),
			CLR_NORMAL,

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


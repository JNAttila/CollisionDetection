#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <GL/glut.h>

// Define a constant for the value of PI
#define GL_PI 3.1415f

GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;

GLfloat ambientLight[] = {0.4f, 0.4f, 0.4f, 1.0f};
GLfloat diffuseLight[] = {0.6f, 0.6f, 0.6f, 1.0f};
GLfloat lightPos[] = {25.0f, 25.0f, 25.0f, 1.0f};
GLfloat specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};

GLfloat specref[] = {1.0f, 1.0f, 1.0f, 1.0f};

time_t timestamp;

int slices = 160;
int stacks = 160;
float radius = 10.0f;

GLfloat* normals = NULL;
GLfloat* vertices = NULL;

//  gömb modellezése normálvektorokkal
void drawSphere(int n, int m, float r)
{
	int i, j;
	GLfloat angle_y, angle_x;

	if(n < 3)
		n = 3;

	for(i = 0, angle_y = -GL_PI / 2; i < n; i++, angle_y += GL_PI / n) {
		glBegin(GL_QUAD_STRIP);
		for(j = 0, angle_x = 0.0; j <= m; j++, angle_x += 2.0 * GL_PI / n) {
			glNormal3f(cos(angle_x)*cos(angle_y), sin(angle_y), sin(angle_x)*cos(angle_y));
			glVertex3f(r * cos(angle_x)*cos(angle_y), r * sin(angle_y), r*sin(angle_x)*cos(angle_y));

			glNormal3f(cos(angle_x)*cos(angle_y + GL_PI/n), sin(angle_y + GL_PI/n), sin(angle_x)*cos(angle_y + GL_PI/n));
			glVertex3f(r * cos(angle_x)*cos(angle_y + GL_PI/n), r * sin(angle_y + GL_PI/n), r*sin(angle_x)*cos(angle_y + GL_PI/n));
		}
		glEnd();
	}
  
}


void buildSphere(int n, int m, float r)
{
	int i, j;
	GLfloat angle_y, angle_x;

	int index = 0;

	for(i = 0, angle_y = -GL_PI / 2; i < n; i++, angle_y += GL_PI / n) {

		for(j = 0, angle_x = 0.0; j <= m; j++, angle_x += 2.0 * GL_PI / n) {

			normals[index + 0] = cos(angle_x)*cos(angle_y);
			normals[index + 1] = sin(angle_y);
			normals[index + 2] = sin(angle_x)*cos(angle_y);

			vertices[index + 0] = r * normals[index + 0];
			vertices[index + 1] = r * normals[index + 1];
			vertices[index + 2] = r * normals[index + 2];
			
			normals[index + 3] = cos(angle_x)*cos(angle_y + GL_PI/n);
			normals[index + 4] = sin(angle_y + GL_PI/n);
			normals[index + 5] = sin(angle_x)*cos(angle_y + GL_PI/n);

			vertices[index + 3] = r * normals[index + 3];
			vertices[index + 4] = r * normals[index + 4];
			vertices[index + 5] = r * normals[index + 5];
			
			index += 6;
		}

	}
  
}


// Called to draw scene
void RenderScene(void)
{
	int i;
	double angle;

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	// >> Modellezo programresz

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specref);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 64);

	glColor3f(1.0f, 0.0f, 0.0f);

//	drawSphere(stacks, slices, radius);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normals);

	glDrawArrays(GL_QUAD_STRIP, 0, stacks*(slices+1)*2);

	// << Modellezo programresz

	glPopMatrix();

	printf("Eltelt ido: %d\n", clock()-timestamp);
	timestamp = clock();
  
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

	glEnable(GL_LIGHTING);
  
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glEnable(GL_LIGHT0);

	vertices = (GLfloat*)malloc(stacks*(slices+1)*2*3*sizeof(GLfloat));
	normals = (GLfloat*)malloc(stacks*(slices+1)*2*3*sizeof(GLfloat));

	buildSphere(stacks, slices, radius);

	timestamp = clock();
}

void SpecialKeys(int key, int x, int y)
{
	// ...
	int state;

	if(key == GLUT_KEY_UP)
		xRot-= 5.0f;
  
	if(key == GLUT_KEY_DOWN)
		xRot += 5.0f;
  
	if(key == GLUT_KEY_LEFT)
		yRot -= 5.0f;
  
	if(key == GLUT_KEY_RIGHT)
		yRot += 5.0f;
  
	if(xRot > 356.0f)
		xRot = 0.0f;
  
	if(xRot < 0.0f)
		xRot = 355.0f;
  
	if(yRot > 356.0f)
		yRot = 0.0f;
  
	if(yRot < -1.0f)
		yRot = 355.0f;

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
	printf("Timer esemeny (%d)\n", value);

	glutPostRedisplay();
	glutTimerFunc(1000, Timer, value + 1);
}

void Idle()
{

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
	gluPerspective(	60.0f,    // fovy
					fAspect,  // aspect
					10.0,     // zNear
					100.0     // zFar
					);
	gluLookAt(	0.0, 0.0, 50.0, // eye
				0.0, 0.0, 0.0,  // center
				0.0, 1.0, 0.0   // up
				);
  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	// >> Inicializalas

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//glutInitWindowSize(300, 300);
	glutCreateWindow("GLUT Alap");

	// << Inicializalas

	// >> Callback fuggvenyek

	glutReshapeFunc(ChangeSizeOrtho); // Parhuzamos vetites
	//glutReshapeFunc(ChangeSizePerspective); // Perspektiv vetites

	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(RenderScene);
	//glutTimerFunc(1000, Timer, 1); // 1 mp mulva meghivodik a Timer() fuggveny
	glutIdleFunc(Idle); // Idle(), ha nem tortenik semmi
  
	// << Callback fuggvenyek

	SetupRC();
	glutMainLoop();
  
	return 0;
}
/*
Lecture 25
http://slides.com/javiergs/ser332-l25
javiergs@asu.edu

Code reused from github above

Authors: Elizabeth Layman
		 Jacob Janas
		 Erik Isaacson

Date	: 8/30/2018
*/

#pragma warning(disable : 4996)

#include "Globals.h"
#include "Lane.h"
#include "Skybox.h"

int width = 1200;
int height = 600;
float ratio = 1.0;

//create variables to hold objects
Lane *lane;
Skybox *skybox;

// controling parameters
int mouse_button;
int mouse_x = 0;
int mouse_y = 0;
float scale = 0.3;
float x_angle = 0.0;
float y_angle = 0.0;

//variables to control camera position and looking at
float camera_pos_x = 0.0f;
float cam_last_x = 0.0f;

float camera_pos_y = 40.0f;
float cam_last_y = 0.0f;

float camera_pos_z = 320.0f;
float cam_last_z = 0.0f;

float camera_look_x = 0.0f;
float camera_look_y = 0.0f;
float camera_look_z = -1.0f;

float angle = 0.0f;

// init
void init() {
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	ratio = (double)width / (double)height;

	//create Lane object
	lane = new Lane();
	
	//create skybox object
	skybox = new Skybox();

	// light
	GLfloat light_ambient[] = { 0.6, 0.6, 0.6, 0.5 };
	GLfloat light_diffuse[] = { 0.3, 0.3, 0.3, 0.3 };
	GLfloat light_specular[] = { 0.5, 0.5, 0.5, 0.3 };
	GLfloat light_position[] = { 0.0, 0.0, 3500.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

// reshape
void reshape(int w, int h) {
	width = w;
	height = h;
	if (h == 0) h = 1;
	ratio = 1.0f * w / h;
}

// mouse
void mouse(int button, int state, int x, int y) {
	mouse_x = x;
	mouse_y = y;
	mouse_button = button;
}

//keyboard
void keyboard(unsigned char key, int x, int y) {

	//move camera forward if w pressed
	if (key == 'w') {
		camera_pos_z = camera_pos_z - 10;
		camera_look_z = camera_look_z - 10;


	}

	//move camera back if s pressed
	else if (key == 's') {
		camera_pos_z = camera_pos_z + 10;
		camera_look_z = camera_look_z + 10;
	}

	glutPostRedisplay();
}

// motion
void motion(int x, int y) {
	if (mouse_button == GLUT_LEFT_BUTTON) {
		y_angle += (float(x - mouse_x) / width) *360.0;
		x_angle += (float(y - mouse_y) / height)*360.0;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON) {
		scale += (y - mouse_y) / 100.0;
		if (scale < 0.1) scale = 0.1;
		if (scale > 7)	scale = 7;
	}
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}

// text
void renderBitmapString(float x, float y, float z, const char *string) {
	const char *c;
	glRasterPos3f(x, y, z);   // fonts position
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}

// display
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(45, ratio, 1, 3000);

	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// lookAt
	gluLookAt(camera_pos_x, camera_pos_y, camera_pos_z,
		camera_look_x, camera_look_y, camera_look_z,
		0.0f, 1.0f, 0.0f);

	// camera
	glScalef(scale, scale, scale);
	glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
	glRotatef(y_angle, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);

	//plane
	glPushMatrix();
	glTranslatef(-900, -900, -900);
	glCallList(lane->getDisplayList());
	glPopMatrix();
	
	// skybox
	glPushMatrix();
	glTranslatef(-2500, -1000, -2500);
	glCallList(skybox->getDisplayList());
	glPopMatrix();

	// end
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	// texto
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	renderBitmapString(0.0, height - 13.0f, 0.0f, "Use [Mouse Left Key] to rotate");
	renderBitmapString(0.0, height - 26.0f, 0.0f, "Use [Mouse Right Key] to scale");
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glutSwapBuffers();
}

// main
void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Textures");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	init();

	glutMainLoop();
}
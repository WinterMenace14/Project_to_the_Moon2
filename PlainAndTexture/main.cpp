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
#include "FlatPlain.h"
#include "Box.h"

int width = 1200;
int height = 600;
float ratio = 1.0;

//create variables to hold objects
Lane *lane;

/*
Creating a small enviornment with stencil and reflections
*/

Skybox *skybox;

//FlatPlain *flatPlain; for lava

//mirror
FlatPlain *mirror;

//normal box
Box *box;

// controling parameters
int mouse_button;
int mouse_x = 0;
int mouse_y = 0;
float scale = 0.3;
float x_angle = 0.0;
float y_angle = 0.0;

float box_pos_x = 0;
float box_pos_y = -800;
float box_pos_z = 200;

//variables to control camera position and looking at
float camera_pos_x = box_pos_x + 15; //orig 0
float camera_pos_y = box_pos_y + 660; //original pos_y = -135.0f
float camera_pos_z = box_pos_z + 50; //orig 0

float camera_look_x = box_pos_x + 15; //orig 0
float camera_look_y = box_pos_y + 640; //orig -200
float camera_look_z = box_pos_z - 50; //orig -1

float angle = 0.0f;

//create variables to control what features are displayed (default all are true)
bool showBoundingBox = true;
bool showFog = true;
bool showSkyBox = true;

//create a menu listener and pass in menu option
void menuListener(int option) {
	//check passed in option
	switch (option) {
	case 1:
		if (showFog) {
			showFog = false;
		}
		else
			showFog = true;
		break;
	case 2:
		if (showBoundingBox) {
			showBoundingBox = false;
		}
		else
			showBoundingBox = true;
		break;
	case 3:
		if (showSkyBox) {
			showSkyBox = false;
		}
		else
			showSkyBox = true;
		break;
	}
	glutPostRedisplay();
}

//menu function to create menus to swap between showing
//features
void createMenus() {

	//create fog menu
	int fogMenu = glutCreateMenu(menuListener);
	glutAddMenuEntry("Enable/Disable", 1);

	//create bounding box menu
	int boundBoxMenu = glutCreateMenu(menuListener);
	glutAddMenuEntry("Enable/Disable", 2);

	//create skybox menu
	int skyBoxMenu = glutCreateMenu(menuListener);
	glutAddMenuEntry("Enable/Disable", 3);

	//create main menu
	int mainMenu = glutCreateMenu(menuListener);
	glutAddSubMenu("Fog", fogMenu);
	glutAddSubMenu("AABB", boundBoxMenu);
	glutAddSubMenu("Skybox", skyBoxMenu);

	//attatch menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// init
void init() {
	createMenus();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	ratio = (double)width / (double)height;

	//create Lane object
	lane = new Lane();
	
	//create skybox object
	skybox = new Skybox();

	//mirror object
	mirror = new FlatPlain(1000, 1000);

	//create flatPlain object
	//flatPlain = new FlatPlain();

	//create box for quiz 3
	box = new Box(100, 100, 100);

	//create cube object
	//cube = new Cube();

	// light
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 }; //0.6, 0.6, 0.6, 0.5
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; //0.3, 0.3, 0.3, 0.3
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //0.5, 0.5, 0.5, 0.3
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 }; //0.0, 0.0, 3500.0, 0.0
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glClearStencil(0);

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	GLfloat fogColor[4] = { 0.5, 0.5, 0.5, 1.0 };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.25);
	glFogf(GL_FOG_START, 10.0);
	glFogf(GL_FOG_END, 6000.0);

}

// rotate what the user see
void rotate_point(float angle) {
	float s = sin(angle);
	float c = cos(angle);
	// translate point back to origin:
	camera_look_x -= camera_pos_x;
	camera_look_z -= camera_pos_z;
	// rotate point
	float xnew = camera_look_x * c - camera_look_z * s;
	float znew = camera_look_x * s + camera_look_z * c;
	// translate point back:
	camera_look_x = xnew + camera_pos_x;
	camera_look_z = znew + camera_pos_z;
}

// reshape
void reshape(int w, int h) {
	width = w;
	height = h;
	if (h == 0) h = 1;
	ratio = 1.0f * w / h;
}

// mouse
/*void mouse(int button, int state, int x, int y) {
	mouse_x = x;
	mouse_y = y;
	mouse_button = button;
}*/

//keyboard
void keyboard(unsigned char key, int x, int y) {

	//move camera forward if w pressed
	if (key == 'a') {

		//change x pos of cam and box
		//camera_pos_x += -10;
		box_pos_x += -25;
		//rotation of the camera
		/*angle += -1.0f;
		rotate_point(-1);*/
	}

	//move camera back if s pressed

	else if (key == 'd') {

		//camera_pos_x += 10;
		box_pos_x += 25;
		//rotation of the camera
		/*angle += 1.0f;
		rotate_point(1);*/
	}

	else if (key == 'w') {
		//move camera pos
		//camera_pos_x += (10) * sin(angle);//*0.1;
		//camera_pos_z += (10) * -cos(angle);//*0.1;

		//move box pos z
		//box_pos_x += (25) * sin(angle);
		box_pos_z += (25) * -cos(angle);

		//change looking
		//camera_look_x += (10) * sin(angle);//*0.1;
		//camera_look_z += (10) * -cos(angle);//*0.1;
	}

	else if (key == 's') {
		//camera_z += 10;
		//camera_pos_x += (10) * sin(angle);//*0.1;
		//camera_pos_z += (-10) * -cos(angle);//*0.1;

		//move box pos z
		//box_pos_x += (-25) * sin(angle);
		box_pos_z += (-25) * -cos(angle);

		//camera_viewing_y -= 10;
		//camera_look_x += (10) * sin(angle);//*0.1;
		//camera_look_z += (-10) * -cos(angle);//*0.1;
	}

	glutPostRedisplay();
}

// motion
/*void motion(int x, int y) {
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
}*/

// text
void renderBitmapString(float x, float y, float z, const char *string) {
	const char *c;
	glRasterPos3f(x, y, z);   // fonts position
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}

// display
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(45, ratio, 1, 10000);

	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// lookAt
	gluLookAt(box_pos_x + 15, box_pos_y + 660, box_pos_z + 50,
		box_pos_x + 15, box_pos_y + 640, box_pos_z - 50,
		0.0f, 1.0f, 0.0f);

	// camera
	glScalef(scale, scale, scale);
	glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
	glRotatef(y_angle, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);

	//create the stencil
	/*glEnable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //Disable writing colors in frame buffer
		glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF); //Place a 1 where rendered
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); 	//Replace where rendered

		// mirror for the stencil
		glPushMatrix();
		glTranslatef(-400, 0, 0);
		glRotatef(90, 1, 0, 0);
		glCallList(mirror->getDisplayList());
		glPopMatrix();
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //Reenable color
	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Keep the pixel

	//draw reflected box
	glPushMatrix();
	glTranslatef(box_pos_x * -1, -800, box_pos_z * -1);
	glScalef(1, 1, -1);
	glCallList(box->getDisplayList());
	glPopMatrix();

	//diable stencil
	glDisable(GL_STENCIL_TEST); */

	//plain
	glPushMatrix();
	glTranslatef(-5000, -800, -5000);
	glCallList(lane->getDisplayList());
	glPopMatrix();

	// Displaying Fog
	/*********************************************************************/
	if (!showFog) {
		glDisable(GL_FOG);
	}

	else if (showFog) {
		glEnable(GL_FOG);
	}
	/*********************************************************************/

	// skybox
	/*********************************************************************/
	if (showSkyBox) {
		glPushMatrix();
		glTranslatef(-10000, -3000, -10000); //-2500, -1000, -2500
		glCallList(skybox->getDisplayList());
		glPopMatrix();
	}
	/*********************************************************************/

	//enable blend and disable light
	/*glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.7, 0.0, 0.0, 0.3);
	glColor4f(1.0, 1.0, 1.0, 0.3);
	//draw the mirror
	glPushMatrix();
	glTranslatef(-400, 0, 0);
	glRotatef(90, 1, 0, 0);
	glCallList(mirror->getDisplayList());
	glPopMatrix();

	//enable lighting and disable blend
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);*/

	//lava
	/*
	glPushMatrix();
	glTranslatef(-10000, -800, -10000);
	glCallList(flatPlain->getDisplayList());
	glPopMatrix();*/

	//box
	glPushMatrix();
	glTranslatef(box_pos_x, box_pos_y, box_pos_z);
	glCallList(box->getDisplayList());
	glPopMatrix();

	//Bounding Boxes - By default are enabled
	if (showBoundingBox) {
		/***********************************************************************************/
		//draw the bounding box around the box
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(box_pos_x, box_pos_y, box_pos_z);
		glCallList(box->getBoundingBox());
		glEnable(GL_LIGHTING);
		glPopMatrix();

		/************************************************************************************/
	}

	// end - of all drawing
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
	renderBitmapString(0.0, height - 13.0f, 0.0f, "Use w, a, s, d, to move camera around.");
	renderBitmapString(0.0, height - 26.0f, 0.0f, "Use i, k, to move forward and back.");
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glutSwapBuffers();
}

// main
void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Textures");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	//glutMouseFunc(mouse);
	//glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	init();

	glutMainLoop();
}
#include "ToTheMoon.h"
#include "timer.h"

ToTheMoon::ToTheMoon() {
	//basic constructor
}

ToTheMoon::~ToTheMoon() {
	//generic destructor
}

//init function
void ToTheMoon::init() {

	//setup graphics enviornment and objects within the world
	//this->createMenus();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	this->ratio = (double)width / (double)height;

	//initialize frame timer
	init_frame_timer();

	//create game objects
	this->lane = new Lane();
	this->skybox = new Skybox();
	this->flatPlain = new FlatPlain(1000, 1000);
	this->box = new Box(100, 100, 100);
	this->p1 = new Player();
	this->ps = new ParticleSystem();
	this->keyboard = new Keyboard();

	//light
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

	//clear stencil buffer
	glClearStencil(0);

	//fog settings
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	GLfloat fogColor[4] = { 0.5, 0.5, 0.5, 1.0 };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.25);
	glFogf(GL_FOG_START, 3000.0);
	glFogf(GL_FOG_END, 6000.0);
}

void ToTheMoon::render() {

	//draw graphics to the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_CULL_FACE);

	// projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, 0, this->width, this->height);
	gluPerspective(45, this->ratio, 1, 10000);

	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// lookAt Game Mode
	gluLookAt(this->camera_pos_x, this->camera_pos_y, this->camera_pos_z,
		this->camera_look_x, this->camera_look_y, this->camera_look_z,
		0.0f, 1.0f, 0.0f);

	// camera
	glScalef(this->scale, this->scale, this->scale);
	glRotatef(this->x_angle, 1.0f, 0.0f, 0.0f);
	glRotatef(this->y_angle, 0.0f, 1.0f, 0.0f);
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
	//glPushMatrix();
	//glTranslatef(-5000, -800, -5000);
	//glCallList(lane->getDisplayList());
	//glPopMatrix();

	// Displaying Fog
	/*********************************************************************/
	if (this->showFog == false) {
		glDisable(GL_FOG);
	}
	else if (this->showFog) {
		glEnable(GL_FOG);
	}
	/*********************************************************************/

	// Skybox
	/*********************************************************************/
	if (showSkyBox) {
		glPushMatrix();
		glTranslatef(-10000, -3000, -10000); //-2500, -1000, -2500
		glCallList(this->skybox->getDisplayList());
		glPopMatrix();
	}
	/*********************************************************************/

	// Displaying Flat Plain
	/*********************************************************************/
	/*if (!showFlatPlain) {
		glPushMatrix();
		glTranslatef(-5000, -800, -5000);
		glCallList(lane->getPerlinDisplayList());
		glPopMatrix();
	}*/
	glPushMatrix();
	glTranslatef(-5000, -800, -5000);
	glCallList(this->lane->getDisplayList());
	glPopMatrix();
	/*********************************************************************/

	//Display Hill(NURBS)
	/*********************************************************************/
	// NURBS
	// V_size curves with U_size control points each
	// V_size + ORDER knots per curve and U_size + ORDER knots per inter-curve conection (controlpnt + 4)
	// V_size*3 and 3 offsets
	// cubic equations (4)
	/*
	const int V_size = 4;
	const int U_size = 4;
	const int ORDER = 4;
	GLfloat ctlpoints[U_size][V_size][3] = {
		{ { 70, -80, -20 } ,{ 70, -80, -10 },{ 70, -80, 10 },{ 70, -80, 20 } },
		{ { 90, -80, -20 } ,{ 90, -60, 0 },{ 90, -60, 0 },{ 90, -80, 20 } },
		{ { 90, -80, -20 } ,{ 90, -60, 0 },{ 90, -60, 0 },{ 90, -80, 20 } },
		{ { 110, -80, -20 } ,{ 110, -80, -10 },{ 110, -80, 10 },{ 110, -80, 20 } }
	};
	GLfloat vknots[V_size + ORDER] = { 0.0, 0.0, 0.0, 0.0, 3.0, 3.0, 3.0, 3.0 };
	GLfloat uknots[U_size + ORDER] = { 0.0, 0.0, 0.0, 0.0, 3.0, 3.0, 3.0, 3.0 };

	GLUnurbsObj *theNurb;
	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

	glPushMatrix();
	glScalef(10, 10, 10);
	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb,
		U_size + ORDER, uknots,
		V_size + ORDER, vknots,
		V_size * 3,
		3,
		&ctlpoints[0][0][0],
		ORDER, ORDER,
		GL_MAP2_VERTEX_3);
	gluEndSurface(theNurb);

	// control graph
	/*
	glDisable(GL_LIGHTING);
	glPointSize(1.0);
	glColor3f(0, 0, 1);
	for (int i = 0; i < U_size; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < V_size; j++) {
			glVertex3f(ctlpoints[i][j][0], ctlpoints[i][j][1], ctlpoints[i][j][2]);
		}
		glEnd();
	}
	*/
	// show control points
	/*
	glPointSize(5.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < U_size; i++) {
		for (int j = 0; j < V_size; j++) {
			glVertex3f(ctlpoints[i][j][0], ctlpoints[i][j][1], ctlpoints[i][j][2]);
		}
	}
	glEnd();
	
	glEnable(GL_LIGHTING);
	glPopMatrix();
	*/
	/*********************************************************************/
	//Shooting Star(Hermite)
	/*********************************************************************/
	glBegin(GL_LINE_STRIP);
	// use the parametric time value 0 to 1
	for (int i = 0; i != N; ++i) {
		float t = (float)i / (N - 1);
		// calculate blending functions
		float b0 = 2 * t*t*t - 3 * t*t + 1;
		float b1 = -2 * t*t*t + 3 * t*t;
		float b2 = t * t*t - 2 * t*t + t;
		float b3 = t * t*t - t * t;
		// calculate the x, y and z of the curve point
		float x = b0 * Geometry[0][0] + b1 * Geometry[1][0] + b2 * Geometry[2][0] + b3 * Geometry[3][0];
		float y = b0 * Geometry[0][1] + b1 * Geometry[1][1] + b2 * Geometry[2][1] + b3 * Geometry[3][1];
		float z = b0 * Geometry[0][2] + b1 * Geometry[1][2] + b2 * Geometry[2][2] + b3 * Geometry[3][2];
		verticies[i][0] = x;
		verticies[i][1] = y;
		verticies[i][2] = z;
		// specify the point
		glVertex3f(x, y, z);
	}
	glEnd();

	int point = 0;
	glPointSize(20.0);
	glColor3f(1, 1, 0);
	glPushMatrix();
	glBegin(GL_POINTS);
	if (point >= 1) {
		glTranslatef(verticies[point][0] - verticies[point - 1][0], verticies[point][1] - verticies[point - 1][1], verticies[point][2] - verticies[point - 1][2]);
		point++;
	}
	if (point == 19) {
		point = 0;
	}
	glVertex3f(verticies[0][0], verticies[0][1], verticies[0][2]);
	glEnd();
	glPopMatrix();

	/*********************************************************************/
	//Particle system attatched to the star
	/*********************************************************************/

	//createing particle system
	ps->add();
	float time = calculate_frame_time();
	ps->update(time);
	ps->remove();

	//drawing particle system
	glPushMatrix();
	drawParticles();
	glPopMatrix();

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

	//Create Game Objects
	/************************************************************************************/
	//box
	/*glPushMatrix();
	glTranslatef(0, -800, 200);
	glCallList(astroDisplayList);
	glPopMatrix();*/
	//box
	glPushMatrix();
	glTranslatef(this->box_pos_x, this->box_pos_y, this->box_pos_z);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(25.0, 25.0, 25.0);
	glCallList(this->p1->getDisplayList());
	glPopMatrix();
	/************************************************************************************/

	//Bounding Boxes - By default are enabled
	/************************************************************************************/
	/*if (showBoundingBox) {
		//draw the bounding box around the box
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(box_pos_x, box_pos_y, box_pos_z);
		glCallList(box->getBoundingBox());
		glEnable(GL_LIGHTING);
		glPopMatrix();

	}
	/************************************************************************************/


	// end - of all drawing
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// texto
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, this->width, 0, this->height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	renderBitmapString(0.0, this->height - 13.0f, 0.0f, "Use w, a, s, d, to move camera around.");
	renderBitmapString(0.0, this->height - 26.0f, 0.0f, "Use i, k, to move forward and back.");
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glDisable(GL_CULL_FACE);
	glutSwapBuffers();
}

//function to handle the game logic
void ToTheMoon::logic() {

	//handle input from the user
	if (this->keyboard->getKeys()[P1_FORWARD]) {
		this->camera_look_z += -10;
		this->camera_pos_z += -10;
		this->box_pos_z += -33.333333;
	}

	if (this->keyboard->getKeys()[P1_LEFT]) {
		this->camera_look_x += -10;
		this->camera_pos_x += -10;
		this->box_pos_x += -33.333333;
	}

	if (this->keyboard->getKeys()[P1_RIGHT]) {
		this->camera_look_x += 10;
		this->camera_pos_x += 10;
		this->box_pos_x += 33.333333;
	}

	if (this->keyboard->getKeys()[P1_BACK]) {
		this->camera_look_z += 10;
		this->camera_pos_z += 10;
		this->box_pos_z += 33.333333;
	}
}

//store the player mode camera
void ToTheMoon::setEditorCamera() {

	//store old camera pos
	this->oldPosX = this->camera_pos_x;
	this->oldPosY = this->camera_pos_y;
	this->oldPosZ = this->camera_pos_z;
	this->oldLookX = this->camera_look_x;
	this->oldLookY = this->camera_look_y;
	this->oldLookZ = this->camera_look_z;

	//set editor camera pos & look
	this->camera_pos_x = 500;
	this->camera_pos_y = -100;
	this->camera_pos_z = 300;

	this->camera_look_x = 0;
	this->camera_look_y = -160;
	this->camera_look_z = 0;
}

//restore the player camera
void ToTheMoon::loadPlayerCamera() {

	this->scale = .3f;
	this->x_angle = 0;
	this->y_angle = 0;

	this->camera_pos_x = this->oldPosX;
	this->camera_pos_y = this->oldPosY;
	this->camera_pos_z = this->oldPosZ;
	this->camera_look_x = this->oldLookX;
	this->camera_look_y = this->oldLookY;
	this->camera_look_z = this->oldLookZ;

}

//Game loop to handle the logic and render
void ToTheMoon::gameLoop() {

	//check to see that the game is running and set frame rate
	int t1, t2;

	t1 = glutGet(GLUT_ELAPSED_TIME);

	if (this->running) {
		this->logic();
		this->render();
	}

	do {
		t2 = glutGet(GLUT_ELAPSED_TIME);
	} while (t2 - t1 < 1000 / FRAMERATE);
}

//read in mouse input
void ToTheMoon::mouse(int button, int state, int x, int y) {
	this->mouse_x = x;
	this->mouse_y = y;
	this->mouse_button = button;
}

//motion around scene
void ToTheMoon::motion(int x, int y) {
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

//rotate the camera
void ToTheMoon::rotate_point(float angle) {
	float s = sin(angle);
	float c = cos(angle);
	// translate point back to origin:
	this->camera_look_x -= this->camera_pos_x;
	this->camera_look_z -= this->camera_pos_z;
	// rotate point
	float xnew = this->camera_look_x * c - this->camera_look_z * s;
	float znew = this->camera_look_x * s + this->camera_look_z * c;
	// translate point back:
	this->camera_look_x = xnew + this->camera_pos_x;
	this->camera_look_z = znew + this->camera_pos_z;
}

//reshape
void ToTheMoon::reshape(int w, int h) {
	this->width = w;
	this->height = h;
	if (h == 0) h = 1;
	this->ratio = 1.0f * w / h;
}

//return the keyboard input
Keyboard* ToTheMoon::getKeyboard() {
	return this->keyboard;
}

//write passed in text to screen
void ToTheMoon::renderBitmapString(float x, float y, float z, const char *string) {
	const char *c;
	glRasterPos3f(x, y, z);   // fonts position
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}

// draw particles
void ToTheMoon::drawParticles() {
	Particle* curr = ps->particle_head;
	// glPointSize(2);
	// glBegin(GL_POINTS);
	// while (curr) {
	//   glVertex3fv(curr->position);
	//	 curr = curr->next;
	// }
	// glEnd();
	while (curr) {
		glPushMatrix();
		//glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		glScalef(100.0, 100.0, 100.0);
		glTranslatef(curr->position[0], curr->position[1], curr->position[2]);
		glScalef(0.001, 0.001, 0.001);
		glCallList(box->getDisplayList());
		glPopMatrix();
		curr = curr->next;
	}

}
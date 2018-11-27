#include "ToTheMoon.h"

ToTheMoon game;

//global variables for window
int width = 1200;
int height = 600;
float ratio = 1.0f;

//create callback functions
void idle() {
	game.gameLoop();
}

void display() {
	game.render();
}

void gameKeyboardDown(unsigned char key, int x, int y) {
	game.getKeyboard()->readKeyboard(key, x, y, true);
}

void gameKeyboardUp(unsigned char key, int x, int y) {
	game.getKeyboard()->readKeyboard(key, x, y, false);
}

// main
void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Textures");
	game.init();
	//glutReshapeFunc(gameReshape);	Come back at a later time
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	//glutMouseFunc(mouse);
	//glutMotionFunc(motion);
	glutKeyboardFunc(gameKeyboardDown);
	glutKeyboardUpFunc(gameKeyboardUp);

	glutMainLoop();
}
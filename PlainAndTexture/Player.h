#include "Globals.h"

class Player {

public:
	Player();
	~Player();
	Mesh* getMesh();
	GLuint getDisplayList();
	void calculateNormalPerFace();
	void calculateNormalPerVertex();
	void meshToDisplayList();

private:
	Mesh* mesh;
	GLuint display;
	GLuint texture;
};
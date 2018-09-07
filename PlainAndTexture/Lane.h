
#include "Globals.h"

class Lane {

public:
	Lane();
	~Lane();
	Mesh* getMesh();
	GLuint getDisplayList();
	GLuint getTexture();
	Mesh* createPlane(int arena_width, int arena_depth, int arena_cell);
	void calculateNormalPerFace();
	void calculateNormalPerVertex();
	void codedTexture();
	void meshToDisplayList();

private:
	Mesh* mesh;
	GLuint display;
	GLuint texture;
};
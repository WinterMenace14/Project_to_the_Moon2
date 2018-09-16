
#include "Globals.h"

class Box {

public:
	Box(int w, int h, int d);
	~Box();
	Mesh* getMesh();
	GLuint getDisplayList();
	Mesh* createCube();
	void calculateNormalPerFace();
	void calculateNormalPerVertex();
	void boxToDisplayList();
	void bmpTexture(UINT texture, const char *file);

private:
	Mesh* mesh;
	GLuint display;
	GLuint texture;
	int width, height, depth;
};
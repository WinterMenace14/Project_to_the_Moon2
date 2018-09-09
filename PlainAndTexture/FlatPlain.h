#include "Globals.h"

class FlatPlain {

public:
	FlatPlain();
	~FlatPlain();
	Mesh* getMesh();
	GLuint getDisplayList();
	GLuint getTexture();
	Mesh* createFlatPlane(int arena_width, int arena_depth, int arena_cell);
	void calculateNormalPerFace();
	void calculateNormalPerVertex();
	void meshToDisplayList();
	void bmpTexture(UINT texture, const char *file);
	void codedTexture();

private:
	Mesh* mesh;
	GLuint display;
	GLuint texture;
};

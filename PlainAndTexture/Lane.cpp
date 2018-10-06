#include "Lane.h"

Lane::Lane() {
	
	mesh = createPlane(10000, 10000, 200);
	perlinMesh = createPerlinPlain(10000, 10000, 20);
	calculateNormalPerFace();
	calculateNormalPerVertex();
	bmpTexture(this->texture, "bmp/brick.bmp");
	//codedTexture();
	meshToDisplayList();
}

Lane::~Lane() {
	//default destructor
}

Mesh* Lane::getMesh() {
	return this->mesh;
}

GLuint Lane::getDisplayList() {
	return this->display;
}

GLuint Lane::getPerlinDisplayList() {
	return this->perlinDisplay;
}

GLuint Lane::getTexture() {
	return this->texture;
}

void Lane::bmpTexture(UINT texture, const char *file) {
	BITMAPINFO *bitmapInfo; // Bitmap information
	GLubyte    *bitmapBits; // Bitmap data
	if (!file) {
		cout << "texture file not found!" << endl;
		return;
	}
	bitmapBits = LoadDIBitmap(file, &bitmapInfo);
	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // must set to 1 for compact data
																				 // glTexImage2D Whith size and minification
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bitmapInfo->bmiHeader.biWidth, bitmapInfo->bmiHeader.biHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmapBits);
}

// Create texture from algorithm
void Lane::codedTexture() {
	const int TexHeight = 128;
	const int TexWidth = 128;
	// create texture in memory
	GLubyte textureImage[TexHeight][TexWidth][3];
	ImprovedNoise noise;
	Vec3f pixelColor;
	for (int i = 0; i < TexHeight; i++)
		for (int j = 0; j < TexWidth; j++) {
			//pixelColor = marbleMap(t_scale(noise.perlinMarble(i * 5, j * 5)));
			pixelColor = lava(t_scale(noise.perlinFireMultiscale(i * 5, j * 5)));
			textureImage[i][j][0] = pixelColor[0] * 255;
			textureImage[i][j][1] = pixelColor[1] * 255;
			textureImage[i][j][2] = pixelColor[2] * 255;
		}
	// setup texture
	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // must set to 1 for compact data
																				 // glTexImage2D Whith size and minification
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TexWidth, TexHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, textureImage); // BGRA to include alpha

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

Mesh* Lane::createPlane(int arena_width, int arena_depth, int arena_cell) {
	Mesh *me = new Mesh;
	int n = arena_width / arena_cell;
	int m = arena_depth / arena_cell;
	ImprovedNoise noise;

	// vertices
	for (int i = 0; i < n; i++) {
		//double value = noise.perlinMultiscale(n, m);
		for (int j = 0; j < m; j++) {
			//double height = noise.perlinMultiscale(i, j);
			me->dot_vertex.push_back(Vec3<GLfloat>(i*arena_cell, 0.0, j*arena_cell));
		}
	}
	//texture
	me->dot_texture.push_back(Vec2<GLfloat>(0.0, 0.0));
	me->dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0));
	me->dot_texture.push_back(Vec2<GLfloat>(1.0, 0.0));
	me->dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0));

	// faces
	for (int i = 0; i < (n*m) - m; i++) {
		if ((i + 1) % n == 0) continue;
		me->face_index_vertex.push_back(i);
		me->face_index_vertex.push_back(i + 1);
		me->face_index_vertex.push_back(i + n);
		me->face_index_vertex.push_back(i + 1);
		me->face_index_vertex.push_back(i + n + 1);
		me->face_index_vertex.push_back(i + n);
		// index for texture
		for (int t = 0; t < 6; t++) {
			me->face_index_texture.push_back(3);//0
			me->face_index_texture.push_back(2);//2
			me->face_index_texture.push_back(1);//1
			me->face_index_texture.push_back(2);//0
			me->face_index_texture.push_back(0);//3
			me->face_index_texture.push_back(1);//2
		}
	}
	return me;
}

//perlin noise plain
Mesh* Lane::createPerlinPlain(int arena_width, int arena_depth, int arena_cell) {
	Mesh *me = new Mesh;
	int n = arena_width / arena_cell;
	int m = arena_depth / arena_cell;
	ImprovedNoise noise;

	// vertices
	for (int i = 0; i < n; i++) {
		//double value = noise.perlinMultiscale(n, m);
		for (int j = 0; j < m; j++) {
			//double height = noise.perlinMultiscale(i, j);
			me->dot_vertex.push_back(Vec3<GLfloat>(i*arena_cell, noise.perlinMultiscale(i, j), j*arena_cell));
		}
	}
	//texture
	me->dot_texture.push_back(Vec2<GLfloat>(0.0, 0.0));
	me->dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0));
	me->dot_texture.push_back(Vec2<GLfloat>(1.0, 0.0));
	me->dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0));

	// faces
	for (int i = 0; i < (n*m) - m; i++) {
		if ((i + 1) % n == 0) continue;
		me->face_index_vertex.push_back(i);
		me->face_index_vertex.push_back(i + 1);
		me->face_index_vertex.push_back(i + n);
		me->face_index_vertex.push_back(i + 1);
		me->face_index_vertex.push_back(i + n + 1);
		me->face_index_vertex.push_back(i + n);
		// index for texture
		for (int t = 0; t < 6; t++) {
			me->face_index_texture.push_back(3);//0
			me->face_index_texture.push_back(2);//2
			me->face_index_texture.push_back(1);//1
			me->face_index_texture.push_back(2);//0
			me->face_index_texture.push_back(0);//3
			me->face_index_texture.push_back(1);//2
		}
	}
	return me;
}

//normal per face
void Lane::calculateNormalPerFace() {
	Vec3<float> v1, v2, v3, v4, v5;
	for (int i = 0; i < this->mesh->face_index_vertex.size(); i += 3) {
		v1 = this->mesh->dot_vertex[this->mesh->face_index_vertex[i]];
		v2 = this->mesh->dot_vertex[this->mesh->face_index_vertex[i + 1]];
		v3 = this->mesh->dot_vertex[this->mesh->face_index_vertex[i + 2]];
		v4 = (v2 - v1);
		v5 = (v3 - v1);
		v4 = v4.cross(v5);
		v4.normalize();
		this->mesh->dot_normalPerFace.push_back(v4);
		int pos = this->mesh->dot_normalPerFace.size() - 1;
		// same normal for all vertex in this face
		this->mesh->face_index_normalPerFace.push_back(pos);
		this->mesh->face_index_normalPerFace.push_back(pos);
		this->mesh->face_index_normalPerFace.push_back(pos);
	}

	//perlin mesh
	for (int i = 0; i < this->perlinMesh->face_index_vertex.size(); i += 3) {
		v1 = this->perlinMesh->dot_vertex[this->perlinMesh->face_index_vertex[i]];
		v2 = this->perlinMesh->dot_vertex[this->perlinMesh->face_index_vertex[i + 1]];
		v3 = this->perlinMesh->dot_vertex[this->perlinMesh->face_index_vertex[i + 2]];
		v4 = (v2 - v1);
		v5 = (v3 - v1);
		v4 = v4.cross(v5);
		v4.normalize();
		this->perlinMesh->dot_normalPerFace.push_back(v4);
		int pos = this->perlinMesh->dot_normalPerFace.size() - 1;
		// same normal for all vertex in this face
		this->perlinMesh->face_index_normalPerFace.push_back(pos);
		this->perlinMesh->face_index_normalPerFace.push_back(pos);
		this->perlinMesh->face_index_normalPerFace.push_back(pos);
	}
}

// calculate normal per vertex
void Lane::calculateNormalPerVertex() {
	this->mesh->dot_normalPerVertex.clear();
	this->mesh->face_index_normalPerVertex.clear();
	Vec3<float> suma; suma.x = 0; suma.y = 0; suma.z = 0;
	//initialize
	for (unsigned int val = 0; val < this->mesh->dot_vertex.size(); val++) {
		this->mesh->dot_normalPerVertex.push_back(suma);
	}
	// calculate sum for vertex
	for (long pos = 0; pos < this->mesh->face_index_vertex.size(); pos++) {
		this->mesh->dot_normalPerVertex[this->mesh->face_index_vertex[pos]] +=
			this->mesh->dot_normalPerFace[this->mesh->face_index_normalPerFace[pos]];
	}
	// normalize for vertex 
	for (unsigned int val = 0; val < this->mesh->dot_normalPerVertex.size(); val++) {
		this->mesh->dot_normalPerVertex[val] = this->mesh->dot_normalPerVertex[val].normalize();
	}
	//normalVertexIndex is the same that vertexIndex
	for (unsigned int pos = 0; pos < this->mesh->face_index_vertex.size(); pos++) {
		this->mesh->face_index_normalPerVertex.push_back(this->mesh->face_index_vertex[pos]);
	}

	//perlin mesh
	suma.x = 0; suma.y = 0; suma.z = 0;
	for (unsigned int val = 0; val < this->perlinMesh->dot_vertex.size(); val++) {
		this->perlinMesh->dot_normalPerVertex.push_back(suma);
	}
	// calculate sum for vertex
	for (long pos = 0; pos < this->perlinMesh->face_index_vertex.size(); pos++) {
		this->perlinMesh->dot_normalPerVertex[this->perlinMesh->face_index_vertex[pos]] +=
			this->perlinMesh->dot_normalPerFace[this->perlinMesh->face_index_normalPerFace[pos]];
	}
	// normalize for vertex 
	for (unsigned int val = 0; val < this->perlinMesh->dot_normalPerVertex.size(); val++) {
		this->perlinMesh->dot_normalPerVertex[val] = this->perlinMesh->dot_normalPerVertex[val].normalize();
	}
	//normalVertexIndex is the same that vertexIndex
	for (unsigned int pos = 0; pos < this->perlinMesh->face_index_vertex.size(); pos++) {
		this->perlinMesh->face_index_normalPerVertex.push_back(this->perlinMesh->face_index_vertex[pos]);
	}
}

// draw
void Lane::meshToDisplayList() {
	this->display = glGenLists(1);
	glNewList(this->display, GL_COMPILE);
	//if (id != 3) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	//}
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < this->mesh->face_index_vertex.size(); i++) {
		// PER VERTEX NORMALS
		if ((!this->mesh->dot_normalPerVertex.empty() && !this->mesh->face_index_normalPerVertex.empty())) {
			glNormal3fv(&this->mesh->dot_normalPerVertex[this->mesh->face_index_normalPerVertex[i]].x);
		}
		// TEXTURES
		if (!this->mesh->dot_texture.empty() && !this->mesh->face_index_texture.empty()) {
			glTexCoord2fv(&this->mesh->dot_texture[this->mesh->face_index_texture[i]].x);
		}
		// COLOR
		//Vec3f offset = (this->mesh->dot_vertex[this->mesh->face_index_vertex[i]]);
		// VERTEX
		//glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
		glVertex3fv(&this->mesh->dot_vertex[this->mesh->face_index_vertex[i]].x);
	}
	glEnd();
	//if (id != 3) {
	glDisable(GL_TEXTURE_2D);
	//}
	glEndList();

	//perlinDisplay
	this->perlinDisplay = glGenLists(1);
	glNewList(this->perlinDisplay, GL_COMPILE);
	//if (id != 3) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	//}
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < this->perlinMesh->face_index_vertex.size(); i++) {
		// PER VERTEX NORMALS
		if ((!this->perlinMesh->dot_normalPerVertex.empty() && !this->perlinMesh->face_index_normalPerVertex.empty())) {
			glNormal3fv(&this->perlinMesh->dot_normalPerVertex[this->perlinMesh->face_index_normalPerVertex[i]].x);
		}
		// TEXTURES
		if (!this->perlinMesh->dot_texture.empty() && !this->perlinMesh->face_index_texture.empty()) {
			glTexCoord2fv(&this->perlinMesh->dot_texture[this->perlinMesh->face_index_texture[i]].x);
		}
		// COLOR
		//Vec3f offset = (this->perlinMesh->dot_vertex[this->perlinMesh->face_index_vertex[i]]);
		// VERTEX
		//glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
		glVertex3fv(&this->perlinMesh->dot_vertex[this->perlinMesh->face_index_vertex[i]].x);
	}
	glEnd();
	//if (id != 3) {
	glDisable(GL_TEXTURE_2D);
	//}
	glEndList();
}
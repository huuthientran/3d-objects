#pragma once
#include "../Point.h"
#include <cstring>

class Vertex {
private:
	Point pos;
	GLfloat color[3];
	GLfloat texCoord[2];
	GLfloat normal[3];
public:
	Vertex() {
		color[0] = 1.0f,		color[1] = 1.0f,		color[2] = 1.0f;
		texCoord[0] = -1.0f,	texCoord[1] = -1.0f;
		normal[0] = 0.0f,		normal[1] = 0.0f,		normal[2] = 0.0f;
	}

	~Vertex() {}

	Vertex(Point pos, GLfloat color[], GLfloat textCoord[], GLfloat normal[]) {
		this->pos = pos;
		memcpy(this->color, color, 3 * sizeof(GLfloat));
		memcpy(this->texCoord, textCoord, 2 * sizeof(GLfloat));
		memcpy(this->normal, normal, 3 * sizeof(GLfloat));
	}

	Vertex(Point other) : Vertex() {
		this->pos = other;
	}

	Vertex(const Vertex& other) {
		this->pos = other.pos;
		memcpy(this->color, other.color, 3 * sizeof(GLfloat));
		memcpy(this->texCoord, other.texCoord, 2 * sizeof(GLfloat));
		memcpy(this->normal, other.normal, 3 * sizeof(GLfloat));
	}

	Point getPos() { return this->pos; }

	void setColor(GLfloat color[]) {
		memcpy(this->color, color, 3 * sizeof(GLfloat));
	}

	void setTextureCoord(GLfloat textCoord[]) {
		memcpy(this->texCoord, textCoord, 2 * sizeof(GLfloat));
	}

	void setNormal(GLfloat normal[]) {
		memcpy(this->normal, normal, 3 * sizeof(GLfloat));
	}

	void render() {
		glColor3f(this->color[0], this->color[1], this->color[2]);
		
		if (this->texCoord[0] != -1.0f && this->texCoord[1] != -1.0f) 
			glTexCoord2f(this->texCoord[0], this->texCoord[1]);
	
		glVertex3f(this->pos.X(), this->pos.Y(), this->pos.Z());
	}
};
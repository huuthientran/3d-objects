#pragma once
#include "../SOIL.h"
#include <gl/glut.h>
#include <vector>
#include <iostream>

#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923

using namespace std; // std::vector

class Mesh {
protected:
	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLfloat> texcoords;
	vector<GLushort> indices;
public:
	virtual void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat angle)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(angle, 1.0f, 1.0f, 1.0f);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
		glNormalPointer(GL_FLOAT, 0, &normals[0]);
		glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
		glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
		glPopMatrix();
	}
};

class Cube : public Mesh {
public:
	Cube(GLfloat size) {
		vertices.resize(6 * 4 * 3);
		normals.resize(6 * 4 * 3);
		texcoords.resize(6 * 4 * 2);

		auto v = vertices.begin();
		auto n = normals.begin();
		auto t = texcoords.begin();

		for (GLuint i = 0; i < 6; ++i) {
			for (GLuint j = 0; j < 4; ++j) {
				//cout << "i and j: " << i << " " << j << "\n";
				GLfloat unit = (i < 3) ? 1.0f : -1.0f;
				GLfloat const x = (i % 3 == 0 || j % 4 < 2) ? unit : -unit;
				GLfloat const y = (i % 3 == 1 || (i % 3 == 0 && (j == 0 || j == 1)) || (i % 3 == 2 && (j == 0 || j == 3))) ? unit : -unit;
				GLfloat const z = (i % 3 == 2 || (j == 0 || j == 3)) ? unit : -unit;

				//cout << x << " " << y << " " << z << endl;

				*t++ = (i % 2 == 0 && (j == 0 || j == 3) || i % 2 == 1 && (j == 0 || j == 1)) ? 1.0f : 0.0f;
				*t++ = (i % 2 == 0 && (j == 0 || j == 1) || i % 2 == 1 && (j == 0 || j == 3)) ? 1.0f : 0.0f;
				//cout	<< "t = " << ((i % 2 == 0 && (j == 0 || j == 3) || i % 2 == 1 && (j == 0 || j == 1)) ? 1.0f : 0.0f) 
				//		<< " " << ((i % 2 == 0 && (j == 0 || j == 1) || i % 2 == 1 && (j == 0 || j == 3)) ? 1.0f : 0.0f) << endl;

				*v++ = x * size;
				*v++ = y * size;
				*v++ = z * size;

				*n++ = x;
				*n++ = y;
				*n++ = z;
			}
			//cout << endl;
		}

		indices.resize(6 * 4 * 4);
		std::vector<GLushort>::iterator it = indices.begin();
		for (GLuint i = 0; i < 5; i++) {
			for (GLuint j = 0; j < 4; j++) {
				*it++ = i * 4 + j;
				*it++ = i * 4 + (j + 1);
				*it++ = i * 4 + (j + 2);
				*it++ = i * 4 + (j + 3);
			}
		}
	}

	void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat angle) {
		Mesh::draw(x, y, z, angle);
	}
};

class Sphere : public Mesh {
public:
	Sphere(GLfloat radius, GLuint rings, GLuint sectors) {
		GLfloat const R = 1. / (GLfloat)(rings - 1);
		GLfloat const S = 1. / (GLfloat)(sectors - 1);
		GLuint r, s;

		vertices.resize(rings * sectors * 3);
		normals.resize(rings * sectors * 3);
		texcoords.resize(rings * sectors * 2);

		auto v = vertices.begin();
		auto n = normals.begin();
		auto t = texcoords.begin();

		for (r = 0; r < rings; ++r) {
			for (s = 0; s < sectors; ++s) {
				// TODO: maybe need to be changed later
				GLfloat const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
				GLfloat const y = sin(-M_PI_2 + M_PI * r * R);
				GLfloat const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

				*t++ = s * S;
				*t++ = r * R;

				*v++ = x * radius;
				*v++ = y * radius;
				*v++ = z * radius;

				*n++ = x;
				*n++ = y;
				*n++ = z;
			}
		}

		indices.resize(rings * sectors * 4);
		std::vector<GLushort>::iterator i = indices.begin();
		for (r = 0; r < rings - 1; r++) {
			for (s = 0; s < sectors - 1; s++) {
				*i++ = r * sectors + s;
				*i++ = r * sectors + (s + 1);
				*i++ = (r + 1) * sectors + (s + 1);
				*i++ = (r + 1) * sectors + s;
			}
		}
	}

	void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat angle) {
		Mesh::draw(x, y, z, angle);
	}
};

class Cylinder : public Mesh {
public:
	Cylinder(GLfloat radius, GLfloat height, GLfloat slices, GLfloat stacks) {
		GLfloat const SLICE = 1. / (GLfloat)(slices - 1);
		GLfloat const STACK = height / (GLfloat)(stacks - 1);
		GLuint slice, stack;

		vertices.resize(slices * (stacks + 2) * 3);
		normals.resize(slices * (stacks + 2) * 3);
		texcoords.resize(slices * (stacks + 2) * 2);

		auto v = vertices.begin();
		auto n = normals.begin();
		auto t = texcoords.begin();

		for (stack = 1; stack <= stacks; ++stack) {
			for (slice = 0; slice < slices; ++slice) {
				GLfloat const x = sin(2 * M_PI * slice * SLICE);
				GLfloat const y = height / 2 - stack * STACK;
				GLfloat const z = cos(2 * M_PI * slice * SLICE);

				*t++ = slice * SLICE;
				*t++ = stack * STACK;

				*v++ = x * radius;
				*v++ = y;
				*v++ = z * radius;

				*n++ = x;
				*n++ = y;
				*n++ = z;
			}
		}

		for (int i = 0; i < 2; ++i) {
			for (slice = 0; slice < slices; ++slice) {
				GLfloat const x = sin(2 * M_PI * slice * SLICE);
				GLfloat const y = (i == 0) ? height / 2 : -height / 2;
				GLfloat const z = cos(2 * M_PI * slice * SLICE);

				*t++ = 0.5 + 0.5 * cos(2 * M_PI * slice * SLICE);
				*t++ = 0.5 - 0.5 * sin(2 * M_PI * slice * SLICE);

				*v++ = x * radius;
				*v++ = y;
				*v++ = z * radius;

				*n++ = x;
				*n++ = y;
				*n++ = z;

			}
		}

		indices.resize(slices * (stacks + 2) * 4);
		std::vector<GLushort>::iterator it
			= indices.begin();
		for (stack = 0; stack < stacks + 1; stack++) {
			for (slice = 0; slice < slices - 1; slice++) {
				*it++ = stack * slices + slice;
				*it++ = stack * slices + (slice + 1);
				*it++ = (stack + 1) * slices + (slice + 1);
				*it++ = (stack + 1) * slices + slice;
			}
		}
	}

	void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat angle) {
		Mesh::draw(x, y, z, angle);
	}
};
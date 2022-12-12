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

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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

				GLfloat const x = (i == 0
					|| (i == 1 && (j == 0 || j == 3))
					|| (i == 2 && (j == 0 || j == 1))
					|| (i == 4 && (j == 2 || j == 3))
					|| (i == 5 && (j == 1 || j == 2))) ? 1.0f : -1.0f;

				GLfloat const y = (i == 1
					|| (i == 0 && (j == 0 || j == 1))
					|| (i == 2 && (j == 0 || j == 3))
					|| (i == 3 && (j == 1 || j == 2))
					|| (i == 5 && (j == 2 || j == 3))) ? 1.0f : -1.0f;

				GLfloat const z = (i == 2
					|| (i == 0 && (j == 0 || j == 3))
					|| (i == 1 && (j == 0 || j == 1))
					|| (i == 3 && (j == 2 || j == 3))
					|| (i == 4 && (j == 1 || j == 2))) ? 1.0f : -1.0f;

				*t++ = (j == 0 || j == 3) ? 1.0f : 0.0f;
				*t++ = (j == 0 || j == 1) ? 1.0f : 0.0f;

				*v++ = x * size;
				*v++ = y * size;
				*v++ = z * size;

				*n++ = x;
				*n++ = y;
				*n++ = z;
			}
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
	Sphere(GLfloat radius, GLuint stacks, GLuint slices) {
		GLfloat const STACK = 1. / (GLfloat)(stacks - 1);
		GLfloat const SLICE = 1. / (GLfloat)(slices - 1);
		GLuint stack, slice;

		vertices.resize(stacks * slices * 3);
		normals.resize(stacks * slices * 3);
		texcoords.resize(stacks * slices * 2);

		auto v = vertices.begin();
		auto n = normals.begin();
		auto t = texcoords.begin();

		for (stack = 0; stack < stacks; ++stack) {
			for (slice = 0; slice < slices; ++slice) {
				// TODO: maybe need to be changed later
				GLfloat const x = cos(2 * M_PI * slice * SLICE) * sin(M_PI * stack * STACK);
				GLfloat const y = sin(-M_PI_2 + M_PI * stack * STACK);
				GLfloat const z = sin(2 * M_PI * slice * SLICE) * sin(M_PI * stack * STACK);

				*t++ = slice * SLICE;
				*t++ = stack * STACK;

				*v++ = x * radius;
				*v++ = y * radius;
				*v++ = z * radius;

				*n++ = x;
				*n++ = y;
				*n++ = z;
			}
		}

		indices.resize(stacks * slices * 4);
		std::vector<GLushort>::iterator it = indices.begin();
		for (stack = 0; stack < stacks - 1; stack++) {
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

class Cylinder : public Mesh {
private:
	vector<GLfloat> baseVertices;
	vector<GLfloat> baseNormals;
	vector<GLfloat> baseTexcoords;
	vector<GLushort> baseIndices;
public:
	Cylinder(GLfloat radius, GLfloat height, GLfloat stacks, GLfloat slices) {
		GLfloat const STACK = 1. / (GLfloat)(stacks - 1);
		GLfloat const SLICE = 1. / (GLfloat)(slices - 1);

		GLuint slice, stack;

		vertices.resize(slices * stacks * 3);
		normals.resize(slices * stacks * 3);
		texcoords.resize(slices * stacks * 2);

		auto v = vertices.begin();
		auto n = normals.begin();
		auto t = texcoords.begin();

		for (stack = 0; stack < stacks; ++stack) {
			for (slice = 0; slice < slices; ++slice) {
				GLfloat const x = cos(2 * M_PI * slice * SLICE);
				GLfloat const y = -1.0/2 + stack * STACK;
				GLfloat const z = sin(2 * M_PI * slice * SLICE);

				*t++ = slice * SLICE;
				*t++ = stack * STACK;

				*v++ = x * radius;
				*v++ = y * height;
				*v++ = z * radius;

				*n++ = x;
				*n++ = y;
				*n++ = z;
			}
		}

		indices.resize(slices * stacks * 4);
		std::vector<GLushort>::iterator it = indices.begin();
		for (stack = 0; stack < stacks - 1; stack++) {
			for (slice = 0; slice < slices - 1; slice++) {
				*it++ = stack * slices + slice;
				*it++ = stack * slices + (slice + 1);
				*it++ = (stack + 1) * slices + (slice + 1);
				*it++ = (stack + 1) * slices + slice;
			}
		}

		{
			// Draw base and top surface
			for (int i = 0; i < 2; ++i) {
				GLfloat h = height / 2.0f - i * height;
				GLfloat ny = -1 + i * 2;

				baseVertices.push_back(0);
				baseVertices.push_back(h);
				baseVertices.push_back(0);
	
				baseNormals.push_back(0);
				baseNormals.push_back(ny);
				baseNormals.push_back(0);

				baseTexcoords.push_back(0.5f);
				baseTexcoords.push_back(0.5f);

				for (slice = 0; slice < slices; ++slice) {
					GLfloat const x = cos(2 * M_PI * slice * SLICE);
					GLfloat const z = sin(2 * M_PI * slice * SLICE);

					baseVertices.push_back(x * radius);
					baseVertices.push_back(h);
					baseVertices.push_back(z * radius);

					baseNormals.push_back(0);
					baseNormals.push_back(ny);
					baseNormals.push_back(0);

					baseTexcoords.push_back(-x * 0.5f + 0.5f);
					baseTexcoords.push_back(-z * 0.5f + 0.5f);
				}
			}

			int baseCenterIndex = 0;
			int topCenterIndex = baseCenterIndex + slices + 1;

			for (int i = 0, k = baseCenterIndex + 1; i < slices; ++i, ++k)
			{
				if (i < slices - 1)
				{
					baseIndices.push_back(baseCenterIndex);
					baseIndices.push_back(k + 1);
					baseIndices.push_back(k);
				}
				else // last triangle
				{
					baseIndices.push_back(baseCenterIndex);
					baseIndices.push_back(k);
					baseIndices.push_back(baseCenterIndex + 1);
				}
			}

			for (int i = 0, k = topCenterIndex + 1; i < slices; ++i, ++k)
			{
				if (i < slices - 1)
				{
					baseIndices.push_back(topCenterIndex);
					baseIndices.push_back(k);
					baseIndices.push_back(k + 1);
				}
				else // last triangle
				{
					baseIndices.push_back(topCenterIndex);
					baseIndices.push_back(k);
					baseIndices.push_back(topCenterIndex + 1);
				}
			}
		}
	}

	void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat angle) {
		// Draw side surface
		Mesh::draw(x, y, z, angle);

		// Draw top and base surface
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(angle, 1.0f, 1.0f, 1.0f);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, &baseVertices[0]);
		glNormalPointer(GL_FLOAT, 0, &baseNormals[0]);
		glTexCoordPointer(2, GL_FLOAT, 0, &baseTexcoords[0]);

		glDrawElements(GL_TRIANGLES, baseIndices.size(), GL_UNSIGNED_SHORT, &baseIndices[0]);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glPopMatrix();
	}
};
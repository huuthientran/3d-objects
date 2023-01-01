#pragma once
#include "SOIL.h"
#include <gl/glut.h>
#include <vector>
#include <iostream>
#include <cmath>
#include "Texture.h"

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

		indices.resize(6 * 4);
		std::vector<GLushort>::iterator it = indices.begin();
		for (GLuint i = 0; i < 6; i++) {
			for (GLuint j = 0; j < 4; j++) {
				*it++ = i * 4 + j;
			}
		}
	}

	void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat angle, bool wired) {		
		if (wired) {
			Mesh::draw(x, y, z, angle);
			return;
		}

		glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(angle, 1.0f, 1.0f, 1.0f);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
		glNormalPointer(GL_FLOAT, 0, &normals[0]);
		glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);

		for (int i = 0; i < 6; ++i) {
			glBindTexture(GL_TEXTURE_2D, textureIDList[i + 7]);
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, &indices[i * 4]);
		}

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glPopMatrix();
	}
};

class Disk : public Mesh {
private:
	vector<GLfloat> center;
public:
	Disk() {

	}

	Disk(GLfloat radius, GLuint slices, vector<GLfloat> center, vector<GLfloat> normals) {
		GLfloat const SLICE = 1. / (GLfloat)(slices - 1);

		this->center.clear();
		this->center.insert(this->center.end(), center.begin(), center.end());
		this->vertices.insert(this->vertices.end(), this->center.begin(), this->center.end());

		this->normals.clear();
		this->normals.insert(this->normals.end(), normals.begin(), normals.end());

		texcoords.push_back(0.5f);
		texcoords.push_back(0.5f);

		for (GLuint slice = 0; slice < slices; ++slice) {
			GLfloat const x = cos(2 * M_PI * slice * SLICE);
			GLfloat const z = sin(2 * M_PI * slice * SLICE);

			vertices.push_back(x * radius + center[0]);
			vertices.push_back(center[1]);
			vertices.push_back(z * radius + center[2]);

			this->normals.push_back(normals[0]);
			this->normals.push_back(normals[1]);
			this->normals.push_back(normals[2]);

			texcoords.push_back(-x * 0.5f + 0.5f);
			texcoords.push_back(-z * 0.5f + 0.5f);
		}

		GLuint baseCenterIndex = 0;
		for (GLuint i = 0, k = baseCenterIndex + 1; i < slices; ++i, ++k)
		{
			if (i < slices - 1)
			{
				indices.push_back(baseCenterIndex);
				indices.push_back(k + 1);
				indices.push_back(k);
			}
			else // last triangle
			{
				indices.push_back(baseCenterIndex);
				indices.push_back(k);
				indices.push_back(baseCenterIndex + 1);
			}
		}
	}

	void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat angle) {

		glPushMatrix();

		glTranslatef(x, y, z);
		glRotatef(angle, 1.0f, 1.0f, 1.0f);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
		glNormalPointer(GL_FLOAT, 0, &normals[0]);
		glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glPopMatrix();
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
	Disk top, base;
public:
	Cylinder(GLfloat radius, GLfloat height, GLuint stacks, GLuint slices) {
		top = Disk(radius, slices, { 0, -height / 2.0f, 0 }, { 0, -1, 0 });
		base = Disk(radius, slices, { 0, height / 2.0f, 0 }, { 0, 1, 0 });

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
	}

	void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat angle, bool wired) {
		// Draw side surface
		if (!wired) glBindTexture(GL_TEXTURE_2D, textureIDList[12]);
		Mesh::draw(x, y, z, angle);
		if (!wired) glBindTexture(GL_TEXTURE_2D, textureIDList[18]);
		top.draw(x, y, z, angle);
		if (!wired) glBindTexture(GL_TEXTURE_2D, textureIDList[24]);
		base.draw(x, y, z, angle);
	}
};

class Cone : public Mesh {
private:
	Disk base;
public:
	Cone(GLfloat radius, GLfloat height, GLuint slices) {
		base = Disk(radius, slices, { 0, -height / 2.0f, 0 }, { 0, -1, 0 });

		GLfloat const SLICE = 1. / (GLfloat)(slices - 1);

		vertices.push_back(0);
		vertices.push_back(height / 2.0f);
		vertices.push_back(0);

		normals.push_back(0);
		normals.push_back(1);
		normals.push_back(0);

		texcoords.push_back(0.5f);
		texcoords.push_back(0.5f);

		for (GLuint slice = 0; slice < slices; ++slice) {
			GLfloat const x = cos(2 * M_PI * slice * SLICE);
			GLfloat const z = sin(2 * M_PI * slice * SLICE);

			vertices.push_back(x * radius);
			vertices.push_back(-height / 2.0f);
			vertices.push_back(z * radius);

			normals.push_back(x);
			normals.push_back(0);
			normals.push_back(z);

			texcoords.push_back(-x * 0.5f + 0.5f);
			texcoords.push_back(-z * 0.5f + 0.5f);
		}

		GLuint baseCenterIndex = 0;
		for (GLuint i = 0, k = baseCenterIndex + 1; i < slices; ++i, ++k)
		{
			if (i < slices - 1)
			{
				indices.push_back(baseCenterIndex);
				indices.push_back(k + 1);
				indices.push_back(k);
			}
			else // last triangle
			{
				indices.push_back(baseCenterIndex);
				indices.push_back(k);
				indices.push_back(baseCenterIndex + 1);
			}
		}
	}

	void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat angle, bool wired) {
		glPushMatrix();

		glTranslatef(x, y, z);
		glRotatef(angle, 1.0f, 1.0f, 1.0f);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
		glNormalPointer(GL_FLOAT, 0, &normals[0]);
		glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);

		if (!wired) glBindTexture(GL_TEXTURE_2D, textureIDList[29]);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glPopMatrix();

		if (!wired) glBindTexture(GL_TEXTURE_2D, textureIDList[22]);
		base.draw(x, y, z, angle);
	}
};

class Torus : public Mesh {
public:
	Torus(GLfloat innerRadius, GLfloat outerRadius, GLuint nsides, GLuint rings) {
		GLfloat const SIDE = 1. / (GLfloat)(nsides - 1);
		GLfloat const RING = 1. / (GLfloat)(rings - 1);

		GLuint ring, side;

		for (side = 0; side < nsides; ++side) {
			for (ring = 0; ring < rings; ++ring) {
				GLfloat const x = (outerRadius + innerRadius * cos(2*  M_PI * side * SIDE)) * cos(2 * M_PI * ring * RING);
				GLfloat const y = innerRadius * sin(2 * M_PI * side * SIDE);
				GLfloat const z = (outerRadius + innerRadius * cos(2 * M_PI * side * SIDE)) * sin(2 * M_PI * ring * RING);

				texcoords.push_back(ring * RING);
				texcoords.push_back(side * SIDE);

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);

				normals.push_back(x);
				normals.push_back(y);
				normals.push_back(z);
			}
		}

		for (side = 0; side < nsides - 1; side++) {
			for (ring = 0; ring < rings - 1; ring++) {
				indices.push_back(side * rings + ring);
				indices.push_back(side * rings + (ring + 1));
				indices.push_back((side + 1) * rings + (ring + 1));
				indices.push_back((side + 1) * rings + ring);
			}
		}
	}

	void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat angle, bool wired) {
		if (!wired) glBindTexture(GL_TEXTURE_2D, textureIDList[16]);
		Mesh::draw(x, y, z, angle);
	}
};

class Hyperboloid : public Mesh {
private:
	Disk top;
	Disk base;
public:
	/**
		@brief Parametric equation of a hyperboloid used here is: (x/a)^2 - y^2 + (z/a)^2 = 1
	*/
	Hyperboloid(GLfloat radius, GLfloat height, GLfloat slices, GLfloat stacks) {
		top = Disk(radius, slices, { 0, height / 2.0f, 0 }, { 0, 1, 0 });
		base = Disk(radius, slices, { 0, -height / 2.0f, 0 }, { 0, -1, 0 });

		GLfloat const STACK = 1. / (GLfloat)(stacks - 1);
		GLfloat const SLICE = 1. / (GLfloat)(slices - 1);
		 
		GLuint slice, stack;

		for (stack = 0; stack < stacks; ++stack) {
			for (slice = 0; slice < slices; ++slice) {
				GLfloat u = 2 * M_PI * slice * SLICE;
				GLfloat v = -height / 2.0f + height * stack * STACK;
				GLfloat a = radius / sqrt(height * height / 4 + 1);
				
				GLfloat const x = cos(u) - v * sin(u);
				GLfloat const y = v;
				GLfloat const z = sin(u) + v * cos(u);

				texcoords.push_back(slice * SLICE);
				texcoords.push_back(stack * STACK);

				vertices.push_back(x * a);
				vertices.push_back(y);
				vertices.push_back(z * a);

				normals.push_back(x);
				normals.push_back(y);
				normals.push_back(z);
			}
		}

		for (stack = 0; stack < stacks - 1; stack++) {
			for (slice = 0; slice < slices - 1; slice++) {
				indices.push_back(stack * slices + slice);
				indices.push_back(stack * slices + (slice + 1));
				indices.push_back((stack + 1) * slices + (slice + 1));
				indices.push_back((stack + 1) * slices + slice);
			}
		}
	}

	void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat angle, bool wired) {
		if (!wired) glBindTexture(GL_TEXTURE_2D, textureIDList[27]);
		Mesh::draw(x, y, z, angle);
		if (!wired) glBindTexture(GL_TEXTURE_2D, textureIDList[6]);
		top.draw(x, y, z, angle);
		if (!wired) glBindTexture(GL_TEXTURE_2D, textureIDList[3]);
		base.draw(x, y, z, angle);
	}
};

class Paraboloid : public Mesh {
public:
	Paraboloid(GLfloat radius, GLfloat height, GLfloat slices, GLfloat stacks) {
		GLfloat const STACK = 1. / (GLfloat)(stacks - 1);
		GLfloat const SLICE = 1. / (GLfloat)(slices - 1);

		GLuint slice, stack;

		for (stack = 0; stack < stacks; ++stack) {
			for (slice = 0; slice < slices; ++slice) {
				GLfloat u = 2 * M_PI * slice * SLICE;
				GLfloat v = height * stack * STACK;
				GLfloat r = sqrt(v / height) * radius;

				GLfloat const x = r * cos(u);
				GLfloat const y = v;
				GLfloat const z = r * sin(u);

				texcoords.push_back(slice * SLICE);
				texcoords.push_back(stack * STACK);

				vertices.push_back(x);
				vertices.push_back(-y + height / 2.0f);
				vertices.push_back(z);

				normals.push_back(x);
				normals.push_back(y);
				normals.push_back(z);
			}
		}

		for (stack = 0; stack < stacks - 1; stack++) {
			for (slice = 0; slice < slices - 1; slice++) {
				indices.push_back(stack * slices + slice);
				indices.push_back(stack * slices + (slice + 1));
				indices.push_back((stack + 1) * slices + (slice + 1));
				indices.push_back((stack + 1) * slices + slice);
			}
		}
	}

	void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat angle, bool wired) {
		if (!wired) glBindTexture(GL_TEXTURE_2D, textureIDList[1]);
		Mesh::draw(x, y, z, angle);
	}
};
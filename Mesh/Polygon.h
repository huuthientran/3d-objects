#pragma once
#include "Vertex.h"
#include <vector>

class MyPolygon {
private:
	std::vector<Vertex> sharedVertices;

public:
	MyPolygon(std::vector<Vertex> vertices) {
		for (size_t i = 0; i < vertices.size(); ++i) {
			this->sharedVertices.push_back(vertices[i]);
		}
	}

	std::vector<Vertex> getVertices() {
		return sharedVertices;
	}
};
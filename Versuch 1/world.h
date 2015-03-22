#ifndef __WORLD_H__
#define __WORLD_H__

#include "voxel.h"

#include <exception>
#include <stdexcept>

#include <glfw/glfw3.h>

class World {
	int _width, _height;
	Voxel* voxels;

public:
	World() {
		_width = 0;
		_height = 0;
		voxels = 0;
	}

	World(int width, int height, const char* data = 0) {
		if (width < 0) throw new std::invalid_argument("invalid width");
		if (height < 0) throw new std::invalid_argument("invalid height");

		_width = width;
		_height = height;
		voxels = new Voxel[width * height];

		if (!voxels)
			throw new std::bad_alloc();

		if (data)
			load(data);
	}

	inline Voxel* get(int x, int y) const {
		if (x < 0 || _width < x) throw new std::out_of_range("invalid x range");
		if (y < 0 || _height < y) throw new std::out_of_range("invalid y range");
		return &voxels[x + y * _width];
	}

	inline Voxel* get(double x, double y) const {
		return get((int)round(x), (int)round(y));
	}

	inline int width() const { return _width; }
	inline int height() const { return _height; }

	void load(const char* data) {
		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				get(x, y)->isSolid(data[x + (_height - y - 1) * _width] == 1);
			}
		}
	}

	void render() {
		glBegin(GL_QUADS);
		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				if (!get(x, y)->isSolid())
					continue;

				glColor3f(1.0, 1.0, 1.0);
				glVertex2d(x - .5, y + .5);
				glVertex2d(x - .5, y - .5);
				glVertex2d(x + .5, y - .5);
				glVertex2d(x + .5, y + .5);
			}
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
			glColor3f(1.0, 1.0, 1.0);
			glVertex2d(0 - 0.5, _height - 0.5);
			glVertex2d(0 - 0.5, 0 - 0.5);
			glVertex2d(_width - 0.5, 0 - 0.5);
			glVertex2d(_width - 0.5, _height - 0.5);
			glVertex2d(0 - 0.5, _height - 0.5);
		glEnd();
	}
};

#endif
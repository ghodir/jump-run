#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

#include "world.h"

char level[] = {
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
}; 

int width = 14;
int height = 11;
double viewport_width = 26.66;
double viewport_height = 20.00;
double camera_x = 5.0;
double camera_y = 5.0;

float x = 2.f;
float y = 6.f;

float vx = 0.0f;
float vy = 0.0f;

float ax = 0.0f;
float ay = 0.0f;

float g = 30.0f;

int jumping = 0;
float friction = 0.9f;
bool space = false;
bool mousebuttons[3] = {false, false, false};

float lx = 0.0;

bool collide(double x1, double y1, double x2, double y2) {
	if (x1 - 0.5 > x2 + 0.5) return false;
	else if (x2 - 0.5 > x1 + 0.5) return false;

	if (y1 - 0.5 > y2 + 0.5) return false;
	else if (y2 - 0.5 > y1 + 0.5) return false;

	return true;
}



int main(void) {
	GLFWwindow* window;
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(800, 600, "glfw", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-viewport_width * 0.5, viewport_width * 0.5, -viewport_height * 0.5, viewport_height * 0.5, 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);

	World world(100, 32);
	double t = glfwGetTime(), dt;
	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);

		{
			double tmp = glfwGetTime();
			dt = tmp - t;
			t = tmp;
		}

		ax = 0.0f;
		ay = 0.0f;

		// input
		if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			vx = -4.f;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			vx = 4.f;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE)) {
			if (!space) {
				if (jumping == 0) {
					vy = 10.0f;
					ay = 10.f;
					y += 0.1f;
					jumping = 1;
				}
				else if (jumping == 1) {
					vy = 10.f;
					ay = 10.f;
					jumping = 2;
				}
			}
			space = true;
		} else {
			space = false;
		}

		double cursor_x, cursor_y;
		int window_width, window_height;
		glfwGetWindowSize(window, &window_width, &window_height);
		glfwGetCursorPos(window, &cursor_x, &cursor_y);

		cursor_x -=  (int)(window_width * 0.5);
		cursor_y  =  (window_height - cursor_y) - (int)(window_height * 0.5);

		cursor_x *= viewport_width / window_width;
		cursor_y *= viewport_height / window_height;

		cursor_x += camera_x;
		cursor_y += camera_y;

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
			if (!mousebuttons[GLFW_MOUSE_BUTTON_1]) {
				if (cursor_x < 0.0 || world.width() < cursor_x ||
					cursor_y < 0.0 || world.height() < cursor_y)
					;
				else if (world.get(cursor_x, cursor_y)->isSolid())
					world.get(cursor_x, cursor_y)->isSolid(false);
				else
					world.get(cursor_x, cursor_y)->isSolid(true);
			}
		}
		else{
			mousebuttons[GLFW_MOUSE_BUTTON_1] = false;
		}

		// update

		ax += 0.f;
		ay += -g;

		if (floor(x + vx*dt) < 0){
			x = 0.0f;
			vx = 0.0f;
		}
		else if (ceil(x + vx*dt) >= world.width()) {
			x = (float)world.width() - 1.0f;
			vx = 0.0f;
		}

		if (false && floor(y + vy*dt) <= 0) {
			y = 0.0f;
			vy = 0.0f;
		}
		else if (ceil(y + vy*dt) >= world.height()) {
			y = (float)world.height() - 1.0f;
			vx = 0.0f;
		}

		{
			bool dont_check_y = false;
			int tx = (int)floor(x);
			int ty = (int)floor(y);

			if (y + vy*dt - 0.0001 < 0.0f || 
				(world.get((int)floor(x + vx*dt), (int)floor(y + vy*dt - 0.0001))->isSolid() ||
				world.get((int)ceil(x + vx*dt), (int)floor(y + vy*dt - 0.0001))->isSolid())) {
				y = (float)ceil(y+vy*dt-0.0001);
				vy = 0;
				ay = 0;
				jumping = 0;
			}
			else if (world.get((int)floor(x + vx*dt),(int)ceil(y + vy*dt))->isSolid() ||
				world.get((int)ceil(x + vx*dt),(int)ceil(y + vy*dt))->isSolid()) {
				vy = 0;
			}

			if (world.get((int)floor(x + vx*dt), (int)floor(y + vy*dt))->isSolid() ||
				world.get((int)ceil(x + vx*dt), (int)floor(y + vy*dt))->isSolid() ||
				world.get((int)floor(x + vx*dt), (int)ceil(y + vy*dt))->isSolid() ||
				world.get((int)ceil(x + vx*dt), (int)ceil(y + vy*dt))->isSolid()) {
				vx = 0;
				ax = 0;
			}
		}

		vx += ax * (float)dt;
		vy += ay * (float)dt;

		if (!jumping) {
			vx *= friction;
		}

		x += vx * (float)dt;
		y += vy * (float)dt;
		
		// render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glTranslated(-camera_x, -camera_y, -3.0f);
		world.render();

		glBegin(GL_QUADS);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2d(x - .5, y + .5);
		glVertex2d(x - .5, y - .5);
		glVertex2d(x + .5, y - .5);
		glVertex2d(x + .5, y + .5);

		glEnd();

		glBegin(GL_LINE_STRIP);
			double tx = round(cursor_x);
			double ty = round(cursor_y);
			glColor3f(1.0, 1.0, 0.0);
			glVertex2d(tx - .5, ty + .5);
			glVertex2d(tx - .5, ty - .5);
			glVertex2d(tx + .5, ty - .5);
			glVertex2d(tx + .5, ty + .5);
			glVertex2d(tx - .5, ty + .5);
		glEnd();

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
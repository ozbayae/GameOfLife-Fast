#include "Life3d.h"
#include "Life.h"

#include <ctime>

#include <GL/glut.h>
#include<chrono>

using namespace life3;

#ifndef GLSCENE_H
#define GLSCENE_H
struct Timer
{
	Timer() { reset(); }
	float elaspsed() const {
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(t2 - start);
		return (float)time_span.count();
	}
	void reset() { start = std::chrono::high_resolution_clock::now(); }
	std::chrono::high_resolution_clock::time_point start;
};
enum Scene
{
	scene1,
	scene2
};

void GLScene(int argc, char*argv[]);
void GLScene(int, int, int argc, char*argv[]);
void Cleanup();

void newLife();
void newlife3d();

void DisplayGL();
void KeyboardGL(unsigned char c, int x, int y);
void ReshapeGL(int w, int h);

void render();
void render3d();
#endif

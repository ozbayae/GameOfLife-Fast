#pragma once
#include <iostream>

using std::malloc;
using std::cout;
using std::endl;

#ifndef _WORLD_H
#define _WORLD_H

class World
{
protected:
	short* grid;
	short* new_grid;
	unsigned int width;
	unsigned int height;

	bool swapped = false;

	//int getNewLife(int x, int y);
	void setNewLife(int x, int y, int val);
	void swapGrids();

public:
	World(int, int);
	virtual ~World();

	int getNeighbors(int, int, int);

	int getLifeform(int x, int y);
	void setLife(int x, int y, int val);
	virtual void update(){ }

	void print();
};

#endif

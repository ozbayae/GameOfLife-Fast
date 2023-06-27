#include "World.h"

#define BITS_IN_BOOL 8

World::World(int w, int h)
{
	width = w;
	height = h;
	grid_width = width + 2;
	grid_height = height + 2;
	grid = (bool *)malloc(grid_width * grid_height * sizeof(bool));
	new_grid = (bool *)malloc(grid_width * grid_height * sizeof(bool));

	for (unsigned int i = 0; i < grid_width * grid_height; i++)
	{
		grid[i] = 0;
		new_grid[i] = 0;
	}
}

World::~World(){ 
	free(grid); free(new_grid); 
}

void World::setNewLife(int x, int y, int val)
{ 
	new_grid[x* grid_width + y] = val; 
}

void World::swapGrids()
{
	std::swap(grid, new_grid);
	swapped = !swapped;
}

int World::getNeighbors(int x, int y, int val)
{
	int count = 0;

	count += grid[(x - 1) * grid_width + (y)];
	count += grid[(x) * grid_width + (y - 1)];
	count += grid[(x - 1) * grid_width + (y - 1)];
	count += grid[(x + 1) * grid_width + (y)];

	count += grid[(x) * grid_width + (y + 1)];
	count += grid[(x + 1) * grid_width + (y + 1)];
	count += grid[(x + 1) * grid_width + (y - 1)];
	count += grid[(x - 1) * grid_width + (y + 1)];
	
	return count;
}

int World::getLifeform(int x, int y)
{ 
		return grid[x*grid_width + y];
}

void World::setLife(int x, int y, int val)
{ 
	grid[x*grid_width + y] = val; 
}

void World::print()
{
	for (unsigned int i = 1; i <= height; i++)
	{
		cout << endl;
		for (unsigned int j = 1; j <= width; j++)
		{
			cout << getLifeform(j, i) << " ";
		}
	}
	cout << endl;

}

#include "World.h"

World::World(int w, int h)
{
	width = w;
	height = h;
	grid_width = (width + 2) / BITS_IN_BOOL;
	grid_height = (height + 2);
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
	//new_grid[x* grid_width + y] = val; 
	if(val)
		new_grid[x * grid_width + y / BITS_IN_BOOL] |= (1UL << (y % BITS_IN_BOOL));
	else
		new_grid[x * grid_width + y / BITS_IN_BOOL] &= ~(1UL << (y % BITS_IN_BOOL));
}

void World::swapGrids()
{
	std::swap(grid, new_grid);
	swapped = !swapped;
}

int World::getNeighbors(int x, int y, int val)
{
	int count = 0;

	//count += grid[(x - 1) * grid_width + (y)];
	//count += grid[(x) * grid_width + (y - 1)];
	//count += grid[(x - 1) * grid_width + (y - 1)];
	//count += grid[(x + 1) * grid_width + (y)];

	//count += grid[(x) * grid_width + (y + 1)];
	//count += grid[(x + 1) * grid_width + (y + 1)];
	//count += grid[(x + 1) * grid_width + (y - 1)];
	//count += grid[(x - 1) * grid_width + (y + 1)];
	//
	//return count;
	//return 0;
	count += getLifeform(x - 1, y);
	count += getLifeform(x, y - 1);
	count += getLifeform(x - 1, y - 1);
	count += getLifeform(x + 1, y);

	count += getLifeform(x, y + 1);
	count += getLifeform(x + 1, y + 1);
	count += getLifeform(x + 1, y - 1);
	count += getLifeform(x - 1, y + 1);

	return count;
}

int World::getLifeform(int x, int y)
{ 
		//return grid[x*grid_width + y];
	int byte = grid[x * grid_width + y / BITS_IN_BOOL];
	return (byte >> (y % BITS_IN_BOOL)) & 1U;
	//return grid[x * grid_width + y / BITS_IN_BOOL] & (1 << (y % BITS_IN_BOOL));
}

void World::setLife(int x, int y, int val)
{ 
	//grid[x*grid_width + y] = val; 
	//grid[x * grid_width + y] = val;
	if (val)
		grid[x * grid_width + y / BITS_IN_BOOL] |= 1UL << (y % BITS_IN_BOOL);
	else
		grid[x * grid_width + y / BITS_IN_BOOL] &= ~(1UL << (y % BITS_IN_BOOL));
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

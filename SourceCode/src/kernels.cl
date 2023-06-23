__kernel void helloworld(__global char* in, __global char* out, __global float* numbers, float x)
{
	int num = get_global_id(0);
	out[num] = in[num] + 1;
	numbers[num] = numbers[num] * x;
}

kernel void update(global short* grid, global short* new_grid, int width) {
    int x = get_global_id(0) + 1;
    int y = get_global_id(1) + 1;

    //int x = index / width;
    //int y = index % height;
    //int m = getNeighbours(x, y, grid, new_grid, width, height);

    int count = 0;

    count += grid[(x - 1) * (width + 2) + (y)];
    count += grid[(x) * (width + 2) + (y - 1)];
    count += grid[(x - 1) * (width + 2) + (y - 1)];
    count += grid[(x + 1) * (width + 2) + (y)];

    count += grid[(x) * (width + 2) + (y + 1)];
    count += grid[(x + 1) * (width + 2) + (y + 1)];
    count += grid[(x + 1) * (width + 2) + (y - 1)];
    count += grid[(x - 1) * (width + 2) + (y + 1)];

    int index = x * (width + 2) + y;

    if (count == 3)
    {
       new_grid[index] = 1;
    }
    else if (count == 2)
    {
       new_grid[index] = grid[index];
    }
    else
    {
       new_grid[index] = 0;
    }
}
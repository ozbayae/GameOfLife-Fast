__kernel void helloworld(__global char* in, __global char* out, __global float* numbers, float x)
{
	int num = get_global_id(0);
	out[num] = in[num] + 1;
	numbers[num] = numbers[num] * x;
}


bool getAlive(int x, int y, global bool* grid, int width)
{
    int index = x * width + y / 8;
    int bit = y % 8;
    unsigned char mask = 1 << bit;
    return (grid[index] & mask) != 0;
}

kernel void update(global bool* grid, global bool* new_grid, int width) {
    int x = get_global_id(0) + 1;
    int y = get_global_id(1) + 1;

    int count = 0;

    ////count += grid[(x - 1) * width + (y)];
    ////count += grid[(x) * width + (y - 1)];
    ////count += grid[(x - 1) * width + (y - 1)];
    ////count += grid[(x + 1) * width + (y)];

    ////count += grid[(x) * width + (y + 1)];
    ////count += grid[(x + 1) * width + (y + 1)];
    ////count += grid[(x + 1) * width + (y - 1)];
    ////count += grid[(x - 1) * width + (y + 1)];

    count += getAlive(x - 1, y, grid, width);
    count += getAlive(x, y - 1, grid, width);
    count += getAlive(x - 1, y - 1, grid, width);
    count += getAlive(x + 1, y, grid, width);

    count += getAlive(x, y + 1, grid, width);
    count += getAlive(x + 1, y + 1, grid, width);
    count += getAlive(x + 1, y - 1, grid, width);
    count += getAlive(x - 1, y + 1, grid, width);

    int index = x * width + y / 8;
    ////set the position of the bool value within the byte array
    int bitPosition = y % 8;

    if (count == 3 || (count == 2 && ((grid[index] >> bitPosition) & 1)))
    {
        ////set the position of the bool value within the byte array
        bool mask = 1 << bitPosition;
        new_grid[index] |= mask;
    }
    else
    {
        bool mask = ~(1 << bitPosition);
        new_grid[index] &= mask;
    }

    //new_grid[index] = grid[index];
}

kernel void update3d(global short* grid, global short* new_grid, int width, int height) {
    int x = get_global_id(0) + 1;
    int y = get_global_id(1) + 1;
    int z = get_global_id(2) + 1;

    int count = 0;

    count += grid[(y+1) + x*(width + 2) + z*(width + 2)*(height + 2)];
    count += grid[(y + 1) + x*(width + 2) + (z+1)*(width + 2)*(height + 2)];
    count += grid[(y + 1) + x*(width + 2) + (z-1)*(width + 2)*(height + 2)];

    count +=  grid[(y + 1) + (x + 1)*(width + 2) + (z + 1)*(width + 2)*(height + 2)];
    count += grid[(y + 1) + (x + 1)*(width + 2) + (z)*(width + 2)*(height + 2)];
    count += grid[(y + 1) + (x + 1)*(width + 2) + (z - 1)*(width + 2)*(height + 2)];
    
    count += grid[(y + 1) + (x - 1)*(width + 2) + z*(width + 2)*(height + 2)];
    count += grid[(y + 1) + (x - 1)*(width + 2) + (z + 1)*(width + 2)*(height + 2)];
    count += grid[(y + 1) + (x - 1)*(width + 2) + (z - 1)*(width + 2)*(height + 2)];
    //
    count += grid[(y - 1) + x*(width + 2) + z*(width + 2)*(height + 2)];
    count += grid[(y - 1) + x*(width + 2) + (z + 1)*(width + 2)*(height + 2)];
    count += grid[(y - 1) + x*(width + 2) + (z - 1)*(width + 2)*(height + 2)];

    count += grid[(y - 1) + (x + 1)*(width + 2) + (z + 1)*(width + 2)*(height + 2)];
    count += grid[(y - 1) + (x + 1)*(width + 2) + (z)*(width + 2)*(height + 2)];
    count += grid[(y - 1) + (x + 1)*(width + 2) + (z - 1)*(width + 2)*(height + 2)];

    count += grid[(y - 1) + (x - 1)*(width + 2) + z*(width + 2)*(height + 2)];
    count += grid[(y - 1) + (x - 1)*(width + 2) + (z + 1)*(width + 2)*(height + 2)];
    count += grid[(y - 1) + (x - 1)*(width + 2) + (z - 1)*(width + 2)*(height + 2)];
    //
    count += grid[(y) + x*(width + 2) + (z + 1)*(width + 2)*(height + 2)];
    count += grid[(y) + x*(width + 2) + (z - 1)*(width + 2)*(height + 2)];

    count += grid[(y) + (x + 1)*(width + 2) + (z + 1)*(width + 2)*(height + 2)];
    count += grid[(y) + (x + 1)*(width + 2) + (z)*(width + 2)*(height + 2)];
    count += grid[(y) + (x + 1)*(width + 2) + (z - 1)*(width + 2)*(height + 2)];

    count += grid[(y) + (x - 1)*(width + 2) + z*(width + 2)*(height + 2)];
    count += grid[(y) + (x - 1)*(width + 2) + (z + 1)*(width + 2)*(height + 2)];
    count += grid[(y) + (x - 1)*(width + 2) + (z - 1)*(width + 2)*(height + 2)];

    int index = y + x*(width + 2) + z*(width + 2)*(height + 2);

    if (count == 5)
    {
       new_grid[index] = 1;
    }
    else if (count < 3 || count > 5)
    {
       new_grid[index] = 0;
    }
    else
    {
       new_grid[index] = grid[index];;
    }
}

int increase(volatile __global int* counter)
{
  atomic_add(counter, 8);
  return *counter;
}

kernel void generate_vertices(global short* grid, global float* vertices, global int* count, int size) {
    //calculate the index
    int x = get_global_id(0);
    int y = get_global_id(1);
    float x_t = x * (500 / (float) size) * 0.02f;
    float y_t = y * (500 / (float) size) * 0.02f;
    float off = 500 / (float)size * 0.01f;
    int index = (x + 1) * (size + 2) + (y + 1);
    bool alive = grid[index] == 1;
    if (alive) {
		int i = increase(count); // 8 vertices per cell
        vertices[i-8] = x_t - off;
        vertices[i-7] = y_t - off;
        vertices[i-6] = x_t + off;
        vertices[i-5] = y_t - off;
        vertices[i-4] = x_t + off;
        vertices[i-3] = y_t + off;
        vertices[i-2] = x_t - off;
        vertices[i-1] = y_t + off;
	}
}
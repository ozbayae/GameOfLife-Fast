#include "GLScene.h"
#include "Stopwatch.h"
#include "Benchmark.h"

int size = 500;

Life3d * life3d = new Life3d(size / 5, size / 5, size / 5);
Life * life = new Life(size, size);

int window_width;
int window_height;
int g_GLUTWindowHandle;
int g_ErrorCode;
float y_offset;
float x_offset;
float scal = 0.0f;

float rot_x = 0.1f;
float rot_y = 0.7f;
float rot_z = 0.3f;
float rot_angle = 0.1f;
bool b_rot = true;
bool sim = true;
bool shade = false;
int time_e = clock();

Scene g_current = scene1;

//whether to show output of various timers
bool timer = false;

//benchmarking variables
bool benchmark = true;

std::vector<int> sizes2d = { 1000, 2000, 3000, 4000, 5000 };
const int iterations = 60; //per size how many frames will be collected for the average and std dev
const int warmup_cycles = 100; //how many frames to run before collecting data to warm up the cache

Benchmark render_bench2d(
	"2D Rendering Benchmark",
	sizes2d, //sizes
	iterations, //iterations
	warmup_cycles //warmup cycles
);
Benchmark simulation_bench2d(
	"2D Simulation Benchmark",
	sizes2d, //sizes, has to be the same as render_bench2d
	iterations, //iterations
	warmup_cycles //warmup cycles
);

std::vector<int> sizes3d = { 400, 500, 600, 700, 800 };
Benchmark render_bench3d(
	"3D Rendering Benchmark",
	sizes3d, //sizes
	iterations, //iterations
	warmup_cycles //warmup cycles
);

Benchmark simulation_bench3d(
	"3D Simulation Benchmark",
	sizes3d, //sizes, has to be the same as render_bench2d
	iterations, //iterations
	warmup_cycles //warmup cycles
);


void GLScene(int argc, char*argv[])
{
	GLScene(900, 900, argc, argv);
}

void GLScene(int x, int y, int argc, char*argv[])
{
	//for (int i = 0; i < 100000; i++)
	//{
	//	int x = rand() % size + 1;
	//	int y = rand() % size + 1;
	//	life->setLife(x, y, 1);
	//}
	newLife();
	newlife3d();
	cout << glutGet(GLUT_ELAPSED_TIME) << endl;
	window_height = y;
	window_width = x;

	glutInit(&argc, argv);

	glutInitWindowPosition(30, 30);
	glutInitWindowSize(window_width, window_height);

	window_width = glutGet(GLUT_SCREEN_WIDTH);
	window_height = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);


	g_GLUTWindowHandle = glutCreateWindow("Conway's Game of Life");
	glutInitWindowSize(window_width, window_height);

	glutDisplayFunc(DisplayGL);
	glutKeyboardFunc(KeyboardGL);
	glutReshapeFunc(ReshapeGL);


	glClearColor(0.156f, 0.172f, 0.203f, 1.00f);
	glClearDepth(1.0f);
	glShadeModel(GL_SMOOTH);

}

void Cleanup()
{
	if (g_GLUTWindowHandle != 0)
	{
		glutDestroyWindow(g_GLUTWindowHandle);
		g_GLUTWindowHandle = 0;
	}
	if (false)
	{
		exit(g_ErrorCode);
	}

}

void newLife()
{
	// ew--calling a destructor
	life->~Life();
	life = new Life(size, size);
	life->randomize();

}

void newlife3d()
{
	// ew--calling a destructor
	life3d->~Life3d();
	life3d = new Life3d(size / 5, size / 5, size / 5);
	life3d->randomize();

}

void DisplayGL()
{
	if (benchmark) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		static bool doing2d = true;
		if (doing2d) {
			int current_size = render_bench2d.GetCurrentSize();
			if (size != current_size) {
				size = current_size;
				newLife();
			}
			if (render_bench2d.Advance()) {
				render_bench2d.StartTiming();
				render();
				render_bench2d.StopTiming();
			}
			if (simulation_bench2d.Advance()) {
				simulation_bench2d.StartTiming();
				life->update();
				simulation_bench2d.StopTiming();
			}
			if (render_bench2d.IsDone() && simulation_bench2d.IsDone()) {
				doing2d = false;

				glEnable(GL_DEPTH_TEST);
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
				g_current = scene2;
			}
		}
		else {
			//because the simulation is run at a different rate than the rendering,
			// we benchmark the simulation and rendering separately
			static bool doing3dRender = true;
			if (doing3dRender) {
				int current_size = render_bench3d.GetCurrentSize();
				if (size != current_size) {
					size = current_size;
					newlife3d();
				}
				if (render_bench3d.Advance()) {
					render_bench3d.StartTiming();
					render3d();
					render_bench3d.StopTiming();
					if ((int)(clock() - time_e) > 100)
					{
						time_e = clock();
						life3d->update();
					}
				}
				if (render_bench3d.IsDone())
					doing3dRender = false;
			}
			else {
				int current_size = simulation_bench3d.GetCurrentSize();
				if (size != current_size) {
					size = current_size;
					newlife3d();
				}
				if (simulation_bench3d.Advance()) {
					simulation_bench3d.StartTiming();
					life3d->update();
					simulation_bench3d.StopTiming();
				}
				if (render_bench3d.IsDone() && simulation_bench3d.IsDone()) {
					benchmark = false;
					exit(0);
				}
			}

		}

	}
	else {
		Stopwatch tm;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (g_current == 0)
		{
			if (timer) tm.start();
			render();
			if (timer) tm.stop("Render 2D");


			if (sim == true)
			{
				if (timer) tm.start();
				life->update();
				if (timer) tm.stop("Simulation 2D");
			}


			//testInstancedRendering();
		}
		else if (g_current == 1)
		{
			if (timer) tm.start();
			render3d();
			if (timer) tm.stop("Render 3D");

			if (sim == true)
			{
				if ((int)(clock() - time_e) > 100)
				{
					time_e = clock();
					if (timer) tm.start();
					life3d->update();
					if (timer) tm.stop("Simulation 3D");
				}
			}

		}
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void KeyboardGL(unsigned char c, int x, int y)
{
	if (c == ' ')
	{
		sim = !sim;
	}
	if (c == 'r')
	{
		b_rot = !(b_rot);
	}
	if (c == '	')
	{
		if (g_current == scene1)
		{
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			(g_current = scene2);
		} else
		if (g_current == scene2)
		{
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			g_current = scene1;
		}
	}
	if (c == 'w')
	{
		y_offset = y_offset - 0.1;
	}

	if (c == 's')
	{
		y_offset += 0.1;
	}

	if (c == 'd')
	{
		x_offset -= 0.1;
	}
	
	if (c == 'a')
	{
		x_offset += 0.1;
	}
	if (c == ',')
	{
		scal -= 0.1f;
	}
	if (c == 'c')
	{
		shade = !shade;
	}

	if (c == '.')
	{
		scal += 0.1f;
	}
	if (c == '1')
	{
		size = 200;
		newLife();
		newlife3d();
	}

	if (c == '2')
	{
		size = 200;
		newLife();
		newlife3d();
	}
	if (c == '3')
	{
		size = 300;
		newLife();
		newlife3d();
	}
	if (c == '4')
	{
		size = 400;
		newLife();
		newlife3d();
	}
	if (c == '5')
	{
		size = 500;
		newLife();
		newlife3d();
	}
	if (c == '6')
	{
		size = 600;
		newLife();
		newlife3d();
	}
	if (c == '7')
	{
		size = 700;
		newLife();
		newlife3d();
	}
	if (c == '8')
	{
		size = 800;
		newLife();
		newlife3d();
	}
	if (c == '9')
	{
		size = 900;
		newLife();
	}
	if (c == '!')
	{
		size = 1000;
		newLife();
	}
	if (c == '@')
	{
		size = 2000;
		newLife();
	}
	if (c == '#')
	{
		size = 3000;
		newLife();
	}
	if (c == '$')
	{
		size = 4000;
		newLife();
	}
	if (c == '%')
	{
		size = 5000;
		newLife();
	}
}

void ReshapeGL(int w, int h)
{
	//std::cout << "ReshapGL( " << w << ", " << h << " );" << std::endl;

	if (h == 0)										// Prevent A Divide By Zero error
	{
		h = 1;										// Making Height Equal One
	}

	window_width = w;
	window_height = h;

	glViewport(0, 0, window_width, window_height);

	// Setup the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)window_width / (GLdouble)window_height, 0.1, 100.0);

	//render();
	glutPostRedisplay();
}

void render()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float y_t = 0.0f;
	float x_t = 0.0f;
	float off = 500 / (float)size *0.01f;

	//glScalef(1.0f+scal, 1.0f+scal, 1.0f+scal);
	glTranslatef(-5.0f + x_offset, -5.0f + y_offset, -9.0f+scal);
	//glTranslatef(-0.5f, -0.5f, 0.0f);
	if(shade == false) glColor3f((169.0f / 255.0f), (234.0f / 255.0f), (123.0f / 255.0f));
	//GLfloat cyan[] = { (169.0f / 255.0f), (234.0f / 255.0f), (123.0f / 255.0f), 1.f };
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
	glBegin(GL_QUADS);
	for (int i = 0; i < size; i++)
	{
		x_t = 0.0f;
		for (int j = 0; j < size; j++)
		{
			if (life->getLifeform(j + 1, i + 1) == 1)
			{
				if (shade == true) glColor3f(((float)i / (float)size), ((float)j / (float)size), 1.0f);
				glVertex2f(x_t - off, y_t + off);
				glVertex2f(x_t + off, y_t + off);
				glVertex2d(x_t + off, y_t - off);
				glVertex2d(x_t - off, y_t - off);
			}

			x_t += (500/(float)size) * 0.02f;
		}
		y_t += (500 / (float)size) *0.02f;
	}
	glEnd();
	glPopMatrix();
	//if (sim == true)
	//{
	//	life->update();
	//}
	
}

void render3d()
{
	if (size < 801)
	{
		float y_t = 0.0f;
		float x_t = 0.0f;
		float z_t = 0.0f;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		if (shade == false)
		{
			GLfloat green[] = { (169.0f / 255.0f), (234.0f / 255.0f), (123.0f / 255.0f), 1.f };
			glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
		}
		/* clear color and depth buffers */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glTranslatef(0.0f + x_offset, 0.0f + y_offset, -7.0f + scal);
		glRotatef(rot_angle/3, rot_x, rot_y, rot_z);
		glRotatef(rot_angle/3, rot_z, rot_y, rot_x);
		glRotatef(rot_angle/3, rot_x, rot_z, rot_y);
		glTranslatef(-2.0f, -2.0f, -2.0f);
		
		glBegin(GL_QUADS);
		float sz = 2.0f * 5.0f / size;
		for (int i = 0; i < size / 5; i++)
		{
			y_t = 0.0f;
			for (int j = 0; j < size / 5; j++)
			{
				x_t = 0.0f;
				for (int k = 0; k < size / 5; k++)
				{
					if (life3d->getLifeform(k + 1, j + 1, i + 1) == 1)
					{
						if (shade == true)
						{
							GLfloat green[] = { 5.0f * ((float)i / (float)size), 5.0f * ((float)j / (float)size), 5.0f * ((float)k / (float)size) };
							glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
						}

						glNormal3f(0.0F, 0.0F, 1.0F);
						glVertex3f(sz + x_t, sz + y_t, sz + z_t); glVertex3f(-sz + x_t, sz + y_t, sz + z_t);
						glVertex3f(-sz + x_t, -sz + y_t, sz + z_t); glVertex3f(sz + x_t, -sz + y_t, sz + z_t);

						glNormal3f(0.0F, 0.0F, -1.0F);
						glVertex3f(-sz + x_t, -sz + y_t, -sz + z_t); glVertex3f(-sz + x_t, sz + y_t, -sz + z_t);
						glVertex3f(sz + x_t, sz + y_t, -sz + z_t); glVertex3f(sz + x_t, -sz + y_t, -sz + z_t);

						glNormal3f(0.0F, 1.0F, 0.0F);
						glVertex3f(sz + x_t, sz + y_t, sz + z_t); glVertex3f(sz + x_t, sz + y_t, -sz + z_t);
						glVertex3f(-sz + x_t, sz + y_t, -sz + z_t); glVertex3f(-sz + x_t, sz + y_t, sz + z_t);

						glNormal3f(0.0F, -1.0F, 0.0F);
						glVertex3f(-sz + x_t, -sz + y_t, -sz + z_t); glVertex3f(sz + x_t, -sz + y_t, -sz + z_t);
						glVertex3f(sz + x_t, -sz + y_t, sz + z_t); glVertex3f(-sz + x_t, -sz + y_t, sz + z_t);

						glNormal3f(1.0F, 0.0F, 0.0F);
						glVertex3f(sz + x_t, sz + y_t, sz + z_t); glVertex3f(sz + x_t, -sz + y_t, sz + z_t);
						glVertex3f(sz + x_t, -sz + y_t, -sz + z_t); glVertex3f(sz + x_t, sz + y_t, -sz + z_t);

						glNormal3f(-1.0F, 0.0F, 0.0F);
						glVertex3f(-sz + x_t, -sz + y_t, -sz + z_t); glVertex3f(-sz + x_t, -sz + y_t, sz + z_t);
						glVertex3f(-sz + x_t, sz + y_t, sz + z_t); glVertex3f(-sz + x_t, sz + y_t, -sz + z_t);
					}
					x_t += sz*2.0f;
				}
				y_t += sz*2.0f;
			}
			z_t += sz*2.0f;
		}
		glEnd();
		//if (sim == true)
		//{
		//	//cout << clock() - time_e << endl;
		//	if ((int)(clock() - time_e) > 100 )
		//	{
		//		time_e = clock();
		//		life3d->update();
		//	}
		//}

		if (b_rot)
		{
			rot_angle++;
			rot_x = ((int)((rot_x + 1.0f) * 10.0f) % 10)/10.0f;
			rot_y = ((int)((rot_y + 1.0f) * 10.0f) % 10) / 10.0f;
			rot_z = ((int)((rot_z + 1.0f) * 10.0f) % 10) / 10.0f;
		}
		glPopMatrix();
	}
}


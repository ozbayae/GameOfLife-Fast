#include "GLScene.h"
#include "Stopwatch.h"
#include <vector>

int size = 500;

Life3d* life3d = new Life3d(size / 5, size / 5, size / 5);
Life* life = new Life(size, size);

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

//std::vector<GLfloat> vertices;

void GLScene(int argc, char* argv[])
{
	GLScene(900, 900, argc, argv);
}

void GLScene(int x, int y, int argc, char* argv[])
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
	//vertices.clear();

	life = new Life(size, size);

	////precalculate the vertices for the grid so we don't have to do it every frame
	//float y_t = 0.0f;
	//float x_t = 0.0f;
	//float off = 500 / (float)size * 0.01f;
	//for (int i = 0; i < size; i++)
	//{
	//	x_t = 0.0f;
	//	for (int j = 0; j < size; j++)
	//	{
	//		if (shade == true) vertices.push_back(((float)i / (float)size));
	//		if (shade == true) vertices.push_back(((float)j / (float)size));
	//		if (shade == true) vertices.push_back(1.0f);
	//		vertices.push_back(x_t - off);
	//		vertices.push_back(y_t + off);
	//		vertices.push_back(x_t + off);
	//		vertices.push_back(y_t + off);
	//		vertices.push_back(x_t + off);
	//		vertices.push_back(y_t - off);
	//		vertices.push_back(x_t - off);
	//		vertices.push_back(y_t - off);

	//		x_t += (500 / (float)size) * 0.02f;
	//	}
	//	y_t += (500 / (float)size) * 0.02f;
	//}
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
	Stopwatch tm;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (g_current == 0)
	{
		tm.start();
		render();
		tm.stop("Render 2D");
		//float elapsed = tm.elaspsed();
		//printf( "Render simulation: %5.1f ms\n", elapsed * 1000);
		//screen->Print(t, 2, SCRHEIGHT - 24, 0xffffff);
	}
	else
		if (g_current == 1)
		{
			tm.start();
			render3d();
			tm.stop("Render 3D");
			//float elapsed = tm.elaspsed();
			//printf("Render simulation: %5.1f ms\n", elapsed * 1000);

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
		}
		else
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
		newlife3d();
	}
	if (c == '!')
	{
		size = 1000;
		newLife();
		newlife3d();

	}
	if (c == '@')
	{
		size = 2000;
		newLife();
		newlife3d();
	}
	if (c == '#')
	{
		size = 3000;
		newLife();
		newlife3d();
	}
	if (c == '$')
	{
		size = 4000;
		newLife();
		newlife3d();
	}
	if (c == '%')
	{
		size = 5000;
		newLife();
		newlife3d();
	}
}

//std::vector<GLfloat> vertices;
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



//faster than vector since it is a fixed size, no dynamic allocation needed as vector grows	
GLfloat vertices[5000 * 5000 * 8];

void render()
{
	glewInit();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float y_t = 0.0f;
	float x_t = 0.0f;
	float off = 500 / (float)size * 0.01f;
	float pos_off = 500 / (float)size * 0.02f;

	//glScalef(1.0f+scal, 1.0f+scal, 1.0f+scal);
	glTranslatef(-5.0f + x_offset, -5.0f + y_offset, -9.0f + scal);
	//glTranslatef(-0.5f, -0.5f, 0.0f);
	if (shade == false) {
		glColor3f((169.0f / 255.0f), (234.0f / 255.0f), (123.0f / 255.0f));
		Stopwatch tm;
		tm.start();

		int vCount = 0;
		int cCount = 0;
		for (int i = 0; i < size; i++)
		{
			x_t = 0.0f;
			for (int j = 0; j < size; j++)
			{
				if (life->getLifeform(j + 1, i + 1) == 1)
				{
					vertices[vCount++] = x_t - off;
					vertices[vCount++] = y_t + off;
					vertices[vCount++] = x_t + off;
					vertices[vCount++] = y_t + off;
					vertices[vCount++] = x_t + off;
					vertices[vCount++] = y_t - off;
					vertices[vCount++] = x_t - off;
					vertices[vCount++] = y_t - off;
				}

				x_t += pos_off;
			}
			y_t += pos_off;
		}
		tm.stop("Loading vertices into array");

		tm.start();
		//count live with get life form
		int live_cells = 0;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++)
			{
				if (life->getLifeform(j + 1, i + 1) == 1)
				{
					live_cells++;
				}
			}
		}

		//live cells is used for the size of the array of vertices
		tm.stop("Counting live cells");

		tm.start();
		GLuint vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vCount * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, 0);

		glDrawArrays(GL_QUADS, 0, vCount / 2);

		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &vbo);

		tm.stop("Drawing vertices");
	}
	else {
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

				x_t += (500 / (float)size) * 0.02f;
			}
			y_t += (500 / (float)size) * 0.02f;
		}
		glEnd();
	}
	glPopMatrix();
	//GLfloat cyan[] = { (169.0f / 255.0f), (234.0f / 255.0f), (123.0f / 255.0f), 1.f };
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);

	

	if (sim == true)
	{
		life->update();
	}

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
		glRotatef(rot_angle / 3, rot_x, rot_y, rot_z);
		glRotatef(rot_angle / 3, rot_z, rot_y, rot_x);
		glRotatef(rot_angle / 3, rot_x, rot_z, rot_y);
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
					x_t += sz * 2.0f;
				}
				y_t += sz * 2.0f;
			}
			z_t += sz * 2.0f;
		}
		glEnd();
		if (sim == true)
		{
			//cout << clock() - time_e << endl;
			if ((int)(clock() - time_e) > 100)
			{
				time_e = clock();
				life3d->update();
			}
		}

		if (b_rot)
		{
			rot_angle++;
			rot_x = ((int)((rot_x + 1.0f) * 10.0f) % 10) / 10.0f;
			rot_y = ((int)((rot_y + 1.0f) * 10.0f) % 10) / 10.0f;
			rot_z = ((int)((rot_z + 1.0f) * 10.0f) % 10) / 10.0f;
		}
		glPopMatrix();
	}
}


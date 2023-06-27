#include "GLScene.h"
#include "Stopwatch.h"
#include <vector>
#include "learnopengl-shader.h"
#include <algorithm>
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
//std::vector<glm::vec2> positions;

//whether to show output of timers
bool timer = false;

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

	////fill positions vector
	//for (int i = 0; i < size; i++)
	//{
	//	for (int j = 0; j < size; j++)
	//	{
	//		positions.push_back(glm::vec2(i, j));
	//	}
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
		if(timer) tm.start();
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
	else
		if (g_current == 1)
		{
			if(timer) tm.start();
			render3d();
			if(timer) tm.stop("Render 3D");

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

	//our keys
	if (c == 'b')
	{
		std::cout << "Do the benchmark!" << std::endl;
	}
	if (c == 't')
	{
		timer = !timer;
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

	Stopwatch tm;

	//glScalef(1.0f+scal, 1.0f+scal, 1.0f+scal);
	glTranslatef(-5.0f + x_offset, -5.0f + y_offset, -9.0f + scal);
	//glTranslatef(-0.5f, -0.5f, 0.0f);
	if (shade == false) {
		glColor3f((169.0f / 255.0f), (234.0f / 255.0f), (123.0f / 255.0f));
		if(timer) tm.start();

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
		if(timer) tm.stop("Loading vertices into array");

		//tm.start();
		////count live with get life form
		//int live_cells = 0;
		//for (int i = 0; i < size; i++) {
		//	for (int j = 0; j < size; j++)
		//	{
		//		if (life->getLifeform(j + 1, i + 1) == 1)
		//		{
		//			live_cells++;
		//		}
		//	}
		//}

		////live cells is used for the size of the array of vertices
		//tm.stop("Counting live cells");
		//std::cout << "live cells:" << live_cells << std::endl;

		if(timer) tm.start(); //drawing vertices
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

		if(timer) tm.stop("Drawing vertices");
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

		//define the 3d cube with vertices and normals
		float quadCube[] = {
			sz + x_t, sz + y_t, sz + z_t, 0.0f, 0.0f, 1.0f,
			sz + x_t, -sz + y_t, sz + z_t, 0.0f, 0.0f, 1.0f,
			-sz + x_t, -sz + y_t, sz + z_t, 0.0f, 0.0f, 1.0f,
			-sz + x_t, sz + y_t, sz + z_t, 0.0f, 0.0f, 1.0f,

			sz + x_t, sz + y_t, -sz + z_t, 0.0f, 0.0f, -1.0f,
			sz + x_t, -sz + y_t, -sz + z_t, 0.0f, 0.0f, -1.0f,
			-sz + x_t, -sz + y_t, -sz + z_t, 0.0f, 0.0f, -1.0f,
			-sz + x_t, sz + y_t, -sz + z_t, 0.0f, 0.0f, -1.0f,

			sz + x_t, sz + y_t, sz + z_t, 0.0f, 1.0f, 0.0f,
			sz + x_t, sz + y_t, -sz + z_t, 0.0f, 1.0f, 0.0f,
			-sz + x_t, sz + y_t, -sz + z_t, 0.0f, 1.0f, 0.0f,
			-sz + x_t, sz + y_t, sz + z_t, 0.0f, 1.0f, 0.0f,

			sz + x_t, -sz + y_t, sz + z_t, 0.0f, -1.0f, 0.0f,
			sz + x_t, -sz + y_t, -sz + z_t, 0.0f, -1.0f, 0.0f,
			-sz + x_t, -sz + y_t, -sz + z_t, 0.0f, -1.0f, 0.0f,
			-sz + x_t, -sz + y_t, sz + z_t, 0.0f, -1.0f, 0.0f,

			sz + x_t, sz + y_t, sz + z_t, 1.0f, 0.0f, 0.0f,
			sz + x_t, -sz + y_t, sz + z_t, 1.0f, 0.0f, 0.0f,
			sz + x_t, -sz + y_t, -sz + z_t, 1.0f, 0.0f, 0.0f,
			sz + x_t, sz + y_t, -sz + z_t, 1.0f, 0.0f, 0.0f,

			-sz + x_t, sz + y_t, sz + z_t, -1.0f, 0.0f, 0.0f,
			-sz + x_t, -sz + y_t, sz + z_t, -1.0f, 0.0f, 0.0f,
			-sz + x_t, -sz + y_t, -sz + z_t, -1.0f, 0.0f, 0.0f,
			-sz + x_t, sz + y_t, -sz + z_t, -1.0f, 0.0f, 0.0f
		};



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

void testInstancedRendering() {
	glewInit();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader shader("shaders/10.1.instancing.vs", "shaders/10.1.instancing.fs");

	// generate a list of 100 quad locations/translation-vectors
	// ---------------------------------------------------------
	glm::vec2 translations[100];
	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2)
	{
		for (int x = -10; x < 10; x += 2)
		{
			glm::vec2 translation;
			translation.x = (float)x / 10.0f + offset;
			translation.y = (float)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}

	// store instance data in an array buffer
	// --------------------------------------
	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float quadVertices[] = {
		// positions     // colors
		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
	};
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	// also set instance data
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw 100 instanced quads
	shader.use();
	glBindVertexArray(quadVAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
	glBindVertexArray(0);


	glPopMatrix();
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
}


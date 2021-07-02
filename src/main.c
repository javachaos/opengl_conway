#include <stdio.h>
#include <GL/glut.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include <../include/binary_matrix.h>

static volatile int keepRunning = 1;
BinaryMatrix *M;
void intHandler(int dummy)
{
	keepRunning = 0;
}

void drawRect(BinaryMatrix *M, int x, int y, int w, int h) {
	if (x > M->num_cols || y > M->num_rows 
	 || x < 0 || y < 0
	 || w > M->num_cols || h > M->num_rows 
	 || w < 0 || h < 0) {
		return;
	}

	//Draw X axis                     // (x, y)
	for (int i = 0; i < w; i++) {     // -------------  <-- 1.
		UpdateEntry(M, y, x+i, 1);    // |           | (x+w, y)
	}                                 // |           |
	//Draw Y axis                     // | <-- 2.    |
	for (int j = 0; j < h; j++) {     // |           | 
		UpdateEntry(M, y+j, x, 1);    // |     3 --> |
	}                                 // |           |  (x+w, y+h)
	for (int k = 0; k < h; k++) {     // -------------  <-- 4
		UpdateEntry(M, y+k, x+w, 1);  // (x, y+h)
	}
	for (int l = 0; l < w; l++) {
		UpdateEntry(M, y+h, x+l, 1);
	}
}

void drawGasket(BinaryMatrix *M, int x, int y, int dimension) {
	if (dimension <= 8) {
		drawRect(M, x, y, dimension, dimension);
	} else {
		float newDimension = dimension / 2;
		drawGasket(M, x, y, newDimension);
		drawGasket(M, x + newDimension, y, newDimension);
		drawGasket(M, x + newDimension, y + newDimension, newDimension);
	}
}

void render() {
	int i, j, k;
    glBegin(GL_POINTS);
	for (i = 0; i < M->num_rows; i++)
	{
		for (j = 0; j < M->num_cols; j++)
		{
            if (CheckEntry(M,i,j)) {
                glColor3f(1,1,1);
                glVertex2i(i,j);

            } else {
                glColor3f(0,0,0);
                glVertex2i(i,j);
            }
		}
	}
    glEnd();
    glutSwapBuffers();
}

void update() {

	int x, y, z = 0;
	int width = M->num_cols - 1;
	int height = M->num_rows - 1;

	int n = 0;//number of mutations per iteration
		for (x = 0; x < height; x++)
		{
			for (y = 0; y < width; y++)
			{
				int k = 0;
				k += CheckEntry(M, x - 1, y - 1);
				k += CheckEntry(M, x, y - 1);
				k += CheckEntry(M, x + 1, y - 1);
				k += CheckEntry(M, x - 1, y);
				k += CheckEntry(M, x + 1, y);
				k += CheckEntry(M, x - 1, y + 1);
				k += CheckEntry(M, x, y + 1);
				k += CheckEntry(M, x + 1, y + 1);
				if (CheckEntry(M, x, y))
				{
 
					if (k < 2 || k > 3)
					{
						UpdateEntry(M, x, y, 0);
						n++;
					}
					else if (k == 2 || k == 3)
					{
						UpdateEntry(M, x, y, 1);
					}
				}
				else
				{
					if (k == 3)
					{
						UpdateEntry(M, x, y, 1);
						n++;
					}
					else
					{
						UpdateEntry(M, x, y, 0);
					}
				}
			}
		}
		if (n == 0)
		{
	        return;
		}
        render(M);
}

void init() {
    time_t t;
    srand((unsigned) time(&t));
	int x, y, z = 0;
	int width = M->num_cols - 1;
	int height = M->num_rows - 1;
    //Initialize the matrix randomly.
	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			int v = (rand() % 100) > 97 ? 1 : 0;
			if (v)
			{
				UpdateEntry(M, y, x, v);
			}
		}
	}
}

int main(int argc, char* argv[]) {

	signal(SIGINT, intHandler);
    M = ConstructBinaryMatrix(800,600);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(800,600);
    glutCreateWindow("Conway's game of life.");

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, 800.0, 600.0,0.0 );

    struct timespec sleeptime;
	sleeptime.tv_sec = 0;
	sleeptime.tv_nsec = 5000000;
	if (argc == 1) {
        init();
		glutDisplayFunc(render);
		while (keepRunning) {
			update();
			nanosleep(&sleeptime, NULL);
		}
	} else if (argc == 5) {
			int x = atoi(argv[1]);
			int y = atoi(argv[2]);
			int w = atoi(argv[3]);
			int h = atoi(argv[4]);
			drawRect(M, x, y, w, h);
		    glutDisplayFunc(render);
			render(M);
			while (keepRunning) {
				nanosleep(&sleeptime, NULL);
			}
	} else {
		glutDisplayFunc(render);
		drawGasket(M, 0, 0, 600);
		render(M);
		while (keepRunning) {
			nanosleep(&sleeptime, NULL);
		}
	}
	DeleteBinaryMatrix(M);
}

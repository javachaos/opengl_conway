#include <stdio.h>
#include <GL/glut.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include <../include/binary_matrix.h>

static volatile int keepRunning = 1;
BinaryMatrix *MATRIX;
void intHandler(int dummy)
{
	keepRunning = 0;
}

void drawRect(int x, int y, int w, int h) {
	if (x > MATRIX->num_cols || y > MATRIX->num_rows 
	 || x < 0 || y < 0
	 || w > MATRIX->num_cols || h > MATRIX->num_rows 
	 || w < 0 || h < 0) {
		return;
	}

	//Draw X axis                          // (x, y)
	for (int i = 0; i < w; i++) {          // -------------  <-- 1.
		UpdateEntry(MATRIX, y, x+i, 1);    // |           | (x+w, y)
	}                                      // |           |
	//Draw Y axis                          // | <-- 2.    |
	for (int j = 0; j < h; j++) {          // |           | 
		UpdateEntry(MATRIX, y+j, x, 1);    // |     3 --> |
	}                                      // |           |  (x+w, y+h)
	for (int k = 0; k < h; k++) {          // -------------  <-- 4
		UpdateEntry(MATRIX, y+k, x+w, 1);  // (x, y+h)
	}
	for (int l = 0; l < w; l++) {
		UpdateEntry(MATRIX, y+h, x+l, 1);
	}
}

void drawGasket(int x, int y, int dimension) {
	if (dimension <= 8) {
		drawRect(x, y, dimension, dimension);
	} else {
		float newDimension = dimension / 2;
		drawGasket(x, y, newDimension);
		drawGasket(x + newDimension, y, newDimension);
		drawGasket(x + newDimension, y + newDimension, newDimension);
	}
}

void render() {
	int i, j, k;
    glBegin(GL_POINTS);
	for (i = 0; i < MATRIX->num_rows; i++)
	{
		for (j = 0; j < MATRIX->num_cols; j++)
		{
            if (CheckEntry(MATRIX,i,j)) {
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
	int width = MATRIX->num_cols - 1;
	int height = MATRIX->num_rows - 1;

	int n = 0;//number of mutations per iteration
	    
	#pragma omp parallel for
		for (x = 0; x < height; x++)
		{
	#pragma omp parallel for
			for (y = 0; y < width; y++)
			{
				int k = 0;
				k += CheckEntry(MATRIX, x - 1, y - 1);
				k += CheckEntry(MATRIX, x, y - 1);
				k += CheckEntry(MATRIX, x + 1, y - 1);
				k += CheckEntry(MATRIX, x - 1, y);
				k += CheckEntry(MATRIX, x + 1, y);
				k += CheckEntry(MATRIX, x - 1, y + 1);
				k += CheckEntry(MATRIX, x, y + 1);
				k += CheckEntry(MATRIX, x + 1, y + 1);
				if (CheckEntry(MATRIX, x, y))
				{
 
					if (k < 2 || k > 3)
					{
						UpdateEntry(MATRIX, x, y, 0);
						n++;
					}
					else if (k == 2 || k == 3)
					{
						UpdateEntry(MATRIX, x, y, 1);
					}
				}
				else
				{
					if (k == 3)
					{
						UpdateEntry(MATRIX, x, y, 1);
						n++;
					}
					else
					{
						UpdateEntry(MATRIX, x, y, 0);
					}
				}
			}
		}
		if (n == 0)
		{
	        return;
		}
        render();
}

void init() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, 800.0, 600.0,0.0 );

    time_t t;
    srand((unsigned) time(&t));
	int x, y, z = 0;
	int width = MATRIX->num_cols - 1;
	int height = MATRIX->num_rows - 1;
    //Initialize the matrix randomly.
	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			int v = (rand() % 100) > 97 ? 1 : 0;
			if (v)
			{
				UpdateEntry(MATRIX, y, x, v);
			}
		}
	}
}

int main(int argc, char* argv[]) {

	signal(SIGINT, intHandler);
    MATRIX = ConstructBinaryMatrix(800,600);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(800,600);
    glutCreateWindow("Conway's game of life.");

    struct timespec sleeptime;
	sleeptime.tv_sec = 0;
	sleeptime.tv_nsec = 5000000;
	glutDisplayFunc(render);
	if (argc == 1) {
        init();
		while (keepRunning) {
			update();
			nanosleep(&sleeptime, NULL);
		}
	} else if (argc == 5) {
			int x = atoi(argv[1]);
			int y = atoi(argv[2]);
			int w = atoi(argv[3]);
			int h = atoi(argv[4]);
			drawRect(x, y, w, h);
			render();
			while (keepRunning) {
				nanosleep(&sleeptime, NULL);
			}
	} else {
		drawGasket(0, 0, 600);
		render();
		while (keepRunning) {
			nanosleep(&sleeptime, NULL);
		}
	}
	DeleteBinaryMatrix(MATRIX);
}

#include <stdio.h>
#include <GL/glut.h>
#include <signal.h>
#include <time.h>
#include <../include/binary_matrix.h>

static volatile int keepRunning = 1;
static BinaryMatrix *M;
void intHandler(int dummy)
{
	keepRunning = 0;
}

void render() {
	int i, j, k;
	for (i = 0; i < M->num_rows; i++)
	{
		for (j = 0; j < M->num_cols; j++)
		{
            glBegin(GL_POINTS);
            if (CheckEntry(M,i,j)) {
                glColor3f(1,1,1);
                glVertex2i(i,j);

            } else {
                glColor3f(0,0,0);
                glVertex2i(i,j);
            }
            glEnd();
		}
	}
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
    M = ConstructBinaryMatrix(500,500);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(500,500);
    glutCreateWindow("Conway's game of life.");

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, 500.0, 500.0,0.0 );

    init();
    struct timespec sleeptime;
	sleeptime.tv_sec = 0;
	sleeptime.tv_nsec = 50000000;
    glutDisplayFunc(render);
    while (keepRunning) {
        update();
    }
	DeleteBinaryMatrix(M);
}

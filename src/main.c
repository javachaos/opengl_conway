#include <stdio.h>
#include <GL/glut.h>
#include <signal.h>
#include <time.h>
#include <../include/binary_matrix.h>

static volatile int keepRunning = 1;

void intHandler(int dummy)
{
	keepRunning = 0;
}


void update(BinaryMatrix *M) {
    struct timespec sleeptime;
	sleeptime.tv_sec = 0;
	sleeptime.tv_nsec = 50000000;


	int x, y, z = 0;

	int width = M->num_cols - 1;
	int height = M->num_rows - 1;

	//Initialize the matrix randomly.
	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			int v = (rand() % 100) > 90 ? 1 : 0;
			if (v)
			{
				UpdateEntry(M, y, x, v);
			}
		}
	}
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
}

void render(BinaryMatrix *M) {
	int i, j, k;
	for (i = 0; i < M->num_rows; i++)
	{
		for (j = 0; j < M->num_cols; j++)
		{
			int x = test(M->data, i * M->num_cols + j);
			if (x)
			{
                glColor3f(1,1,1);
                glVertex2i(i,j);
			} else {
                glColor3f(0,0,0);
                glVertex2i(i,j);
            }
		}
	}
}

int main(int argc, char* argv[]){

    BinaryMatrix *M = ConstructBinaryMatrix(500, 500);
	signal(SIGINT, intHandler);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(500,500);

    glutCreateWindow("A Simple OpenGL Program");

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, 500.0, 500.0,0.0 );

    glBegin(GL_POINTS);
    while (keepRunning) {
        update(M);
        render(M);
    }
    
	DeleteBinaryMatrix(M);
    glEnd();
}

#include "../include/binary_matrix.h"
 
BinaryMatrix *ConstructBinaryMatrix(int rows, int cols)
{
	if (rows > 0 && cols > 0)
	{
		BinaryMatrix *b = malloc(sizeof(BinaryMatrix));
		if (b != NULL)
		{
			b->num_cols = cols;
			b->num_rows = rows;
			b->data = NULL;
			b->data = (int *)calloc(rows, (cols / BITS_PER_BYTE) + 1);
			if (b->data == NULL)
			{
				printf("Array too large, failed to allocate memory.\n");
				DeleteBinaryMatrix(b);
				return NULL;
			}
		}
		return b;
	}
	else
	{
		printf("Error in CreateMatrix: number of rows and columns must be positive, returning NULL.\n");
		return NULL;
	}
}

void DeleteBinaryMatrix(BinaryMatrix *M)
{
	free(M->data);
	free(M);
}

void UpdateEntry(BinaryMatrix *M, int row, int col, int c)
{
	if (IsMatrixIndex(M, row, col))
	{
		if (c == 1)
		{
			setBit(M->data, row * M->num_cols + col);
		}
		else if (c == 0)
		{
			clearBit(M->data, row * M->num_cols + col);
		}
		else
		{
			printf(
				"Error in UpdateEntry: content must be 0 or 1\n");
		}
	}
	else
	{
		printf(
			"Error in UpdateEntry: index out of bounds.\n");
	}
}

int IsMatrixIndex(BinaryMatrix *M, int row, int col)
{
	if (M == NULL)
	{
		printf(
			"IsMatrixIndex Error: NULL parameter passed.\n");
	}
	else
	{
		return row < M->num_rows && col < M->num_cols && col >= 0 && row >= 0;
	}
}

int CheckEntry(BinaryMatrix *M, int row, int col)
{
	if (IsMatrixIndex(M, row, col))
	{
		return test(M->data, row * M->num_cols + col);
	}
	else
	{
		return 0;
	}
}

void PrintMatrix(BinaryMatrix *M)
{
	int i, j, k;
	//Draw first line for this snapshot
	for (k = 0; k < M->num_cols; k++)
	{
		putchar('-');
	}
	for (i = 0; i < M->num_rows; i++)
	{
		putchar('\n');
		for (j = 0; j < M->num_cols; j++)
		{
			int x = test(M->data, i * M->num_cols + j);
			if (x)
			{
				putchar('1');
			}
			else
			{
				putchar('0');
			}
		}
	}
	putchar('\n');
	//Draw the bottom line for this snapshot
	for (k = j; k > 0; k--)
	{
		putchar('-');
	}
	putchar('\n');
}

// How this works.
// Storing bits into an int array.
//http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
//
void setBit(int *A, int k)
{
	A[k / 32] |= 1 << (k % (32));
}

void clearBit(int *A, int k)
{
	A[k / 32] &= ~(1 << (k % (32)));
}

int test(int *A, int k)
{
	return ((A[k / 32] & (1 << (k % (32) ))) != 0);
}

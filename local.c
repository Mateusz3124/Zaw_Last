#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int size;

typedef struct
{
	float ** element;
} matrix;


long long wall_clock_time()
{
#ifdef __linux__
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	return (long long)(tp.tv_nsec + (long long)tp.tv_sec * 1000000000ll);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (long long)(tv.tv_usec * 1000 + (long long)tv.tv_sec * 1000000000ll);
#endif
}

/**
 * Allocates memory for a matrix of size SIZE
 * The memory is allocated row-major order, i.e.
 *  elements from the same row are allocated at contiguous
 *  memory addresses.
 **/
void allocate_matrix(matrix* m)
{
    m->element = malloc(sizeof(float*) * size);
    for (int i = 0; i < size; i++)
	{
        m->element[i] = malloc(sizeof(float) * size);
    }
}

/**
 * Free the memory allocated for a matrix.
 **/
void free_matrix(matrix* m) {
	int i;
	for (i = 0; i < size; i++)
		free(m->element[i]);
	free(m->element);
}

/**
 * Initializes the elements of the matrix with
 * random values between 0 and 9
 **/
void init_matrix(matrix m)
{
	int i, j;

	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
		{
			m.element[i][j] = rand() % 10;
		}
}

/**
 * Initializes the elements of the matrix with
 * element 0.
 **/
void init_matrix_zero(matrix m)
{
	int i, j;

	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
		{
			m.element[i][j] = 0.0;
		}
}


/**
 * Multiplies matrix @a with matrix @b storing
 * the result in matrix @result
 *
 * The multiplication algorithm is the O(n^3)
 * algorithm
 */
void mm(matrix a, matrix b, matrix result)
{
	int i, j, k;

	// Do the multiplication
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			for(k = 0; k < size; k++)
				result.element[i][j] += a.element[i][k] * b.element[k][j];
}

void print_matrix(matrix m)
{
	int i, j;

	for (i = 0; i < size; i++)
	{
		printf("row %4d: ", i);
		for (j = 0; j < size; j++)
			printf("%6.2f  ", m.element[i][j]);
		printf("\n");
	}
}



void work()
{
	matrix a, b, result1;
	long long before, after;

	// Allocate memory for matrices
	allocate_matrix(&a);
	allocate_matrix(&b);
	allocate_matrix(&result1);

	// Initialize matrix elements
	init_matrix(a);
	init_matrix(b);

	// Perform sequential matrix multiplication
	before = wall_clock_time();
	mm(a, b, result1);
	after = wall_clock_time();
        fprintf(stderr, "Matrix multiplication on CPU took %1.2f seconds\n", ((float)(after - before))/1000000000);

	free_matrix(&a);
	free_matrix(&b);
	free_matrix(&result1);
}


int main(int argc, char ** argv)
{
	srand(0);

	printf("Usage: %s <size>\n", argv[0]);

	if (argc >= 2)
		size = atoi(argv[1]);
	else
		size = 1024;

	fprintf(stderr,"Sequential matrix multiplication of size %d\n", size);

	// Multiply the matrices
	work();

	return 0;
}
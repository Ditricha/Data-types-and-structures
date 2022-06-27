#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define S 5

int** create_matrix(int M, int Q, int quantity);
void print_matrix(int **matrix, int M, int Q);
void free_matrix(int **matrix, int N);
void matrix_mult_standard(int **matrix_1, int **matrix_2, int **resulting_matrix, int M, int Q, int N);
void matrix_mult_winograd(int **matrix_1, int **matrix_2, int **resulting_matrix, int M, int Q, int N);
void matrix_mult_winogradOpt(int **matrix_1, int **matrix_2, int **resulting_matrix, int M, int Q, int N);

double PCFreq = 0.0;
__int64 CounterStart = 0;
void StartCounter();
double GetCounter();

int main()
{
	int **matrix_1, **matrix_2, **resulting_matrix;
	int n, q;

	srand(time(NULL));
	matrix_1 = create_matrix(S, S, 1);
	matrix_2 = create_matrix(S, S, 1);
	resulting_matrix = create_matrix(S, S, 0);

	printf("Matrix 1:\n");
	print_matrix(matrix_1, S, S);

	printf("\nMatrix 2:\n");
	print_matrix(matrix_2, S, S);

	printf("\n\n1. Standard matrix multiplication algorithm;\n2. Winograd matrix multiplication algorithm;\n3. Optimized Winograd matrix multiplication algorithm.\n");
	printf("Choose the number of an algorithm: ");
	scanf("%d", &n, sizeof(n));

	/*matrix_mult_standard(matrix_1, matrix_2, resulting_matrix, S, S, S);*/
	/*matrix_mult_winograd(matrix_1, matrix_2, resulting_matrix, S, S, S);*/
	/*matrix_mult_winogradOpt(matrix_1, matrix_2, resulting_matrix, S, S, S);*/

	if (n == 1) {
		StartCounter();
		for (q = 0; q < 1000; q++)
			matrix_mult_standard(matrix_1, matrix_2, resulting_matrix, S, S, S);
		printf("\nTime: %lf\n", GetCounter() / 1000.0);
	}
	else if (n == 2) {
		StartCounter();
		for (q = 0; q < 1000; q++)
			matrix_mult_winograd(matrix_1, matrix_2, resulting_matrix, S, S, S);
		printf("\nTime: %lf\n", GetCounter() / 1000.0);
	}
	else if (n == 3) {
		StartCounter();
		for (q = 0; q < 1000; q++)
			matrix_mult_winogradOpt(matrix_1, matrix_2, resulting_matrix, S, S, S);
		printf("\nTime: %lf\n", GetCounter() / 1000.0);
	}
	else
		printf("\nError: you chose uncorrect number.\n");

	printf("\nThe resulting matrix:\n");
	print_matrix(resulting_matrix, S, S);

	free_matrix(matrix_1, S);
	free_matrix(matrix_2, S);
	free_matrix(resulting_matrix, S);

	getchar();
	return 0;
}


int** create_matrix(int M, int Q, int quantity)
{
	int i, j;
	
	int **matrix = (int**) calloc(M, sizeof(int));
	for (i = 0; i < M; i++)
		matrix[i] = (int*) calloc(Q, sizeof(int));

	if (quantity) {
		for (i = 0; i < Q; i++)
			for (j = 0; j < Q; j++)
				matrix[i][j] = rand() % 10;
	}

	return matrix;
}


void print_matrix(int **matrix, int M, int Q)
{
	int i, j;
	//printf("\nMatrix %d\n", matrix);
	
	for (i = 0; i < M; i++) {
		for (j = 0; j < Q; j++)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
}


void free_matrix(int **matrix, int N)
{
	int i;

	for (i = 0; i < N; i++)
		free((void*)matrix[i]);
	free((void*)matrix);
}


void matrix_mult_standard(int **matrix_1, int **matrix_2, int **resulting_matrix, int M, int Q, int N)
{
	int i, j, k;

	for (i = 0; i < M; i++)
		for (j = 0; j < Q; j++) {
			resulting_matrix[i][j] = 0;
			for (k = 0; k < N; k++)
				resulting_matrix[i][j] = resulting_matrix[i][j] + matrix_1[i][k] * matrix_2[k][j];
		}
}


void matrix_mult_winograd(int **matrix_1, int **matrix_2, int **resulting_matrix, int M, int Q, int N)
{
	int i, j, k;
	int *mulH = (int*)calloc(M, sizeof(int));
	int *mulV = (int*)calloc(N, sizeof(int));

	//I part
	for (i = 0; i < M; i++)
		for (k = 0; k < N / 2; k++)
			mulH[i] = mulH[i] + matrix_1[i][2 * k] * matrix_1[i][2 * k + 1];

	//II part
	for (j = 0; j < Q; j++)
		for (k = 0; k < N / 2; k++)
			mulV[j] = mulV[j] + matrix_2[2 * k][j] * matrix_2[2 * k + 1][j];

	//III part
	for (i = 0; i < M; i++)
		for (j = 0; j < Q; j++) {
			resulting_matrix[i][j] = -mulH[i] - mulV[j];
			for (k = 0; k < N / 2; k++) {
				resulting_matrix[i][j] = resulting_matrix[i][j] + ((matrix_1[i][2 * k] + matrix_2[2 * k + 1][j]) * (matrix_1[i][2 * k + 1] + matrix_2[2 * k][j]));
			}
		}

	// IV part
	if (N % 2 == 1) {
		for (i = 0; i < M; i++)
			for (j = 0; j < Q; j++)
				resulting_matrix[i][j] = resulting_matrix[i][j] + (matrix_1[i][N - 1] * matrix_2[N - 1][j]);
	}

	free((void*)mulH);
	free((void*)mulV);
}

//Улучшения:
//1) for (k = 0; k < N; k++) ~> for (k = 1; k < N; k += 2) I, II, III
//2) += III, IV
//3) mulH[i] = mulH[i] + matrix_1[i][2 * k] * matrix_1[i][2 * k + 1] ~> mulH[i] += matrix_1[i][k - 1] * matrix_1[i][k] I, II, III
//4) += ~> -= из 4), получим - mulH[i] - mulV[j] ~> mulH[i] + mulV[j] III
//5) добавление буффера buff III

void matrix_mult_winogradOpt(int **matrix_1, int **matrix_2, int **resulting_matrix, int M, int Q, int N)
{
	int i, j, k, buff;
	int *mulH = (int*)calloc(M, sizeof(int));
	int *mulV = (int*)calloc(N, sizeof(int));

	//I part
	for (i = 0; i < M; i++)
		for (k = 1; k < N; k += 2)
			mulH[i] -= matrix_1[i][k - 1] * matrix_1[i][k];

	//II part
	for (j = 0; j < Q; j++)
		for (k = 1; k < N; k += 2)
			mulV[j] -= matrix_2[k - 1][j] * matrix_2[k][j];

	//III part
	for (i = 0; i < M; i++)
		for (j = 0; j < Q; j++) {
			resulting_matrix[i][j] = mulH[i] + mulV[j];
			buff = 0;
			for (k = 1; k < N; k += 2)
				buff += (matrix_1[i][k] + matrix_2[k - 1][j]) * (matrix_1[i][k - 1] + matrix_2[k][j]);
			resulting_matrix[i][j] += buff;
		}

	//IV part
	if (N % 2 == 1) {
		for (i = 0; i < M; i++)
			for (j = 0; j < Q; j++)
				resulting_matrix[i][j] += matrix_1[i][N - 1] * matrix_2[N - 1][j];
	}

	free((void*)mulH);
	free((void*)mulV);
}


void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		printf("QueryPerformanceFrequency failed!\n");

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}


double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}

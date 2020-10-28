#include <iostream>

#include <vector>
#include <string>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int** create_rand_smatrix(int nrow, int ncol, float density_percent, float negative_percent)
{
	int negative = 0;
	int total_num = nrow * ncol;
	int num_nz = total_num * density_percent / 100;
	int neg_nz = total_num * negative_percent / 100;
	int pos_nz = num_nz - neg_nz;
	int i;
	
	//не работает
	/*int **matrix = new int* [nrow];
	for (i = 0; i < nrow; i++)
		matrix[i] = new int [ncol];*/
	
	int **matrix = (int**)calloc(nrow, sizeof(int));
	for (i = 0; i < nrow; i++)
		matrix[i] = (int*)calloc(ncol, sizeof(int));

	srand(time(0));

	//генерация рандомной матрицы
	for (i = 0; i < num_nz; i++) {
		int row = rand() % nrow;
		int col = rand() % ncol;
		if (matrix[row][col] != 0) {
			i = i - 1;
			continue;
		}
		else {
			if (negative < neg_nz) {
				matrix[row][col] = (-1) * (rand() % 100 + 1);
				negative++;
			}
			else
				matrix[row][col] = (rand() % 100 + 1);
		}
	}
	return matrix;
}

void print_rand_smatrix(int **matrix, int nrows, int ncols)
{
	cout << endl << "-Random matrix-" << endl;
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			cout.width(3);
			cout << matrix[i][j] << "  ";
		}
		cout << endl;
	}
}

void compressed_matrix(int **matrix, int nrows, int ncols)
{
	int i, j, col = 0, num = 0;
	int JA[100], JR[100];

	cout << endl << "-CRS matrix-" << endl;

	cout << "AN:   ";
	for (i = 0; i < nrows; i++) {
		for (j = 0; j < ncols; j++) {
			if ((matrix[i][j] != 0)) {
				cout.width(3);
				cout << matrix[i][j] << "  ";
			}
		}
	}

	cout << endl << "JA:   ";
	for (i = 0; i < nrows; i++) {
		for (j = 0; j < ncols; j++) {
			if ((matrix[i][j] != 0)) {
				JA[j] = j;
				col += 1;
				cout.width(3);
				cout << JA[j] << "  ";
			}
		}
		JR[0] = 0;
		JR[i + 1] = col;
	}

	cout << endl << "JR:   ";
	for (i = 0; i < nrows + 1; i++) {
		cout.width(3);
		cout << JR[i] << "  ";
	}

	cout << endl << "col  " << col << "\n" << endl;
}

int main()
{
	int **A;
	int n_rows1, n_cols1;
	float nz1, nz_neg1;

	int **B;
	int n_rows2, n_cols2;
	float nz2, nz_neg2;

	cout << "-Matrix 1 generation-" << endl;
	cout << "Inpute rows, columns, density (percentage) of non-zero entries and percentage of negative numbers" << endl;
	cin >> n_rows1 >> n_cols1 >> nz1 >> nz_neg1;

	A = create_rand_smatrix(n_rows1, n_cols1, nz1, nz_neg1);
	print_rand_smatrix(A, n_rows1, n_cols1);
	compressed_matrix(A, n_rows1, n_cols1);

	cout << endl << "-Matrix 2 generation-" << endl;
	cout << "Inpute rows, columns, density (percentage) of non-zero entries and percentage of negative numbers" << endl
		<< "The numbers must be equal to the number of columns (rows) or rows (columns) of the first matrix." << endl;
	cin >> n_rows2 >> n_cols2 >> nz2 >> nz_neg2;

	B = create_rand_smatrix(n_rows2, n_cols2, nz2, nz_neg2);
	print_rand_smatrix(B, n_rows2, n_cols2);
	compressed_matrix(B, n_rows2, n_cols2);


	getchar();
	getchar();

	return 0;
}

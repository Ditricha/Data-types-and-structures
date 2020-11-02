#include <iostream>

#include <vector>
#include <string>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int** create_rand_smatrix(int nrow, int ncol, int density_percent, int negative_percent);
void print_rand_smatrix(int **matrix, int nrow, int ncol);
void compressing(int **matrix, int nrow, int ncol);


int main()
{
	int **A;
	int n_rows1, n_cols1;
	int nz1, nz_neg1;

	int **B;
	int n_rows2, n_cols2;
	int nz2, nz_neg2;

	cout << "-Matrix 1 generation-" << endl;
	cout << "Inpute rows, columns, density (percentage) of non-zero entries and percentage of negative numbers" << endl;
	cin >> n_rows1 >> n_cols1 >> nz1 >> nz_neg1;

	A = create_rand_smatrix(n_rows1, n_cols1, nz1, nz_neg1);
	print_rand_smatrix(A, n_rows1, n_cols1);
	cout << endl;
	compressing(A, n_rows1, n_cols1);

	cout << endl << endl << "-Matrix 2 generation-" << endl;
	cout << "Inpute rows, columns, density (percentage) of non-zero entries and percentage of negative numbers" << endl
		<< "The numbers must be equal to the number of columns (rows) or rows (columns) of the first matrix." << endl;
	cin >> n_rows2 >> n_cols2 >> nz2 >> nz_neg2;

	B = create_rand_smatrix(n_rows2, n_cols2, nz2, nz_neg2);
	print_rand_smatrix(B, n_rows2, n_cols2);
	cout << endl;
	compressing(B, n_rows2, n_cols2);

	getchar();
	getchar();

	return 0;
}

int** create_rand_smatrix(int nrow, int ncol, int density_percent, int negative_percent)
{
	int negative = 0;
	int total_num = nrow * ncol;
	int num_nz = total_num * density_percent / 100;
	int neg_nz = total_num * negative_percent / 100;
	int pos_nz = num_nz - neg_nz;
	int i;
	
	int **matrix = new int*[nrow];
	for (i = 0; i < nrow; i++)
		matrix[i] = new int[ncol];
	for (j = 0; j < nrow; j++)
		for (k = 0; k < ncol; k++)
			matrix[j][k] = 0;

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

void print_rand_smatrix(int **matrix, int nrow, int ncol)
{
	cout << endl << "-Random matrix-" << endl;
	for (int i = 0; i < nrow; i++) {
		for (int j = 0; j < ncol; j++) {
			cout.width(3);
			cout << matrix[i][j] << "  ";
		}
		cout << endl;
	}
}

void compressing(int **matrix, int nrow, int ncol)
{
	int i, j;
	vector<int> AN;
	vector<int> JA;
	vector<int> JR;
	
	for (int i = 0; i < nrow; i++) {
			JR.push_back(AN.size());
			for (int j = 0; j < ncol; j++) {
				if (matrix[i][j] != 0) {
					AN.push_back(matrix[i][j]);
					JA.push_back(j);
				}
			}
		}
		JR.push_back(AN.size());

		if (AN.size() > 0 && JR.size() > 0 && JA.size() > 0) {
			cout << "AN: [";
			for (int point : AN) {
				cout.width(3);
				cout << point << " ";
			}
			cout << "]" << endl;
			cout << "JA: [";
			for (int point : JA) {
				cout.width(3);
				cout << point << " ";
			}
			cout << "]" << endl;
			cout << "JR: [";
			for (int point : JR) {
				cout.width(3);
				cout << point << " ";
			}
			cout << "]" << endl;
		}
		else {
			cout << "NULL" << endl;
		}
}

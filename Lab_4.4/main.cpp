#include "pch.h"
#include <iostream>
#include "Header.h"

using namespace std;

CRS Addition(CRS&, CRS&);
CRS Multiplication(CRS& matrix1, CRS& matrix2);
void Print_result(CRS&, CRS&, CRS&, CRS&);

int main()
{
	int row; int column;
	int nz, nz_neg;

	cout << "-Matrix 1 generation-" << endl;
	cout << "Inpute rows, columns, density (percentage) of non-zero entries and percentage of negative numbers" << endl;
	cin >> row >> column >> nz >> nz_neg;
	CRS matrix1(row, column, nz, nz_neg);
	
	cout << endl << endl << "-Matrix 2 generation-" << endl;
	cout << "Inpute rows, columns, density (percentage) of non-zero entries and percentage of negative numbers" << endl
		<< "The numbers must be equal to the number of columns (rows) or rows (columns) of the first matrix." << endl;
	cin >> row >> column >> nz >> nz_neg;
	CRS matrix2(row, column, nz, nz_neg);

	matrix1.Compressing();
	matrix2.Compressing();

	CRS Result_1 = Addition(matrix1, matrix2);
	Result_1.Decompressing();

	CRS Result_2 = Multiplication(matrix1, matrix2);
	Result_2.Decompressing();

	Print_result(matrix1, matrix2, Result_1, Result_2);

	getchar();
	getchar();
	return 0;
}
CRS Addition(CRS& matrix1, CRS& matrix2)
{
	if (matrix1.Rows() == matrix2.Rows() && matrix1.Columns() == matrix2.Columns()) {
		int row = matrix1.Rows();
		int column = matrix1.Columns();
		CRS Res(row, column);
		int* ColumnsTemp = new int[column];
		
		for (int i = 0; i < row; i++) {
			for (int n = 0; n < column; n++)
				ColumnsTemp[n] = 0;
			Res.JR.push_back(Res.AN.size());
			for (int j = matrix1.JR[i]; j < matrix1.JR[i + 1]; j++)
				ColumnsTemp[matrix1.JA[j]] += matrix1.AN[j];
			for (int k = matrix2.JR[i]; k < matrix2.JR[i + 1]; k++)
				ColumnsTemp[matrix2.JA[k]] += matrix2.AN[k];
			for (int l = 0; l < column; l++)
				if (ColumnsTemp[l] != 0) {
					Res.AN.push_back(ColumnsTemp[l]);
					Res.JA.push_back(l);
				}
		}
		Res.JR.push_back(Res.AN.size());
		delete ColumnsTemp;
		return Res;
	}
	else {
		cout << "Error: not possible to execute addition function." << endl;
		exit(-1);
	}
}

CRS Multiplication(CRS& matrix1, CRS& matrix2)
{
	if (matrix1.Rows() == matrix2.Columns() && matrix1.Columns() == matrix2.Rows()) {
		int row = matrix1.Columns();
		int column = matrix2.Rows();
		CRS Res(row, column);
		int* ColumnsTemp = new int[column];

		Res.JR.push_back(Res.AN.size());
		for (int i = 0; i < matrix1.Rows; i++) {
			for (int j = 0; j < matrix2.Columns; j++) {
				ColumnsTemp = 0;
				for (int k = matrix1.JR[i]; k < matrix1.JR[i + 1]; k++) 
					for (int l = matrix2.JR[j]; l < matrix2.JR[j + 1]; l++) {
						if (matrix1.JA[k] == matrix2.JA[l]) {
							ColumnsTemp += matrix1.AN[k] * matrix2.AN[l];
							Res.AN.push_back(ColumnsTemp[l]);
							Res.JA.push_back(l);
						}
					}
			}
		}
		Res.JR.push_back(Res.AN.size());
		delete ColumnsTemp;
		return Res;
	}
	else {
		cout << "Error: the matrices cannot be multiplied due to the wrong dimensions" << endl;
		exit(-1);
	}
}

void Print_result(CRS& matrix1, CRS& matrix2, CRS& result_1, CRS& result_2)
{
	cout << "-Compressed matrix 1-" << endl;
	matrix1.Show_Compressing();

	cout << endl << endl << "-Compressed matrix 2-" << endl;
	matrix2.Show_Compressing();
	
	cout << endl << endl << "-Result of addition-" << endl;
	result_1.Show_Matrix();

	cout << endl << endl << "-Result of multiplication-" << endl;
	result_2.Show_Matrix();
}

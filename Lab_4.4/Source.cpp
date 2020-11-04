#include "pch.h"
#include <iostream>
#include <time.h>
#include "Header.h"

using namespace std;

CRS::CRS()
{
	Matrix = nullptr;
	rows = 0;
	columns = 0;
}

CRS::CRS(int row, int column)
{
	rows = row; columns = column;

	Matrix = new int*[rows];
	for (int i = 0; i < rows; i++)
		Matrix[i] = new int[columns];

	for (int j = 0; j < rows; j++)
		for (int k = 0; k < columns; k++)
			Matrix[j][k] = 0;
}

CRS::CRS(int row, int column, int density_percent, int negative_percent)
{
	rows = row; columns = column;

	/*Matrix = new int*[rows];
	for (int i = 0; i < rows; i++)
		Matrix[i] = new int[columns];

	for (int j = 0; j < rows; j++)
		for (int k = 0; k < columns; k++)
			Matrix[j][k] = 0;*/

	int negative = 0;
	int total_num = rows * columns;
	int num_nz = total_num * density_percent / 100;
	int neg_nz = total_num * negative_percent / 100;
	int pos_nz = num_nz - neg_nz;
	int i, j, k;

	Matrix = new int*[rows];
	for (i = 0; i < rows; i++)
		Matrix[i] = new int[columns];
	for (j = 0; j < rows; j++)
		for (k = 0; k < columns; k++)
			Matrix[j][k] = 0;

	srand(time(0));

	for (i = 0; i < num_nz; i++) {
		int row = rand() % rows;
		int col = rand() % columns;
		if (Matrix[row][column] != 0) {
			i = i - 1;
			continue;
		}
		else {
			if (negative < neg_nz) {
				Matrix[row][column] = (-1) * (rand() % 100 + 1);
				negative++;
			}
			else
				Matrix[row][column] = (rand() % 100 + 1);
		}
	}
	//return matrix;
}

CRS::CRS(const CRS& other)
{
	rows = other.rows; columns = other.columns;
	if (other.Matrix != nullptr) {
		Matrix = new int*[rows];
		for (int i = 0; i < rows; i++)
			Matrix[i] = new int[columns];

		for (int j = 0; j < rows; j++)
			for (int k = 0; k < columns; k++)
				Matrix[j][k] = other.Matrix[j][k];
	}
	AN = other.AN; JR = other.JR; JA = other.JA;
}

CRS& CRS::operator=(const CRS& other)
{
	if (this == &other)
		return *this;
	if (Matrix != nullptr) {
		for (int i = 0; i < rows; i++)
			delete Matrix[i];
		delete Matrix;
	}
	rows = other.rows; columns = other.columns;
	Matrix = new int*[rows];
	for (int i = 0; i < rows; i++)
		Matrix[i] = new int[columns];
	for (int j = 0; j < rows; j++)
		for (int k = 0; k < columns; k++)
			Matrix[j][k] = other.Matrix[j][k];
	AN = other.AN; JR = other.JR; JA = other.JA;
}

void CRS::Add(int value, int row, int column)
{
	Matrix[row][column] = value;
}

int CRS::Rows()
{
	return rows;
}

int CRS::Columns()
{
	return columns;
}

void CRS::Show_Matrix()
{
	if (Matrix != nullptr) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++)
				cout << Matrix[i][j] << " ";
			cout << endl;
		}
	}
	else {
		cout << "NULL" << endl;
	}
}

void CRS::Show_Compressing()
{
	if (AN.size() > 0 && JR.size() > 0 && JA.size() > 0) {
		cout << "AN: [";
		for (int point : AN)
			cout << point << " ";
		cout << "]" << endl;
		cout << "JA: [";
		for (int point : JA)
			cout << point << " ";
		cout << "]" << endl;
		cout << "JR: [";
		for (int point : JR)
			cout << point << " ";
		cout << "]" << endl;
	}
	else {
		cout << "NULL" << endl;
	}
}

void CRS::Compressing()
{
	for (int i = 0; i < rows; i++) {
		JR.push_back(AN.size());
		for (int j = 0; j < columns; j++) {
			if (Matrix[i][j] != 0) {
				AN.push_back(Matrix[i][j]);
				JA.push_back(j);
			}
		}
	}
	JR.push_back(AN.size());
}

void CRS::Decompressing()
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			Matrix[i][j] = 0;
	int index = 0;
	for (int i = 0; i < JR.size() - 1; i++) {
		for (int j = JR[i]; j < JR[i + 1]; j++)
			Matrix[index][JA[j]] = AN[j];
		index++;
	}
}
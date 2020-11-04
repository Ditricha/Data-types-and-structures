#pragma once
#include "pch.h"
#include <iostream>
#include <vector>
using namespace std;

class CRS {
private:
	int rows;
	int columns;
	int **Matrix;
public:
	vector<int> AN;
	vector<int> JA;
	vector<int> JR;

	CRS();
	CRS(int, int);
	CRS(int, int, int, int);
	CRS(const CRS&);
	CRS& operator=(const CRS&);
	void Compressing();
	void Decompressing();
	void Add(int, int, int);
	void Show_Matrix(void);
	void Show_Compressing(void);
	int Rows();
	int Columns();
	~CRS() {
		if (Matrix != nullptr) {
			for (int i = 0; i < rows; i++)
				delete Matrix[i];
			delete Matrix;
		}
	}
};

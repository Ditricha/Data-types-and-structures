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
	
	//Ещё я буду признательна, если подскажешь, как выделить память для матрицы правильно.
	//Потому что сейчас работает только сишное выделение =_=
	/*int **matrix = new int* [nrow];
	for (i = 0; i < nrow; i++)
		matrix[i] = new int [ncol];*/
	
	int **matrix = (int**)calloc(nrow, sizeof(int));
	for (i = 0; i < nrow; i++)
		matrix[i] = (int*)calloc(ncol, sizeof(int));

	srand(time(0));

	// Random matrix generation
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

//Я хочу, чтобы функция принимала матрицу в "обычном" виде, а возвращала в "сжатом".
//Чтобы после эти изменённые сжатые матрицы мможно было использовать для сложения и тэ дэ.
//Пока удалось только так сделать.
int** compressed_matrix(int **matrix, int nrows, int ncols)
{
	int i, j, col = 0, num = 0;
	int JA[100], JR[100];

	int **new_matrix = (int**)calloc(nrows, sizeof(int));
	for (i = 0; i < nrows; i++)
		new_matrix[i] = (int*)calloc(ncols, sizeof(int));

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

	return new_matrix;
}


//int** sum_matrix(int **matrix_1, int **matrix_2)
//{
//	/*int **matrix = new int* [nrow];
//	for (i = 0; i < nrow; i++)
//		matrix[i] = new int [ncol];*/
//
//	int **matrix_n = (int**)calloc(nrow, sizeof(int));
//	for (i = 0; i < nrow; i++)
//		matrix[i] = (int*)calloc(ncol, sizeof(int));
//	
//	Matrix ans = new Matrix(n, m);
//	//сколько элементов уже прошли в конечном массиве
//	int sum_length = 0;
//	//сколько было в старой паре массивов
//	int old_mas1_count = 0;
//	int old_mas2_count = 0;
//	//сколько элементов прошли в массиве
//	int cols1_done = 0;
//	int cols2_done = 0;
//	for (int i = 0; i < m1.pointer.Count - 1; i++)
//	{
//		//добавляем текущую строку
//		ans.pointer.Add(ans.cols.Count);
//		//два массива для каждой строки матрицы
//		List<int> mas1 = new List<int>();
//		List<int> mas2 = new List<int>();
//		for (int j = m1.pointer[i]; j < m1.pointer[i + 1]; j++)
//			mas1.Add(m1.cols[j]);
//		for (int j = m2.pointer[i]; j < m2.pointer[i + 1]; j++)
//			mas2.Add(m2.cols[j]);
//
//		//выбираем длинный массив и записываем его в ans
//		//mas1 - должен быть длиннее
//		bool swap = false;
//		if (mas2.Count > mas1.Count)
//		{
//			List<int> buff = mas1;
//			mas1 = mas2;
//			mas2 = buff;
//			swap = true;
//		}
//
//		//записываем длинный массив в ответ
//		for (int j = 0; j < mas1.Count; j++)
//		{
//			ans.cols.Add(mas1[j]);
//			if (!swap)
//				ans.values.Add(m1.values[cols1_done + j]);
//			else
//				ans.values.Add(m2.values[cols2_done + j]);
//		}
//		//проводилось ли в строке только сложение
//		bool onlySum = true;
//		//добавляем в первый массив элементы второго массива, если таких нет там и складываем с существующими, если есть
//		for (int j = 0; j < mas2.Count; j++)
//		{
//
//			//есть ли текущий элемент второго в ответе
//			bool IS = false;
//			int pos = 0;
//			for (int k = sum_length; k < ans.cols.Count && !IS; k++)
//				if (mas2[j] == ans.cols[k])
//				{
//					IS = true;
//					pos = k;
//				}
//			//если нет, вставляем его по возрастанию
//			if (!IS)
//			{
//				int k;
//				for (k = sum_length; k < ans.cols.Count; k++)
//					if (ans.cols[k] > mas2[j])
//						break;
//				//вставляем индекс
//				ans.cols.Insert(k, mas2[j]);
//				//если была перестановка, вставляем из первого, иначе из второго
//				if (swap)
//					ans.values.Insert(k, m1.values[cols1_done + j]);
//				else
//					ans.values.Insert(k, m2.values[cols2_done + j]);
//				onlySum = false;
//			}
//			else //если да, складываем value
//			{
//				if (swap)
//					ans.values[pos] += m1.values[cols1_done + j];
//				else
//					ans.values[pos] += m2.values[cols2_done + j];
//			}
//		}
//		sum_length = (mas1.Count == mas2.Count && !onlySum) ? sum_length + 2 * mas1.Count : sum_length + mas1.Count;
//		if (swap)
//		{
//			old_mas1_count = mas2.Count;
//			old_mas2_count = mas1.Count;
//			cols1_done += mas2.Count;
//			cols2_done += mas1.Count;
//		}
//		else
//		{
//			old_mas1_count = mas1.Count;
//			old_mas2_count = mas2.Count;
//			cols1_done += mas1.Count;
//			cols2_done += mas2.Count;
//		}
//	}
//	ans.pointer.Add(ans.cols.Count);
//
//	return ans;
//}


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

	A = compressed_matrix(A, n_rows1, n_cols1);

	cout << endl << "-Matrix 2 generation-" << endl;
	cout << "Inpute rows, columns, density (percentage) of non-zero entries and percentage of negative numbers" << endl
		<< "The numbers must be equal to the number of columns (rows) or rows (columns) of the first matrix." << endl;
	cin >> n_rows2 >> n_cols2 >> nz2 >> nz_neg2;

	B = create_rand_smatrix(n_rows2, n_cols2, nz2, nz_neg2);
	print_rand_smatrix(B, n_rows2, n_cols2);

	B = compressed_matrix(B, n_rows2, n_cols2);


	getchar();
	getchar();

	return 0;
}



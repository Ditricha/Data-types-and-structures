// Lab_3.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <time.h>
#include <locale.h>

using std::cout;

using namespace std;


#define ALPHA	1			
#define BETA	3			
#define T_MAX	100			
#define M		20			
#define p		0.5			
/*#define SIZE 1000000*/		

struct ANT_TYPE {
	int number; 
	double length; 
	int *array; 
};

//struct BRUTE_TYPE {
//	int number;
//	double length;
//	int *array;
//};


//int ReadSize(int**, int );
//int ReadMatrix(int**,  int );
//double Probability(int, ANT_TYPE, double **, double **, int);
//void InitializationAnts(ANT_TYPE[M], int, int);
//void InitializationLength(double **, double **, int);
//void GenerationPheromon(double **, int);
//void Pheromon(ANT_TYPE[M], double, double **, int);
//ANT_TYPE AntColony(double **, int, int, int);
//void PrintResult(ANT_TYPE);

//#define V 9
int** graph; //graph for city relationship
int n;
//int size;

vector<vector<int>> path;

int travlling_salesman_problem()
{
	int i;
	// store all vertex apart from source vertex
	vector<int> path;
	for (i = 1; i < n; i++)
		path.push_back(i);

	// store minimum weight Hamiltonian Cycle.
	int min_path = INT_MAX;
	vector<int> final_path;
	do {
		int node = 0;
		// store current Path weight(cost)
		int current_pathweight = 0;

		// compute current path weight
		for (i = 0; i < path.size(); i++) {
			current_pathweight += graph[node][path[i]];
			node = path[i];
		}
		current_pathweight += graph[node][0];

		//if (min_path > current_pathweight) {
		//	final_path = path;  //maybe has problem
		//	min_path = current_pathweight;
		//}

		// update minimum
		min_path = min(min_path, current_pathweight);

	} while (next_permutation(path.begin(), path.end()));

	return min_path;
}

//int travllingSalesmanProblem()
//{
//	BRUTE_TYPE path;
//	path.number = 0;
//	path.length = -1;
//	path.array = (int *)malloc(sizeof(int) * n);
//	int i;
//	// store all vertex apart from source vertex
//	vector<int> path;
//	for (i = 1; i < n; i++)
//			path.push_back(i);
//
//	// store minimum weight Hamiltonian Cycle.
//	int min_path = INT_MAX;
//	vector<int> final_path;
//	do {
//		int node = 0;
//		// store current Path weight(cost)
//		int current_pathweight = 0;
//
//		// compute current path weight
//		for (i = 0; i < path.size(); i++) {
//			current_pathweight += graph[node][path[i]];
//			node = path[i];
//		}
//		current_pathweight += graph[node][0];
//		
//		//if (min_path > current_pathweight) {
//		//	final_path = path;  //maybe has problem
//		//	min_path = current_pathweight;
//		//}
//
//		// update minimum
//		min_path = min(min_path, current_pathweight);
//
//	} while (next_permutation(path.begin(), path.end()));
//
//	return min_path;
//}


//void read_file() {
//
//	int i, j;
//	ifstream fin;
//	fin.open("graph.txt");
//	
//	if (!fin.is_open()) {
//		cout << "Error during the opening.";
//	}
//		fin >> n;  //n for city number
//		graph = new int*[n];
//		for (i = 0; i < n; ++i)  //initial each line
//			graph[i] = new int[n];
//
//		while (!fin.eof()) {
//			for (i = 0; i < n; i++)  {     //input each line
//				for (j = 0; j < n; j++)
//					fin >> graph[i][j];
//			}
//
//			for (i = 0; i < n; i++) {     //input each line
//				for (j = 0; j < n; j++) {
//					cout.width(3);
//					cout << graph[i][j] << " ";
//				}
//				cout << endl;
//			}
//		}
//		fin.close();
//}

//int read_size()
//{
//	ifstream fin;
//	fin.open("graph.txt");
//
//	if (!fin.is_open()) {
//		cout << "Error during the opening.";
//		return 0;
//	}
//	fin >> n;
//	return n;
//}
	
	
int read_matrix()
{
	int i, j;

	ifstream fin;
	fin.open("graph.txt");

	if (!fin.is_open()) {
		cout << "Error during the opening.";
		return 0;
	}
	fin >> n;  //n for city number
	graph = new int*[n];
	for (i = 0; i < n; ++i)  //initial each line
		graph[i] = new int[n];

	while (!fin.eof()) {
		for (i = 0; i < n; i++) {     //input each line
			for (j = 0; j < n; j++)
				fin >> graph[i][j];
		}

		for (i = 0; i < n; i++) {     //input each line
			for (j = 0; j < n; j++) {
				cout.width(3);
				cout << graph[i][j] << " ";
			}
			cout << endl;
		}
	}
	fin.close();
	return n;
}

double probability(int to, ANT_TYPE ant, double **tao, double **epsilon, int n)
{
	double res = 0.0, sum = 0.0;
	int from = ant.array[ant.number - 1];
	/* Если вершина уже посещена, возвращаем 0 */
	for (int i = 0; i < ant.number; ++i) {
		if (to == ant.array[i]) {
			return 0.0;
		}
	}
	/* Подсчет суммы в знаминателе */
	for (int j = 0; j < n; ++j) {
		int flag = 1;
		/* Проверка: посещал ли муравей j вершину? */
		for (int i = 0; i < ant.number; ++i) {
			if (j == ant.array[i]) {
				flag = 0;
			}
		}
		/* Если нет, тогда прибавляем к общей сумме */
		if (flag && from >= 0) {
			sum += pow(tao[from][j], ALPHA) * pow(epsilon[from][j], BETA);
		}
	}
	if (from >= 0) {
		/* Возврат значения вероятности */
		res = pow(tao[from][to], ALPHA) * pow(epsilon[from][to], BETA) / sum;
	}
	return res;
}
                                                             
void initialization_length(double **epsilon, int **graph, int n) {
	for (int i = 0; i < n; ++i) {
		/* Создание двумерного динамического массива */
		epsilon[i] = (double *)malloc(sizeof(double) * n);
		for (int j = 0; j < n; ++j) {
			/* Если текущее ребро не является петлей */
			if (i != j) {
				epsilon[i][j] = 1.0 / graph[i][j];
			}
		}
	}
}
                                                              
void generation_pheromon(double **tao, int n) {
	/* Генерация феромона */
	srand((unsigned)time(NULL));
	for (int i = 0; i < n; ++i) {
		/* Создание двумерного динамического массива */
		tao[i] = (double *)malloc(sizeof(double) * n);
		for (int j = 0; j < n; ++j) {
			/* Случайное заполнение массива, значения которых от 1 до size */
			tao[i][j] = 1 + (n * rand() / RAND_MAX);
		}
	}
}
                                                          
void initialization_ants(ANT_TYPE ants[M], int n, int city_1) {
	/* Цикл по всем муравьям */
	for (int k = 0; k < M; ++k) {
		ants[k].number = 0;
		ants[k].length = 0.0;
		ants[k].array = (int *)malloc(sizeof(int) * n);
		/* Размещение колонии в начальной вершине */
		ants[k].array[ants[k].number++] = city_1;
	}
}
                                                            
void pheromon(ANT_TYPE ants[M], double Q, double **tao, int k) {
	for (int i = 0; i < ants[k].number - 1; ++i) {
		/* Объявление и инициализация переменных */
		int from = ants[k].array[i % ants[k].number];
		int to = ants[k].array[(i + 1) % ants[k].number];
		tao[from][to] += Q / ants[k].length;
		tao[to][from] = tao[from][to];
	}
}

ANT_TYPE ant_colony(int **graph, int n, int city_1, int city_2)
{
	/* Начальная инициализация данных о лучшем маршруте */
	ANT_TYPE path;
	path.number = 0;
	path.length = -1;
	path.array = (int *)malloc(sizeof(int) * n);
	double Q = 1.0;
	ANT_TYPE ants[M];
	/* Инициализация данных о расстоянии и количестве феромона */
	double **epsilon = NULL, **tao = NULL;
	/* Создание динамических массивов */
	epsilon = (double **)malloc(sizeof(double *) * n);
	tao = (double **)malloc(sizeof(double *) * n);

	/* Вызов функции для инициализации начального пути */
	initialization_length(epsilon, graph, n);

	/* Вызов функции для генерации феромона */
	generation_pheromon(tao, n);

	/* Вызов функции для размещения всех муравьем в начальном городе */
	initialization_ants(ants, n, city_1);

	/* Основной цикл муравьиного алгоритма */
	for (int t = 0; t < T_MAX; ++t) {
		/* Цикл по муравьям */
		for (int k = 0; k < M; ++k) {
			/* Локальный поиск маршрута для k-го муравья */
			do {
				int J_max = -1;
				double P_max = 0.0;
				for (int j = 0; j < n; ++j) {
					/* Проверка вероятности перехода в вершину j */
					if (ants[k].array[ants[k].number - 1] != j) {
						/* Вызов функции для подсчета вероятности */
						double P = probability(j, ants[k], tao, epsilon, n);
						if (P != 0 && P >= P_max) {
							P_max = P; // запоминаем большую вероятность
							J_max = j; // запоминаем номер города, которому соответствует эта вероятность
						}
					}
				}
				if (J_max >= 0) {
					/* Подсчёт пройденного пути и составление маршрута */
					ants[k].length += graph[ants[k].array[ants[k].number - 1]][J_max]; // текущей длине приравнивается значение из D
					ants[k].array[ants[k].number++] = J_max;
				}
			} while (ants[k].array[ants[k].number - 1] != city_2); // пока не достигнута конечная вершина

			/* Оставляем след феромона от колоний */
			pheromon(ants, Q, tao, k);

			/* Проверка: нашли лучшее решение? */
			if (ants[k].length < path.length || path.length < 0) { // т.е. длина построенного маршрута меньше той, которая дана изнчально задана в графе
				/* Переприсваивание номеров вершин, длин и маршрута */
				path.number = ants[k].number;
				path.length = ants[k].length;
				for (int i = 0; i < path.number; ++i) {
					path.array[i] = ants[k].array[i];
				}
			}
			/* Обновление муравьев */
			ants[k].number = 1;
			ants[k].length = 0.0;
		}
		/* Цикл по ребрам */
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				/* Обновление феромона для ребра (i, j) с учетом его испарения */
				if (i != j) {
					tao[i][j] *= (1 - p);
				}
			}
		}
	}

	/* Освобождение памяти */
	free(epsilon);
	free(tao);

	/* Возврат кратчайшего маршрута */
	return path;
}

//void PrintResult(ANT_TYPE path) {
//	/*cout << "Path: " << ++path.array[0];
//	for (int i = 1; i < path.number; ++i) {
//		cout << " -> " << ++path.array[i];
//	}*/
//	cout << endl << "Path length: " << path.length << endl;
//}

int main()
{
	/*int **graph;*/
	/*int size = 0;*/
	int num, city_1 = 0, city_2 = 0;
	/*int i, j, num;*/
	
	/*int graph[][V] = { { 0, 9, 20, 95, 10, 37, 55, 12, 82 },
					   { 49, 0, 34, 2, 50, 43, 78, 56, 34 },
					   { 69, 26, 0, 6, 12, 61, 30, 56, 76 },
					   { 38, 29, 37, 0, 88, 25, 81, 30, 9 },
					   { 80, 45, 4, 12, 0, 1, 61, 36, 79 },
					   { 81, 80, 95, 8, 83, 0, 9, 79, 10 },
					   { 13, 30, 75, 52, 43, 93, 0, 37, 43 },
					   { 20, 72, 92, 48, 52, 45, 61, 0, 58 },
					   { 13, 29, 64, 2, 56, 15, 46, 87, 0 } };
	*/
	/*read_file();*/
	/*size = read_size(graph, size);*/
	read_matrix();

	cout << endl << "Choose the number of an algorithm:" << endl;
	cout << "1. Brute force algorithm;" << endl << "2. Ant colony algorithm." << endl;
	cin >> num;

	if (num == 1) {
		int s = 0;
		cout << endl << "Path: " << travlling_salesman_problem() << endl;
	}
	else if (num == 2) {
		while (city_1 < 1 || city_1 > n) {
			cout << endl << "Enter the starting vertex from 1 to " << n << ": ";
			cin >> city_1;
		}
		while (city_2 < 1 || city_2 > n || city_2 == city_1) {
			cout << "Enter the finite vertex from 1 to " << n << ", except " << city_1 << ": ";
			cin >> city_2;
		}

		ANT_TYPE path = ant_colony(graph, n, --city_1, --city_2);

		cout << endl << "Path: " << path.length << endl;
		/* Вывод результата на экран */
		/*PrintResult(way);*/
	}
	/* Освобождение памяти */
	delete[] graph;

	/* Задержка сообщения на экране */
	getchar();
	
	return 0;
}



#include <list>

list <double> params = { 0.001, 0.25, 0.5, 0.75, 1.0 };

list <int> tmax = { 50, 100, 150, 200, 500 };
#define PAR = 4;

int main() {
	int row; int col;
	row = col = 4;
	int row = 4;
	int col = 4;
	/*int PAR = 4;*/

	int **result[4][4];
	**result = new int *[4];
		for (int i = 0; i < 4; i++) {
			**result[i] = new int[4];
		}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			**result[i][j] = (i + 1) * 10 + (j + 1);
		}
	}

	// уничтожение
	for (int i = 0; i < 4; i++) {
		delete[] resulting_matrix[i];
	}
	delete[] resulting_matrix;
}
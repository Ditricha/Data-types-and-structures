#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <time.h>

using std::cout;

using namespace std;


#define ALPHA	1			
#define BETA	3			
#define TMAX	100			
#define M	20			
#define p	0.5					

struct ANT_TYPE {
	int number; 
	double length; 
	int *array; 
};


//int travlling_salesman_problem(int **, int)
//int read_matrix(int **,  int);
//double probability(int, ANT_TYPE, double **, double **, int);
//void initialization_ants(ANT_TYPE[M], int, int);
//void initialization_length(double **, double **, int);
//void generation_pheromon(double **, int);
//void pheromon(ANT_TYPE[M], double, double **, int);
//ANT_TYPE AntColony(double **, int, int, int);
//void print_result(ANT_TYPE);


int **graph;
int n;

vector<vector<int>> path;

int travlling_salesman_problem()
{
	int i, node, current_pathweight;
	vector<int> path;
	for (i = 1; i < n; i++)
		path.push_back(i);

	int min_path = INT_MAX;
	vector<int> final_path;
	do {
		node = 0;
		current_pathweight = 0;

		for (i = 0; i < path.size(); i++) {
			current_pathweight += graph[node][path[i]];
			node = path[i];
		}
		current_pathweight += graph[node][0];

		min_path = min(min_path, current_pathweight);

	} while (next_permutation(path.begin(), path.end()));

	return min_path;
}
		
int read_matrix()
{
	int i, j;

	ifstream fin;
	fin.open("graph.txt");

	if (!fin.is_open()) {
		cout << "Error during the opening.";
		return 0;
	}
	fin >> n;
	graph = new int*[n];
	for (i = 0; i < n; i++)
		graph[i] = new int[n];

	while (!fin.eof()) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++)
				fin >> graph[i][j];
		}

		for (i = 0; i < n; i++) {
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
	int i, j, flag;
	double res = 0.0, sum = 0.0;
	int from = ant.array[ant.number - 1];

	for (i = 0; i < ant.number; i++) {
		if (to == ant.array[i]) {
			return 0.0;
		}
	}

	for (j = 0; j < n; j++) {
		flag = 1;
		for (i = 0; i < ant.number; i++) {
			if (j == ant.array[i]) {
				flag = 0;
			}
		}
		if (flag && from >= 0) {
			sum += pow(tao[from][j], ALPHA) * pow(epsilon[from][j], BETA);
		}
	}
	if (from >= 0) {
		res = pow(tao[from][to], ALPHA) * pow(epsilon[from][to], BETA) / sum;
	}
	return res;
}
                                                             
void initialization_length(double **epsilon, int **graph, int n)
{
	int i, j;
	for (i = 0; i < n; i++) {
		epsilon[i] = (double *)malloc(sizeof(double) * n);
		for (j = 0; j < n; j++) {
			if (i != j) {
				epsilon[i][j] = 1.0 / graph[i][j];
			}
		}
	}
}
                                                              
void generation_pheromon(double **tao, int n)
{
	int i, j;
	srand((unsigned)time(NULL));
	for (i = 0; i < n; i++) {
		tao[i] = (double *)malloc(sizeof(double) * n);
		for (j = 0; j < n; j++) {
			tao[i][j] = 1 + (n * rand() / RAND_MAX);
		}
	}
}
                                                          
void initialization_ants(ANT_TYPE ants[M], int n, int city_1)
{
	for (int k = 0; k < M; k++) {
		ants[k].number = 0;
		ants[k].length = 0.0;
		ants[k].array = (int *)malloc(sizeof(int) * n);
		ants[k].array[ants[k].number++] = city_1;
	}
}
                                                            
void pheromon(ANT_TYPE ants[M], double Q, double **tao, int k)
{
	for (int i = 0; i < ants[k].number - 1; i++) {
		int from = ants[k].array[i % ants[k].number];
		int to = ants[k].array[(i + 1) % ants[k].number];
		tao[from][to] += Q / ants[k].length;
		tao[to][from] = tao[from][to];
	}
}

ANT_TYPE ant_colony(int **graph, int n, int city_1, int city_2)
{
	int t, k, i, j;
	int J_max; 
	double P_max, double P;
	
	ANT_TYPE path;
	path.number = 0;
	path.length = -1;
	path.array = (int *)malloc(sizeof(int) * n);
	double Q = 1.0;
	ANT_TYPE ants[M];

	double **epsilon = NULL;
	double **tao = NULL;
	epsilon = (double **)malloc(sizeof(double *) * n);
	tao = (double **)malloc(sizeof(double *) * n);

	initialization_length(epsilon, graph, n);

	generation_pheromon(tao, n);

	initialization_ants(ants, n, city_1);

	for (t = 0; t < TMAX; t++) {
		for (k = 0; k < M; k++) {
			do {
				J_max = -1;
				P_max = 0.0;
				for (j = 0; j < n; j++) {
					if (ants[k].array[ants[k].number - 1] != j) {
						P = probability(j, ants[k], tao, epsilon, n);
						if (P != 0 && P >= P_max) {
							P_max = P;
							J_max = j;
						}
					}
				}
				if (J_max >= 0) {
					ants[k].length += graph[ants[k].array[ants[k].number - 1]][J_max];
					ants[k].array[ants[k].number++] = J_max;
				}
			} while (ants[k].array[ants[k].number - 1] != city_2);

			pheromon(ants, Q, tao, k);

			if (ants[k].length < path.length || path.length < 0) { 
				path.number = ants[k].number;
				path.length = ants[k].length;
				for (i = 0; i < path.number; i++) {
					path.array[i] = ants[k].array[i];
				}
			}
			ants[k].number = 1;
			ants[k].length = 0.0;
		}
		for ( i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				if (i != j) {
					tao[i][j] *= (1 - p);
				}
			}
		}
	}
	free(epsilon);
	free(tao);

	return path;
}

int main()
{
	int num, s, city_1 = 0, city_2 = 0;

	read_matrix();

	cout << endl << "Choose the number of an algorithm:" << endl;
	cout << "1. Brute force algorithm;" << endl << "2. Ant colony algorithm." << endl;
	cin >> num;

	if (num == 1) {
		s = 0;
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
	}
	delete[] graph;

	getchar();
	
	return 0;
}

//попытки в параметризацию
/*#include <list>

list <double> params = { 0.001, 0.25, 0.5, 0.75, 1.0 };

list <int> tmax = { 50, 100, 150, 200, 500 };
#define PAR = 4;

int main() {
	int row; int col;
	row = col = 4;
	int row = 4;
	int col = 4;

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

	for (int i = 0; i < 4; i++) {
		delete[] resulting_matrix[i];
	}
	delete[] resulting_matrix;
}*/

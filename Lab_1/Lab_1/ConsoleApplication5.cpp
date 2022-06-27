#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX 10
#define MIN2(A, B) ((A)<(B) ? (A) : (B))
#define MIN3(A, B, C) ((A) < (B) ? MIN2(A, C) : MIN2(B, C))

int distanceLev_nr(char * str1, char * str2, int len1, int len2);
int distanceLev_r(char * str1, char * str2, int len1, int len2);
int distanceDamLev_nr(char * str1, char * str2, int len1, int len2);

double PCFreq = 0.0;
__int64 CounterStart = 0;
void StartCounter();
double GetCounter();


int main()
{
	char s1[MAX];
	char s2[MAX];

	int n;

	printf("Enter the first word:\n");
	scanf_s("%s", &s1, sizeof(s1));

	printf("Enter the second word:\n");
	scanf_s("%s", &s2, sizeof(s2));

	int l1 = strlen(s1);
	int l2 = strlen(s2);

	printf("\n1. Levenshtein distance with matrix;\n2. Recursive Levenshtein distance;\n3. Damerau-Levenshtein distance with matrix.\n");
	printf("\nChoose number of algorithm: ");
	scanf_s("%d", &n, sizeof(n));
	
	if (n == 1) {
		StartCounter();
		int dl_nr;
		for (int q = 0; q < 1000; q++) {
			dl_nr = distanceLev_nr(s1, s2, l1, l2);
		}
		printf("\nTime: %lf\n", GetCounter() / 1000.0);
		printf("Edit distance: %d\n", dl_nr);
	}
	else if (n == 2) {
		StartCounter();
		int dl_r;
		for (int q = 0; q < 1000; q++) {
			dl_r = distanceLev_r(s1, s2, l1, l2);
		}
		printf("\nTime: %lf\n", GetCounter() / 1000.0);
		printf("Edit distance: %d\n", dl_r);
	}
	else if (n == 3) {
		StartCounter();
		int ddl_nr;
		for (int q = 0; q < 1000; q++) {
			ddl_nr = distanceDamLev_nr(s1, s2, l1, l2);
		}
		printf("\nTime: %lf\n", GetCounter() / 1000.0);
		printf("Edit distance: %d\n", ddl_nr);
	}
	else
		printf("\nError: you chose uncorrect number.\n");
	
	getchar();
	getchar();
	return 0;
}

int distanceLev_nr(char * str1, char * str2, int len1, int len2)
{
	int i, j;
	int D[MAX][MAX];
	len1 = strlen(str1);
	len2 = strlen(str2);

	for (i = 0; i < len1 + 1; i++)
		D[i][0] = i;

	for (j = 0; j < len2 + 1; j++)
		D[0][j] = j;

	for (i = 1; i < len1 + 1; i++) {
		for (j = 1; j < len2 + 1; j++) {
			if (str1[i - 1] == str2[j - 1])
				D[i][j] = MIN3(D[i - 1][j - 1], D[i - 1][j] + 1, D[i][j - 1] + 1);
			else
				D[i][j] = MIN3(D[i - 1][j - 1] + 1, D[i - 1][j] + 1, D[i][j - 1] + 1);
		}
	}
	return D[len1][len2];
}

int distanceLev_r(char * str1, char * str2, int len1, int len2)
{
	int i = len1;
	int j = len2;

	if (i == 0 && j == 0)
		return 0;
	else if (i == 0 && j > 0)
		return j;
	else if (j == 0 && i > 0)
		return i;
	return MIN3((distanceLev_r(str1, str2, i, j - 1) + 1),
				(distanceLev_r(str1, str2, i - 1, j) + 1),
				(distanceLev_r(str1, str2, i - 1, j - 1) + ((str1[i - 1] == str2[j - 1]) ? 0 : 1)));
}

int distanceDamLev_nr(char * str1, char * str2, int len1, int len2)
{
	int i, j;
	int D[MAX][MAX];
	len1 = strlen(str1);
	len2 = strlen(str2);

	for (i = 0; i <= len1; i++)
		D[i][0] = i;

	for (j = 0; j <= len2; j++)
		D[0][j] = j;

	for (i = 1; i < len1 + 1; i++) {
		for (j = 1; j < len2 + 1; j++) {
			D[i][j] = MIN2(D[i - 1][j] + 1, D[i][j - 1]) + 1;
			D[i][j] = MIN2(D[i][j], D[i - 1][j - 1] + (str1[i - 1] != str2[j - 1]));

			if (i >= 2 && j >= 2 && ((str1[i - 2] + str1[i - 1]) == (str2[j - 1] + str2[j - 2]))) {
				D[i][j] = MIN2(D[i][j], D[i - 2][j - 2] + 1);
			}
		}
	}
	return D[len1][len2];
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

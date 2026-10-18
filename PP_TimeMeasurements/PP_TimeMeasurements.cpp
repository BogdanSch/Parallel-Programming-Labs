#include <iostream>
#include <random>
#include "task1.h"
#include "task2.h"
#include "task34.h"
#include "constants.h"
#include "SquareMatrix.h"

using std::random_device, std::mt19937, std::uniform_int_distribution;

static void multiplyMatrix(const int a[][MATRIX_SIZE], const int b[][MATRIX_SIZE], int result[][MATRIX_SIZE], size_t size) {
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			result[i][j] = 0;

	for (int i = 0; i < size; i++) {
		for (int k = 0; k < size; k++) {
			for (int j = 0; j < size; j++) {
				result[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

int main()
{
	// Task 1 - Year 2038 problem
	printf("TASK 1:\n");
	displayYear2038Problem();
	
	// Task 2 - Functions benchmarks
	printf("\nTASK 2:\n");
	benchmarkTimeFunctions();

	// Tasks 3 and 4
	performTasks3And4();

	// Task 5,6,7 - Matrix multiplication
	printf("\nTask 5-7:\n");
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(0, 100);

	int a0[MATRIX_SIZE][MATRIX_SIZE]{};
	int b0[MATRIX_SIZE][MATRIX_SIZE]{};
	int c0[MATRIX_SIZE][MATRIX_SIZE]{};
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			a0[i][j] = distrib(gen);
			b0[i][j] = distrib(gen);
		}
	}
	multiplyMatrix(a0, b0, c0, MATRIX_SIZE);

	//int** a1 = new int*[MATRIX_SIZE];
	//int** b1 = new int*[MATRIX_SIZE];
	//int** c1 = new int*[MATRIX_SIZE];
	//for (int i = 0; i < MATRIX_SIZE; i++) {
	//	a1[i] = new int[MATRIX_SIZE];
	//	b1[i] = new int[MATRIX_SIZE];
	//	c1[i] = new int[MATRIX_SIZE];
	//	for (int j = 0; j < MATRIX_SIZE; j++) {
	//		a1[i][j] = distrib(gen);
	//		b1[i][j] = distrib(gen);
	//	}
	//}
	//multiplyMatrix(a1, b1, c1, MATRIX_SIZE);

	//for (int i = 0; i < MATRIX_SIZE; i++) {
	//	delete[] a1[i];
	//	delete[] b1[i];
	//	delete[] c1[i];
	//}
	//delete[] a1;
	//delete[] b1;
	//delete[] c1;

	SquareMatrix a(MATRIX_SIZE);
	SquareMatrix b(MATRIX_SIZE);
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			a.set(i, j, distrib(gen));
			b.set(i, j, distrib(gen));
		}
	}
	SquareMatrix c = a * b;
}
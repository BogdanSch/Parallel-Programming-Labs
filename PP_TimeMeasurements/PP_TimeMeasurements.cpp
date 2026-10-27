#include <iostream>
#include <random>
#include "task1.h"
#include "task2.h"
#include "task34.h"
#include "constants.h"
#include "SquareMatrix.h"
#include <omp.h>

using std::random_device, std::mt19937, std::uniform_int_distribution;

template <typename T>
__declspec(noinline) static void multiplyMatrix(T** a, T** b, T** result, size_t size) {
	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			result[i][j] = 0;

	for (size_t i = 0; i < size; i++) {
		for (size_t k = 0; k < size; k++) {
			for (size_t j = 0; j < size; j++) {
				result[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

template <typename T>
__declspec(noinline) static void multiplyMatrixPerfomanceProfiler(const char* typeName, T** a, T** b, T** c, size_t matrixSize) {
	printf("%s\n", typeName);
	multiplyMatrix(a, b, c, matrixSize);
}

template <typename T>
static void timeMatrixMultiplication(const char* typeName, T** a, T** b, T** result, size_t size, int numberOfTrials = 10) {
	double minDifference = DBL_MAX;

	for (int i = 0; i < numberOfTrials; i++) {
		double start = omp_get_wtime();

		multiplyMatrix(a, b, result, size);

		double difference = omp_get_wtime() - start;
		if (minDifference > difference)
			minDifference = difference;
	}

	printf("T_normal (%s) = %.8f ms\n", typeName, minDifference * 1e3);
}

static void timeObjectMatrixMultiplication(const SquareMatrix& a, const SquareMatrix& b, int numberOfTrials = 10) {
	double minDifference = DBL_MAX;

	for (int i = 0; i < numberOfTrials; i++) {
		double start = omp_get_wtime();

		SquareMatrix result = a * b;

		double difference = omp_get_wtime() - start;
		if (minDifference > difference)
			minDifference = difference;
	}

	printf("T_object = %.8f ms\n", minDifference * 1e3);
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

	size_t matrixSize = 512;
	for (int i = 0; i < 3; i++) {
		printf("\nn=%zu:\n", matrixSize);

		int** a1 = new int* [matrixSize];
		int** b1 = new int* [matrixSize];
		int** c1 = new int* [matrixSize];
		SquareMatrix a(matrixSize);
		SquareMatrix b(matrixSize);

		for (int i = 0; i < matrixSize; i++) {
			a1[i] = new int[matrixSize];
			b1[i] = new int[matrixSize];
			c1[i] = new int[matrixSize];
			for (int j = 0; j < matrixSize; j++) {
				int firstValue = distrib(gen);
				int secondValue = distrib(gen);
				a.set(i, j, firstValue);
				b.set(i, j, secondValue);
				a1[i][j] = firstValue;
				b1[i][j] = secondValue;
			}
		}
		timeObjectMatrixMultiplication(a, b, 1);
		timeMatrixMultiplication("int", a1, b1, c1, matrixSize, 1);

		for (int i = 0; i < matrixSize; i++) {
			delete[] a1[i];
			delete[] b1[i];
			delete[] c1[i];
		}
		delete[] a1;
		delete[] b1;
		delete[] c1;

		matrixSize *= 2;
	}

	// Task 9 - Matrix multiplication (data types)
	matrixSize = 1024;
	printf("\nn=%zu:\n", matrixSize);

	__int8** a0 = new __int8* [matrixSize];
	__int8** b0 = new __int8* [matrixSize];
	__int8** c0 = new __int8* [matrixSize];

	__int16** a1 = new __int16* [matrixSize];
	__int16** b1 = new __int16* [matrixSize];
	__int16** c1 = new __int16* [matrixSize];

	__int32** a2 = new __int32* [matrixSize];
	__int32** b2 = new __int32* [matrixSize];
	__int32** c2 = new __int32* [matrixSize];

	__int64** a3 = new __int64* [matrixSize];
	__int64** b3 = new __int64* [matrixSize];
	__int64** c3 = new __int64* [matrixSize];

	float** a4 = new float* [matrixSize];
	float** b4 = new float* [matrixSize];
	float** c4 = new float* [matrixSize];

	double** a5 = new double* [matrixSize];
	double** b5 = new double* [matrixSize];
	double** c5 = new double* [matrixSize];

	for (size_t i = 0; i < matrixSize; i++) {
		a0[i] = new __int8[matrixSize];
		b0[i] = new __int8[matrixSize];
		c0[i] = new __int8[matrixSize] {};

		a1[i] = new __int16[matrixSize];
		b1[i] = new __int16[matrixSize];
		c1[i] = new __int16[matrixSize] {};

		a2[i] = new __int32[matrixSize];
		b2[i] = new __int32[matrixSize];
		c2[i] = new __int32[matrixSize] {};

		a3[i] = new __int64[matrixSize];
		b3[i] = new __int64[matrixSize];
		c3[i] = new __int64[matrixSize] {};

		a4[i] = new float[matrixSize];
		b4[i] = new float[matrixSize];
		c4[i] = new float[matrixSize] {};

		a5[i] = new double[matrixSize];
		b5[i] = new double[matrixSize];
		c5[i] = new double[matrixSize] {};

		for (size_t j = 0; j < matrixSize; j++) {
			int firstValue = distrib(gen);
			int secondValue = distrib(gen);

			a0[i][j] = static_cast<__int8>(firstValue);
			b0[i][j] = static_cast<__int8>(secondValue);

			a1[i][j] = static_cast<__int16>(firstValue);
			b1[i][j] = static_cast<__int16>(secondValue);

			a2[i][j] = static_cast<__int32>(firstValue);
			b2[i][j] = static_cast<__int32>(secondValue);

			a3[i][j] = static_cast<__int64>(firstValue);
			b3[i][j] = static_cast<__int64>(secondValue);

			a4[i][j] = static_cast<float>(firstValue);
			b4[i][j] = static_cast<float>(secondValue);

			a5[i][j] = static_cast<double>(firstValue);
			b5[i][j] = static_cast<double>(secondValue);
		}
	}

	timeMatrixMultiplication("__int8", a0, b0, c0, matrixSize, 1);
	timeMatrixMultiplication("__int16", a1, b1, c1, matrixSize, 1);
	timeMatrixMultiplication("__int32", a2, b2, c2, matrixSize, 1);
	timeMatrixMultiplication("__int64", a3, b3, c3, matrixSize, 1);
	timeMatrixMultiplication("float", a4, b4, c4, matrixSize, 1);
	timeMatrixMultiplication("double", a5, b5, c5, matrixSize, 1);

	// Task 10 - Matrix multiplication (data types) in Perfomance mode
	printf("\nTASK 10:\n");
	multiplyMatrixPerfomanceProfiler("__int8", a0, b0, c0, matrixSize);
	multiplyMatrixPerfomanceProfiler("__int16", a1, b1, c1, matrixSize);
	multiplyMatrixPerfomanceProfiler("__int32", a2, b2, c2, matrixSize);
	multiplyMatrixPerfomanceProfiler("__int64", a3, b3, c3, matrixSize);
	multiplyMatrixPerfomanceProfiler("float", a4, b4, c4, matrixSize);
	multiplyMatrixPerfomanceProfiler("double", a5, b5, c5, matrixSize);

	for (size_t i = 0; i < matrixSize; i++) {
		delete[] a0[i]; delete[] b0[i]; delete[] c0[i];
		delete[] a1[i]; delete[] b1[i]; delete[] c1[i];
		delete[] a2[i]; delete[] b2[i]; delete[] c2[i];
		delete[] a3[i]; delete[] b3[i]; delete[] c3[i];
		delete[] a4[i]; delete[] b4[i]; delete[] c4[i];
		delete[] a5[i]; delete[] b5[i]; delete[] c5[i];
	}
	delete[] a0; delete[] b0; delete[] c0;
	delete[] a1; delete[] b1; delete[] c1;
	delete[] a2; delete[] b2; delete[] c2;
	delete[] a3; delete[] b3; delete[] c3;
	delete[] a4; delete[] b4; delete[] c4;
	delete[] a5; delete[] b5; delete[] c5;
}
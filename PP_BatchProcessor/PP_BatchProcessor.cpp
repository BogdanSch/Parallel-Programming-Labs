#include <iostream>
#include <random>
#include <omp.h>
#include <cmath>

using std::random_device, std::mt19937, std::uniform_int_distribution, std::uniform_real_distribution, std::default_random_engine, std::round;

static size_t countPositiveNumbersAtEvenIndices(int* numbers, size_t size) {
	size_t count = 0;
	for (size_t i = 0; i < size; i += 2) {
		if (numbers[i] > 0)
			count++;
	}
	return count;
}
static void timeCountPositiveNumbersAtEvenIndices(const char* text, int* numbers, size_t size, int numberOfTrials = 10) {
	double minTime = DBL_MAX;

	for (int i = 0; i < numberOfTrials; i++) {
		double start = omp_get_wtime();

		countPositiveNumbersAtEvenIndices(numbers, size);

		double end = omp_get_wtime();
		double timeDifference = end - start;
		if (timeDifference < minTime)
			minTime = timeDifference;
	}
	printf("(%s): Work took %.8f seconds\n", text, minTime);
}

// Відсортована частина починається з кінця
static void sort(int* x, int n) {
	for (int p = n - 1; p >= 1; p--) {
		for (int i = 0; i < p; i++) {
			if (x[i] > x[i + 1]) {
				int temp = x[i]; 
				x[i] = x[i + 1]; 
				x[i + 1] = temp;
			}
		}
	}
}
// Кращий з точки зору передбачення, оскільки ліва частина відсортована, через що процесор менше помиляється при порівнянні
static void optimizedSort(int* x, int n) {
	for (int p = 1; p < n; p++) {
		for (int i = p - 1; i >= 0; i--) {
			if (x[i] > x[i + 1]) {
				int temp = x[i];
				x[i] = x[i + 1];
				x[i + 1] = temp;
			}
		}
	}
}
// Припинення ітерації, оскільки ліва частина відсортована
static void optimizedSortWithBreak(int* x, int n) {
	for (int p = 1; p < n; p++) {
		for (int i = p - 1; i >= 0; i--) {
			if (x[i] > x[i + 1]) {
				int temp = x[i];
				x[i] = x[i + 1];
				x[i + 1] = temp;
			}
			else {
				break;
			}
		}
	}
}
static void timeSortAlgorithms(const int* originalArray, size_t size, size_t numberOfTrials = 5) {
	int* a = new int[size];
	int* b = new int[size];
	int* c = new int[size];

	double minTimeClassic = DBL_MAX;
	for (int t = 0; t < numberOfTrials; t++) {
		for (int i = 0; i < size; i++) 
			a[i] = originalArray[i];

		double start = omp_get_wtime();
		sort(a, size);
		double end = omp_get_wtime();

		if (end - start < minTimeClassic) 
			minTimeClassic = end - start;
	}

	double minTimeOpt = DBL_MAX;
	for (int t = 0; t < numberOfTrials; t++) {
		for (int i = 0; i < size; i++) 
			b[i] = originalArray[i];

		double start = omp_get_wtime();
		optimizedSort(b, size);
		double end = omp_get_wtime();

		if (end - start < minTimeOpt) 
			minTimeOpt = end - start;
	}

	double minTimeOptWithBreak = DBL_MAX;
	for (int t = 0; t < numberOfTrials; t++) {
		for (int i = 0; i < size; i++)
			c[i] = originalArray[i];

		double start = omp_get_wtime();
		optimizedSortWithBreak(c, size);
		double end = omp_get_wtime();

		if (end - start < minTimeOptWithBreak)
			minTimeOptWithBreak = end - start;
	}

	printf("Classic Bubble Sort:   %.10f seconds\n", minTimeClassic);
	printf("Optimized Bubble Sort: %.10f seconds\n", minTimeOpt);
	printf("Optimized Bubble Sort With Break: %.10f seconds\n", minTimeOptWithBreak);

	delete[] a;
	delete[] b;
	delete[] c;
}

static void multiplyPolynomials(int* poly1, size_t size1, int* poly2, size_t size2, int* result) {
	for (size_t i = 0; i < size1 + size2 - 1; i++) 
		result[i] = 0;

	for (size_t i = 0; i < size1; i++) {
		for (size_t j = 0; j < size2; j++) {
			result[i + j] += poly1[i] * poly2[j];
		}
	}
}
static void optimizedMultiplyPolynomials(int* poly1, size_t size1, int* poly2, size_t size2, int* result) {
	for (size_t i = 0; i < size1 + size2 - 1; i++)
		result[i] = 0;

	for (size_t i = 0; i < size2; i++) {
		if (poly2[i] == 0) {
			continue;
		}
		else if (poly2[i] == 1) {
			for (size_t j = 0; j < size1; j++) {
				result[i + j] += poly1[j];
			}
		} 
		else if (poly2[i] == -1) {
			for (size_t j = 0; j < size1; j++) {
				result[i + j] -= poly1[j];
			}
		}
	}
}
static void timeMultiplyPolynomials(int* poly1, size_t size1, int* poly2, size_t size2, int* result, int numberOfTrials = 6) {
	double minTime = DBL_MAX;

	for (int t = 0; t < numberOfTrials; t++) {
		double start = omp_get_wtime();
		multiplyPolynomials(poly1, size1, poly2, size2, result);
		double end = omp_get_wtime();

		double diff = end - start;
		if (diff < minTime)
			minTime = diff;
	}
	printf("Classic polynomial multiplication:   %.8f seconds\n", minTime);
}
static void timeOptimizedMultiplyPolynomials(int* poly1, size_t size1, int* poly2, size_t size2, int* result, int numberOfTrials = 6) {
	double minTime = DBL_MAX;

	for (int t = 0; t < numberOfTrials; t++) {
		double start = omp_get_wtime();
		optimizedMultiplyPolynomials(poly1, size1, poly2, size2, result);
		double end = omp_get_wtime();

		double diff = end - start;
		if (diff < minTime)
			minTime = diff;
	}
	printf("Optimized polynomial multiplication: %.8f seconds\n", minTime);
}

template<typename T>
static void roundArrayOptimized(T* numbers, T* result, size_t size) {
	for (int i = 0; i < size; i++) {
		T extraValue = 0.5;
		if (numbers[i] < 0)
			extraValue = -0.5;
		result[i] = (int)(numbers[i] + extraValue);
	}
}
template<typename T>
static void roundArray(T* numbers, T* result, size_t size) {
	for (int i = 0; i < size; i++) {
		result[i] = round(numbers[i]);
	}
}
template<typename T>
static void timeRoundArray(T* numbers, T* result, size_t size, int numberOfTrials = 6) {
	double minTime = DBL_MAX;

	for (int t = 0; t < numberOfTrials; t++) {
		double start = omp_get_wtime();
		roundArray(numbers, result, size);
		double end = omp_get_wtime();

		double diff = end - start;
		if (diff < minTime)
			minTime = diff;
	}
	printf("Classic array rounding: %.8f seconds\n", minTime);
}
template<typename T>
static void timeRoundArrayOptimized(T* numbers, T* result, size_t size, int numberOfTrials = 6) {
	double minTime = DBL_MAX;

	for (int t = 0; t < numberOfTrials; t++) {
		double start = omp_get_wtime();
		roundArrayOptimized(numbers, result, size);
		double end = omp_get_wtime();

		double diff = end - start;
		if (diff < minTime)
			minTime = diff;
	}
	printf("Optimized array rounding: %.8f seconds\n", minTime);
}

int main()
{
	printf("TASK 1 countPositiveNumbersAtEvenIndices(n = 50M):\n");
	const size_t SIZE = 50'000'000;
	int* positiveNumbers = new int[SIZE];
	int* negativeNumbers = new int[SIZE];
	int* mixedNumbers = new int[SIZE];

	const int LOWER = -200;
	const int UPPER = 200;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distr(LOWER, UPPER);

	for (size_t i = 0; i < SIZE; i++) {
		int number = distr(gen);
		positiveNumbers[i] = abs(number);
		negativeNumbers[i] = number > 0 ? -number : number;
		mixedNumbers[i] = number;
	}

	timeCountPositiveNumbersAtEvenIndices("negative", negativeNumbers, SIZE);	
	timeCountPositiveNumbersAtEvenIndices("positive", positiveNumbers, SIZE);
	timeCountPositiveNumbersAtEvenIndices("mixed", mixedNumbers, SIZE);

	delete[] positiveNumbers;
	delete[] negativeNumbers;
	delete[] mixedNumbers;

	printf("\nTASK 2 bubbleSort(n = 50k):\n");
	const size_t ARRAY_SIZE = 50'000;
	int* initialArray = new int[ARRAY_SIZE];

	for (size_t i = 0; i < ARRAY_SIZE; i++) {
		initialArray[i] = distr(gen);
	}
	timeSortAlgorithms(initialArray, ARRAY_SIZE);
	delete[] initialArray;

	printf("\nTASK 3 multiplyPolynomials(n = 10k):\n");
	const int POLY_SIZE = 10'000;

	int* poly1 = new int[POLY_SIZE];
	int* poly2 = new int[POLY_SIZE];
	int* result = new int[POLY_SIZE + POLY_SIZE - 1];

	uniform_int_distribution<> polyDistr(-1, 1);
	for (int i = 0; i < POLY_SIZE; i++) {
		poly1[i] = distr(gen);
		poly2[i] = polyDistr(gen);
	}
	timeMultiplyPolynomials(poly1, POLY_SIZE, poly2, POLY_SIZE, result);
	timeOptimizedMultiplyPolynomials(poly1, POLY_SIZE, poly2, POLY_SIZE, result);

	delete[] poly1;
	delete[] poly2;
	delete[] result;

	printf("\nTASK 4 round(n = 50M):\n");
	double* numbers1 = new double[SIZE];
	double* output1 = new double[SIZE];

	float* numbers2 = new float[SIZE];
	float* output2 = new float[SIZE];

	uniform_real_distribution<double> dist(LOWER, UPPER);
	default_random_engine engine;

	for (size_t i = 0; i < SIZE; i++) {
		numbers1[i] = dist(engine);
		numbers2[i] = dist(engine);
	}

	printf("Rounding doubles: \n");
	timeRoundArray(numbers1, output1, SIZE);
	timeRoundArrayOptimized(numbers1, output1, SIZE);

	printf("\nRounding floats: \n");
	timeRoundArray(numbers2, output2, SIZE);
	timeRoundArrayOptimized(numbers2, output2, SIZE);

	delete[] numbers1;
	delete[] output1;
	delete[] numbers2;
	delete[] output2;
}
#include <iostream>
#include <random>
#include <Windows.h>
#include <omp.h>
#include "constants.h"

using std::random_device, std::mt19937, std::uniform_int_distribution;
const size_t LENGTH = 1000;

static double calculateSum(const double numbers[], size_t length) {
	double sum = numbers[0];
	for (int i = 1; i < length; i++) {
		sum += numbers[i];
	}
	return sum;
}

static void time__rdtsc(const double numbers[], size_t length, int numberOfTrials) {
	uint64_t minTime = UINT64_MAX;
	double result = 0;
	for (int i = 0; i < numberOfTrials; i++) {
		_mm_lfence();
		uint64_t uStart = __rdtsc();

		double newResult = calculateSum(numbers, length);
		if (i > 0 && newResult != result) {
			printf("The results of addition don't match.");
			exit(1);
		}
		result = newResult;

		uint64_t difference = __rdtsc() - uStart;
		if (difference < minTime) {
			minTime = difference;
		}
		_mm_lfence();
	}
	printf("Sum = %lf", result);
	printf("\n__rdtsc(): t_min = %.8lf us\n", double(minTime) / CPU_DEFAULT_FREQUENCY * 1e6);
}

static void timeQueryPerformanceCounter(const double numbers[], size_t length, int numberOfTrials) {
	LARGE_INTEGER lFrequency;
	LARGE_INTEGER currentTime;
	uint64_t minTime = INT64_MAX;
	if (!QueryPerformanceFrequency(&lFrequency)) {
		printf("QueryPerformanceFrequency failed!\n");
		exit(1);
	}
	double result = 0;

	for (int i = 0; i < numberOfTrials; i++) {
		QueryPerformanceCounter(&currentTime);
		uint64_t t0 = currentTime.QuadPart;

		double newResult = calculateSum(numbers, length);
		if (i > 0 && newResult != result) {
			printf("The results of addition don't match.");
			exit(1);
		}
		result = newResult;

		QueryPerformanceCounter(&currentTime);
		uint64_t difference = currentTime.QuadPart - t0;
		if (difference < minTime)
			minTime = difference;
	}
	printf("Sum = %lf", result);
	printf("\nQueryPerformanceCounter(): t_min = %.8lf us\n", double(minTime) / lFrequency.QuadPart * 1e6);
}

static void timeAbsoluteOmp(const double numbers[], size_t length, int numberOfTrials = 100) {
	double minDifference = DBL_MAX;
	double result = 0;

	for (int i = 0; i < numberOfTrials; i++) {
		double start = omp_get_wtime();

		double newResult = calculateSum(numbers, length);
		if (i > 0 && newResult != result) {
			printf("The results of addition don't match.");
			exit(1);
		}
		result = newResult;

		double difference = omp_get_wtime() - start;
		if (minDifference > difference)
			minDifference = difference;
	}

	printf("Sum = %lf\t", result);
	printf("omp_get_wtime(): T = %.8f ms\n", minDifference * 1e3);
}

static void timeRelativeGetTickCount(const double numbers[], size_t length) {
	DWORD start = GetTickCount();
	size_t count = 0;
	double result = 0;

	while (GetTickCount() - start <= 2000) {
		result = calculateSum(numbers, length);
		count++;
	}
	printf("Sum = %lf\t", result);
	printf("N = %zu\n", count);
}

void performTasks3And4() {
	printf("\nTASK 3:\n");
	const int NUM_OF_TRIALS = 10;

	double numbers[LENGTH]{};
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(0, 100);
	for (int i = 0; i < LENGTH; i++) {
		numbers[i] = double(distrib(gen));
	}
	timeQueryPerformanceCounter(numbers, LENGTH, NUM_OF_TRIALS);
	time__rdtsc(numbers, LENGTH, NUM_OF_TRIALS);

	// Task 4
	printf("\nTASK 4:\n");
	for (size_t i = 1; i <= 3; i++) {
		const size_t length = LENGTH * (i * 100);
		double* dynamicNumbers = new double[length];
		for (int i = 0; i < length; i++) {
			dynamicNumbers[i] = double(distrib(gen));
		}
		printf("\nn=%zu:\n", length);
		printf("Absolute time measurement:\n");
		timeAbsoluteOmp(dynamicNumbers, length);
		printf("Relative time measurement:\n");
		timeRelativeGetTickCount(dynamicNumbers, length);

		delete[] dynamicNumbers;
	}
}
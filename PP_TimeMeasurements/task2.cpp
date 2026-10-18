#include <iostream>
#include <time.h>
#include <Intrin.h>
#include <chrono>
#include <omp.h>
#include <Windows.h>
#include "./constants.h"

using namespace std::chrono;
using std::chrono::duration_cast;

void benchmarkTimeFunctions() {
	time_t start = time(0);
	time_t finish = start;
	while (finish == start) {
		finish = time(0);
	}
	printf("\ttime(): Time difference: %lld s\n", finish - start);

	start = clock();
	finish = start;
	while (finish == start) {
		finish = clock();
	}
	printf("\tclock(): Time difference: %f ms\n", (double)(finish - start) / CLOCKS_PER_SEC * 1e3);

	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	start = ft.dwLowDateTime;
	finish = start;
	while (finish == start) {
		GetSystemTimeAsFileTime(&ft);
		finish = ft.dwLowDateTime;
	}
	printf("\tGetSystemTimeAsFileTime(): Time difference: %f us\n", double(finish - start) * 100 / 1e3);

	FILETIME ft1;
	GetSystemTimePreciseAsFileTime(&ft1);
	start = ft1.dwLowDateTime;
	finish = start;
	while (finish == start) {
		GetSystemTimePreciseAsFileTime(&ft1);
		finish = ft1.dwLowDateTime;
	}
	printf("\tGetSystemTimePreciseAsFileTime(): Time difference: %lld ns\n", (finish - start) * 100);

	DWORD dStart = GetTickCount();
	DWORD dFinish = dStart;
	while (dFinish == dStart) {
		dFinish = GetTickCount();
	}
	printf("\tGetTickCount(): Time difference: %lu ms\n", (dFinish - dStart));

	_mm_lfence();
	uint64_t uStart = __rdtsc();
	uint64_t uFinish = uStart;
	while (uFinish == uStart) {
		uFinish = __rdtsc();
		_mm_lfence();
	}
	printf("\t__rdtsc(): Time difference: %.8lf ns\n", double(uFinish - uStart) / CPU_DEFAULT_FREQUENCY * 1e9);

	LARGE_INTEGER lFrequency;
	LARGE_INTEGER lStart;
	LARGE_INTEGER lFinish;
	if (!QueryPerformanceFrequency(&lFrequency)) {
		printf("QueryPerformanceFrequency failed!\n");
		exit(1);
	}

	QueryPerformanceCounter(&lStart);
	lFinish = lStart;
	while (lStart.QuadPart == lFinish.QuadPart) {
		QueryPerformanceCounter(&lFinish);
	}
	printf("\tQueryPerformanceCounter(): Time difference: %.8lf ns\n", double(lFinish.QuadPart - lStart.QuadPart) / lFrequency.QuadPart * 1e9);

	auto cStart = high_resolution_clock::now();
	auto cFinish = cStart;
	while ((cFinish - cStart).count() == 0) {
		cFinish = high_resolution_clock::now();
	}
	long long dif = duration_cast<nanoseconds>(cFinish - cStart).count();
	printf("\thigh_resolution_clock::now(): Time difference: %lld ns\n", dif);

	double oStart = omp_get_wtime();
	double oFinish = oStart;
	while (oFinish == oStart) {
		oFinish = omp_get_wtime();
	}
	printf("\tomp_get_wtime(): Time difference: %f ns\n", (oFinish - oStart) * 1e9);
}
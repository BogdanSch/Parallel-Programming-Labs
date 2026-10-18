#include <Windows.h>
#include <iostream>

void ConvertTimeTToSystemTime(SYSTEMTIME* st, const time_t* tt) {
	tm my_tm;
	localtime_s(&my_tm, tt);

	memset(st, 0, sizeof(SYSTEMTIME));
	st->wYear = my_tm.tm_year + 1900;
	st->wMonth = my_tm.tm_mon + 1;
	st->wDay = my_tm.tm_mday;
	st->wHour = my_tm.tm_hour;
	st->wMinute = my_tm.tm_min;
	st->wSecond = my_tm.tm_sec;
}

void displayYear2038Problem() {
    SYSTEMTIME st;
    time_t tt = (time_t)0x7FFFFFFF;
    ConvertTimeTToSystemTime(&st, &tt);
    printf("%4.4d:%2.2d:%2.2d %2.2d:%2.2d:%2.2d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}
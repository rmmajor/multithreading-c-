#pragma once

#ifdef OS_LAB_5_EXPORTS
#define OS_LAB_5_API __declspec(dllexport)
#else
#define OS_LAB_5_API __declspec(dllimport)
#endif

#include <Windows.h>
#include <vector>

extern "C" OS_LAB_5_API int min_on_vector(int threads_cnt, std::vector <int>&v);
extern "C" OS_LAB_5_API int getProgress();
extern "C" OS_LAB_5_API void drawProgress();
extern "C" OS_LAB_5_API void updProgress(int chunk_of_work);


extern "C" OS_LAB_5_API DWORD WINAPI answerInThreads(LPVOID param);
extern "C" OS_LAB_5_API DWORD WINAPI statusBar(LPVOID param);

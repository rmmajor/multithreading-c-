#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <semaphore>
#include <sstream>
#include <iomanip>

#define MAX_THREADS 1000

using namespace std;
using namespace std::literals::chrono_literals;

//sync objects
mutex processLocker;
counting_semaphore thread_semaphore(8);

int completed_work;
int step, threads_count;
int final_ans = INT_MAX;
vector < int > a;

HANDLE  handles[MAX_THREADS];

DWORD WINAPI answerInThreads(LPVOID param);
DWORD WINAPI statusBar(LPVOID param);
DWORD threadID[MAX_THREADS];
DWORD processUpdaterID;

int getProgress();
void drawProgress();
void to_int_vector(string s);
void updProgress(int chunk_of_work);
int min_on_vector(int threads_cnt);

int main()
{
    //int n;
    string text, text_r;
//    vector < int > v;

    HANDLE file = CreateFileA("D:\\university\\2_kurs\\OS\\os_lab_4\\labcLion\\testsets\\test0.txt",
                              GENERIC_READ,
                              0, NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);


    HANDLE mapping = CreateFileMapping(file,
                                       0,
                                       PAGE_READONLY,
                                       0,
                                       0,
                                       NULL);

    text_r = (char *)MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);

    CloseHandle(mapping);
    CloseHandle(file);

    to_int_vector(text_r);

    cout << "Enter number of threads:\n";
    cin >> threads_count;
    cout << "Final answer: " << min_on_vector(threads_count) << endl;

    system("PAUSE");
    return 0;
}

int min_on_vector(int threads_cnt) {
    //a = v;
    processLocker.lock();
    int n = a.size();
    threads_count = threads_cnt;
    processLocker.unlock();

    step = n / threads_count;

    auto start = std::chrono::steady_clock::now();

    HANDLE processUpdater;
    processUpdater = CreateThread(
            0,
            0,
            statusBar,
            NULL,
            0,
            &processUpdaterID);

    int pos = 0;
    for (int i = 0; i < threads_count; i++) {
        handles[i] = CreateThread(
                NULL,
                0,
                answerInThreads,
                (LPVOID)pos,
                0,
                &threadID[i]);
        pos += step;
        if (handles[i] == NULL) {
            cout << "wtf\n";
            ExitProcess(3);
        }
    }

    for (int i = 0; i < threads_count; i++) {
        WaitForSingleObject(handles[i], INFINITE);
        CloseHandle(handles[i]);
    }

    WaitForSingleObject(processUpdater, INFINITE);
    CloseHandle(processUpdater);

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    cout << fixed << setprecision(10);
    cout << "\n Entire time: " << elapsed_seconds.count() << "s\n";

    return final_ans;
}

DWORD WINAPI answerInThreads(LPVOID param) {

    thread_semaphore.acquire();

    int n = a.size();
    int thread_pos;
    memcpy(&thread_pos, &param, sizeof param);

    int l = thread_pos;
    int r = thread_pos + step;

    if (n - step < r) {
        r = n;
    }

    int mn = INT_MAX;
    for (int i = l; i < r; i++) {
        mn = min(mn, a[i]);
        updProgress(1);
    }

    processLocker.lock();
    final_ans = min(mn, final_ans);
    processLocker.unlock();

    thread_semaphore.release();
    return 0;
}

DWORD WINAPI statusBar(LPVOID param) {
    int n = a.size();
    while(getProgress() < n) {
        Sleep(100);
        drawProgress();
    }

    return 0;
}

int getProgress() {
    int progress;

    processLocker.lock();
    progress = completed_work;
    processLocker.unlock();

    return progress;
}

void drawProgress() {
    processLocker.lock();
    int n = a.size();
    cout << "\r" << "Process: ";
    int procents = ((double)completed_work / (double)n) * 100;

    for (int i = 0; i < 50; i++) {
        if (i*2 <= procents)
            cout << (char)219;
        else
            cout << "_";
    }

    cout << " " << procents << " / 100 % \n";
    cout << "current answer: " << final_ans << endl;
    processLocker.unlock();
}

void updProgress(int chunk_of_work) {

    processLocker.lock();
    completed_work += chunk_of_work;
    processLocker.unlock();
}

void to_int_vector(string s) {
    string temp;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '\n' || s[i] == '\0' ||s[i] == ' ') {

            int x;
            stringstream ss;
            ss << temp;
            ss >> x;
            a.push_back(x);
            temp = "";
        }
        else {
            temp += s[i];
        }
    }
}
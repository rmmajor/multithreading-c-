
#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
//#include <mutex>
//#include <semaphore>

#define MAX_THREADS 1

using namespace std;
//using namespace std::literals::chrono_literals;

//sync objects
//mutex processLocker, rangeLocker, updLocker, drawLocker;
//counting_semaphore thread_semaphore(8);

int completed_work;
int n, step, g_pos;
int range = 0, final_ans = INT_MAX;
vector < int > a, mins;

double timestamps[MAX_THREADS];
HANDLE  stackTh[64];
HANDLE  handles[MAX_THREADS];
LARGE_INTEGER t1, t2, f, start_timestamp, finish_timestamp;

DWORD WINAPI answerInThreads(LPVOID param);
//DWORD WINAPI statusBar(LPVOID param);
DWORD threadID[MAX_THREADS];
DWORD processUpdaterID;

int final_answer();
//int getProgress();
//void drawProgress();
//void updProgress(int chunk_of_work);

int main()
{
   ifstream fin("D:\\university\\2_kurs\\OS\\os_lab_4\\labcLion\\testsets\\test3.txt");
   fin >> n;
   int x;
   for (int i = 0; i < n; i++) {
       fin >> x;
       a.push_back(x);
   }
   fin.close();

   step = n / MAX_THREADS;
   int pos = 0;

//   HANDLE processUpdater;
//   processUpdater = CreateThread(
//           0,
//           0,
//           statusBar,
//           NULL,
//           0,
//           &processUpdaterID);

   QueryPerformanceFrequency(&f);
   QueryPerformanceCounter(&start_timestamp);


   //WaitForMultipleObjects(MAX_THREADS, handles, TRUE, INFINITE);
   pos = 0;
   for (int i = 0; i < MAX_THREADS; i++) {
       //cout << pos << endl;
       handles[i] = CreateThread(
               NULL,
               0,
               answerInThreads,
               (LPVOID)pos,
               0,
               &threadID[i]);
       pos += step;
//        if (handles[i] == NULL) {
//            cout << "wtf\n";
//            ExitProcess(3);
//        }
   }

   for (int i = 0; i < MAX_THREADS; i++) {
       WaitForSingleObject(handles[i], INFINITE);
       CloseHandle(handles[i]);
   }

   QueryPerformanceCounter(&finish_timestamp);

//   WaitForSingleObject(processUpdater, INFINITE);
//   CloseHandle(processUpdater);

   double time = (finish_timestamp.QuadPart - start_timestamp.QuadPart) * 1.0 / (f.QuadPart);

//    for (int i = 0; i < MAX_THREADS; i++) {
//        cout << "Thread" << i+1 << " time: " << timestamps[i] << endl;
//    }
   cout << "\nEntire time: " << time << "s \n";

   cout << endl;
   cout << final_ans << endl;
   system("PAUSE");
   return 0;
}


DWORD WINAPI answerInThreads(LPVOID param) {

//   thread_semaphore.acquire();

   int thread_pos;
   memcpy(&thread_pos, &param, sizeof param);

   int l = thread_pos;
   int r = thread_pos + step;

//   processLocker.lock();
   range += (r - l);
//   processLocker.unlock();

   // хз чи паше
   if (n - step < r) {
       r = n;
   }

   //cout << l << " " << r << '\n';
   int mn = INT_MAX;
   for (int i = l; i < r; i++) {
       //problem
       mn = min(mn, a[i]);
//       updProgress(1);
   }
//   processLocker.lock();
   //mins.push_back(mn);
   final_ans = min(mn, final_ans);
   // cout << "r " << r << " |\n";
  // processLocker.unlock();

   //Sleep(100);

   //updProgress(step);
   //thread_semaphore.release();
   return 0;
}
/*
DWORD WINAPI statusBar(LPVOID param) {
   while(getProgress() < n) {
       Sleep(100);
       drawProgress();
   }

   return 0;
}

int getProgress() {
   int progress = 0;

   processLocker.lock();
   progress = completed_work;
   processLocker.unlock();

   return progress;
}

void drawProgress() {
   processLocker.lock();

   cout << "\r" << "Process: ";
   int procents = ((double)completed_work / (double)n) * 100;
   //cerr << completed_work << " " << procents << endl;

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
*/
int final_answer() {
   int ans = mins[0];
   for (auto it: mins) {
           ans = min(ans, it);
       //cout << "it ->" << it << endl;
   }
   return ans;
}

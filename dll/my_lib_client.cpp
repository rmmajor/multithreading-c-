#include <iostream>
#include <fstream>
#include <vector>
#include "os_lab_5.h"

using namespace std;

int main()
{
    ifstream fin("D:\\university\\2_kurs\\OS\\os_lab_4\\labcLion\\testsets\\test0.txt");
    int n;
    fin >> n;
    int x;
    vector < int > a;   
    for (int i = 0; i < n; i++) {
        fin >> x;
        a.push_back(x);
    }
    fin.close();

    int threads_cnt;
    cout << "Enter number of threads:\n";
    cin >> threads_cnt;

    cout << min_on_vector(threads_cnt, a) << endl;

    system("PAUSE");
    return 0;
}

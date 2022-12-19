#include <iostream>
#include <time.h>
#include <cstdlib>
#include <fstream>

#define int long long

using namespace std;

main()
{
    srand(time(0));
    ofstream fout("testsets/test3.txt");
    unsigned int n;
    int a, b;

    cout << "Enter the size of set: \n";
    cin >> n;
    fout << n << endl;

    cout << "Set the limits for random\n";
    cin >> a >> b;

    for (int i = 0; i < n; i++) {
        fout << a + rand() % (a-b) << " ";
    }
    fout << endl;

}

#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <time.h>

using namespace std;

void printVector(const int n_mechs, const int n_jobs, vector< vector<int> >& myVector) {
    for(int i = 0; i < n_mechs; ++i) {
        for(int j = 0; j < n_jobs; ++j) {
            cout << myVector[i][j] << " ";
        }
        cout << endl;
    }
}

const int countMakespan(const int n_mechs, const int n_jobs, vector< vector<int> > myVector) {

    for(int i = 1; i < n_mechs; ++i) {
        myVector[i][0] += myVector[i-1][0];
    }
    for(int j = 1; j < n_jobs; ++j) {
        myVector[0][j] += myVector[0][j-1];
    }
    for(int i = 1; i < n_mechs; ++i) {
        for(int j = 1; j < n_jobs; ++j) {
            myVector[i][j] += (myVector[i-1][j] > myVector[i][j-1]) ? myVector[i-1][j] : myVector[i][j-1];
        }
    }
    ///printVector(n_mechs, n_jobs, myVector);
    return myVector[n_mechs-1][n_jobs-1];
}

vector< vector<int> >& iiTestSwap(const int n_mechs, const int njobA, const int njobB, vector< vector<int> > myVector) {
    for(int i = 0; i < n_mechs; ++i) {
        swap(myVector[i][njobA], myVector[i][njobB]);
    }
    return myVector;
} /// need to fix

vector< vector<int> >& iiSwap(const int n_mechs, const int njobA, const int njobB, vector< vector<int> >& myVector) {
    for(int i = 0; i < n_mechs; ++i) {
        swap(myVector[i][njobA], myVector[i][njobB]);
    }
    return myVector;
}

const int iterativeImprovement(const int n_mechs, const int n_jobs, vector< vector<int> >& myVector) {
    bool nextSwap = true;
    int bestm = countMakespan(n_mechs, n_jobs, myVector);
    ///cout << "Origin MakeSpan: " << bestm << endl << endl;
    while(nextSwap) {
        nextSwap = false;
        for(int j1 = 0; j1 < n_jobs - 1; ++j1) {
            for(int j2 = j1 + 1; j2 < n_jobs; ++j2) {
                if( bestm > countMakespan(n_mechs, n_jobs, iiTestSwap(n_mechs, j1, j2, myVector)) ) {
                    bestm = countMakespan(n_mechs, n_jobs, iiSwap(n_mechs, j1, j2, myVector));
                    nextSwap = true;
                    ///cout << "Swap: ("<< j1 << ", " << j2 << "). MakeSpan: " << bestm << endl;
                    ///printVector(n_mechs, n_jobs, myVector);
                }
                ///cout << "try: " << j1 << ", " << j2 << endl;
            }
        }
        ///if (nextSwap) { cout << endl; }
    }
    return bestm;
}

int main()
{
    srand(time(0));

    ifstream ifile("PFSP_benchmark_data_set/tai20_5_1.txt");
    ofstream ofile("output.txt");

    int n_jobs, n_mechs, counter = 1, times = 20;
    string testName;

    ifile >> n_jobs >> n_mechs >> testName;
    vector< vector<int> > myVector(n_mechs, vector<int>(n_jobs, 0));

    for(int i = 0; i < n_mechs; ++i) {
        for(int j = 0; j < n_jobs; ++j) {
            ifile >> myVector[i][j];
        }
    }

    while(counter <= times) {
        int a = rand() % n_jobs, b = ( a + ( rand() % (n_jobs - 1) + 1 ) ) % n_jobs;
        myVector = iiSwap(n_mechs, a, b, myVector);
        ofile << counter << " times" << endl;
        ofile << "Swap: (" << a << ", " << b << "). Min MakeSpan: " << iterativeImprovement(n_mechs, n_jobs, myVector) << endl;
        counter++;
    }

    return 0;
}

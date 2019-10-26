#include <iostream>
#include <cmath>
#include "dtmf.h"

using namespace std;

vector<int> DTMF::num_cycle(vector<int> freqs, const int N, const int R) {
    float k;
    vector<int> K;

    for (int i = 0; i < 8; i++) {
        k = float(N) * float(freqs[i]) / float(R);
        k = round(k);
        K.push_back(k);
    }
    return K;
}

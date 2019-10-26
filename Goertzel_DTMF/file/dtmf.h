#ifndef GOERTZEL_DTMF_DTMF_H
#define GOERTZEL_DTMF_DTMF_H

#include <vector>

using namespace std;

class DTMF {
public:
    DTMF() : N(205), R(8000) {}

    /*DTMF frequencies*/
    vector<int> freqs{
            697, 770, 852, 941,
            1209, 1336, 1477, 1633
    };
    /*DTMF buttons*/
    vector<vector<char> > keys = {
            {'1', '2', '3', 'A'},
            {'4', '5', '6', 'B'},
            {'7', '8', '9', 'C'},
            {'*', '0', '#', 'D'}
    };

    const int N; // Number of samples in processing section
    const int R; // Sampling rate

    /*Number of complete cycles of target frequency in processing section*/
    vector<int> num_cycle(vector<int> f, const int N, const int R);

};

#endif //GOERTZEL_DTMF_DTMF_H


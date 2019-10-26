#ifndef GOERTZEL_DTMF_GOERTZEL_H
#define GOERTZEL_DTMF_GOERTZEL_H

#include <vector>

using namespace std;

class Goertzel {
public:
    Goertzel() : nums(0) {}

    int nums; //Numbers of signal processing batch

    struct out {
        vector<double> P;
        vector<double> Pm;
        vector<double> outP;
        vector<int> index;
        vector<int> result;
    } output;

    /*Signal pre-processing, get parameter C*/
    vector<double> pre_process(vector<int> K, const int N);

    /*Signal processing, get the result*/
    void process(vector<double> C, const int N, vector<int> sample);

    /*Signal post-processing, filter the output*/
    void post_process();

    /*Run Goertzel Algorithm*/
    void run(vector<int> K, const int N, vector<int> sample);

};

#endif //GOERTZEL_DTMF_GOERTZEL_H



#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "goertzel.h"

using namespace std;

static const double PI = 3.1415925;


void Goertzel::run(vector<int> K, const int N, vector<int> sample) {

    vector<double> C;

    C = pre_process(K, N);  //get parameter C
//    for (int i = 0; i < 8; i++) cout << C[i] << endl;
    process(C, N, sample);
    post_process();
}


vector<double> Goertzel::pre_process(vector<int> K, const int N) {
    vector<double> C;
    double c;
    for (int i = 0; i < 8; i++) {
        c = 2 * cos(2.0 * PI * double(K[i]) / double(N));
        C.push_back(c);
    }
    return C;
}


void Goertzel::process(vector<double> C, const int N, vector<int> sample) {

    double p;
    double q0, q1, q2;  // 中间变量
    double Px, Py;
    int x_ind, y_ind;

    /* Goertzel Algorithm */
    nums = int(sample.size() / N);  // 126
    for (int k = 0; k < nums; k++) {
        for (int i = 0; i < 8; i++) {
            p = 0;
            q0 = 0;
            q1 = 0;
            q2 = 0;
            for (int j = N * k; j < N * (k + 1); j++) {
                q0 = C[i] * q1 - q2 + sample[j];
                q2 = q1;
                q1 = q0;
            }
            p = pow(q1, 2) + pow(q2, 2) - C[i] * q1 * q2;
            this->output.P.push_back(p);
        }

        /* Max value from rows and columns */
        Px = 0, Py = 0;
        x_ind = -1, y_ind = -1;
        for (int i = 8 * k; i < 8 * (k + 1); i++) {
            if (i < 8 * k + 4) {
                if (Px < this->output.P[i]) {
                    Px = this->output.P[i];
                    x_ind = i;
                }
            } else {
                if (Py < this->output.P[i]) {
                    Py = this->output.P[i];
                    y_ind = i;
                }
            }
        }

        this->output.Pm.push_back(Px);
        this->output.Pm.push_back(Py);
        this->output.index.push_back(x_ind % 8);
        this->output.index.push_back((y_ind % 8) - 4);
    }
}


void Goertzel::post_process() {

    double Pmax;
    int count;

    for (unsigned long k = 0; k < this->output.index.size(); k += 2) {
        count = 0;
        Pmax = max(this->output.Pm[k], this->output.Pm[k + 1]);

        if (this->output.Pm[k] > 4e+9 and this->output.Pm[k + 1] > 4e+9) {  // set threshold

            if (this->output.Pm[k] >= this->output.Pm[k + 1] * 0.158 and
                this->output.Pm[k + 1] >= this->output.Pm[k] * 0.398) {

                for (unsigned long i = 8 * k; i < 8 * (k + 1); i++) {
                    if (this->output.P[i] > Pmax * 0.158) count += 1;
                }
                if (count < 3) {
//                    this->output.result.push_back(this->output.index[k]);
//                    this->output.result.push_back(this->output.index[k + 1]);
                    this->output.outP.push_back(this->output.Pm[k]);
                    this->output.outP.push_back(this->output.Pm[k + 1]);
                }
            }
        }
    }
}

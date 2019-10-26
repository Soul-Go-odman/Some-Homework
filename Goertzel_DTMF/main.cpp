#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include "file/goertzel.h"
#include "file/dtmf.h"

using namespace std;

vector<int> data_read(string file_name);

void data_write(vector<char> output);

void outP(vector<double> P, string filename);


int main() {

    DTMF dtmf;
    Goertzel goertzel;

    vector<int> K;  //Number of complete cycles of target frequency in processing section
    vector<double> C; // Parameter
    vector<double> P;
    vector<char> button;
    vector<int> number = data_read("telephone_number1.txt");

    K = dtmf.num_cycle(dtmf.freqs, dtmf.N, dtmf.R);
    goertzel.run(K, dtmf.N, number);

    for (unsigned long i = 0; i < goertzel.output.outP.size(); i += 2) {
//        cout << i << ": " << goertzel.output.result[i] << '\t' << goertzel.output.result[i + 1];
//        cout << " => " << dtmf.keys[goertzel.output.result[i]][goertzel.output.result[i + 1]] << endl;
//        button.push_back(dtmf.keys[goertzel.output.result[i]][goertzel.output.result[i + 1]]);
        P.push_back(goertzel.output.outP[i]);
        P.push_back(goertzel.output.outP[i + 1]);
    }

    cout << "This file contain " << P.size() / 2 << " numbers" << endl;
//    for (char i: button) cout << i << endl;
//    for (double i: P) cout << i << endl;

//    data_write(button);
    outP(P, "outP.txt");
    outP(goertzel.output.P, "P.txt");
    outP(goertzel.output.Pm, "Pm.txt");

    return 0;
}


vector<int> data_read(string file_name) {

    vector<int> data;
    string s;
    int sample;

    string file_path = "../data/" + file_name;
    ifstream infile(file_path);
    cout << "Reading from this file: " << file_path << endl;

    assert(infile.is_open());
    while (!infile.eof()) {
        getline(infile, s);
        sample = stoi(s);
        data.push_back(sample);
    }

    infile.close();
    return data;
}


void data_write(vector<char> output) {
    ofstream outfile("result.txt");

    for (char i: output) {
        outfile << i << endl;
    }

    outfile.close();
}


void outP(vector<double> P, string filename) {
    ofstream outfile(filename);

    for (double i: P) {
        outfile << i << endl;
    }

    outfile.close();
}

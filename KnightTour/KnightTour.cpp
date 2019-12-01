#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <iomanip>
#include "algorithm.h"

using namespace std;

int CHESS_SIZE = 8;
int TOTAL_STEP = 64;
void Init(vector<vector<int> >& chess);  // 初始化棋盘
void Print(vector<vector<int> >& chess); // 打印棋盘


int main(){
	vector<vector<int>> chess(CHESS_SIZE, vector<int>(CHESS_SIZE));
	Init(chess);
	DFS dfs(chess, 0, 0);

	clock_t start_time = clock();
	dfs.run(0, 0, 1);
	clock_t end_time = clock();
	cout << "Time cost: " << (end_time - start_time)/ CLOCKS_PER_SEC * 1000 << " ms" << endl;

	dfs.get_nBack();
	system("pause");

	return 1;
}


void Init(vector<vector<int> >& chess) {
	for(int i = 0; i < CHESS_SIZE; i++) {
		for (int j = 0; j < CHESS_SIZE; j++) {
			chess[i][j]=0;
		}
	}
}

void Print(vector<vector<int> >& chess) {
	for (int i = 0; i < CHESS_SIZE; i++) {
		for (int j = 0; j < CHESS_SIZE; j++) {
			cout << left << setw(3) << chess[i][j];
		}
		cout << endl;
	}
	cout << "* * * * * * * * * * * * * * * * *" << endl;
}

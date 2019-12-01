#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <thread>
#include "algorithm.h"

using std::vector;
using std::cout;
using std::endl;

extern int CHESS_SIZE;
extern int TOTAL_STEP;


bool DFS::check(int x, int y) {
	return (x >= 0 && x < CHESS_SIZE && y >= 0 && y < CHESS_SIZE && _chess[y][x] == 0);
}

void DFS::sort_index(vector<vector<int>>& nRoad) {
	std::sort(nRoad.begin(), nRoad.end());
}

//void DFS::sort_direction(int x, int y) {
//	int x_next=0, y_next=0;
//	int x_next_next=0, y_next_next=0;
//	init_nRoad(_nRoad);
//
//	int count = 0;
//	for (int i = 0; i < CHESS_SIZE; i++) {  
//		count = 0;
//		x_next = x + _move[i][0];
//		y_next = y + _move[i][1];
//
//		if (check(x_next, y_next)) {
//			for (int j = 0; j < CHESS_SIZE; j++) {
//				x_next_next = x_next + _move[j][0];
//				y_next_next = y_next + _move[j][1];
//				if (check(x_next_next, y_next_next)) {
//					count++;
//				}  
//			}
//		}
//		else count = 99;
//		_nRoad[i][0] = count;
//	}
//	sort_index(_nRoad);
//}


void DFS::sort_direction(int x, int y) {
	int x_next = 0, y_next = 0;
	int x_next_next = 0, y_next_next = 0;
	init_nRoad(_nRoad);

	int count = 0;
	for (int i = 0; i < CHESS_SIZE; i++) {
		count = 0;
		x_next = x + _move[i][0];
		y_next = y + _move[i][1];

		if (check(x_next, y_next)) {
			std::thread road0();

			for (int j = 0; j < CHESS_SIZE; j++) {
				x_next_next = x_next + _move[j][0];
				y_next_next = y_next + _move[j][1];
				if (check(x_next_next, y_next_next)) {
					count++;
				}
			}
		}
		else count = 99;
		_nRoad[i][0] = count;
	}
	sort_index(_nRoad);
}


void DFS::run(int x, int y, int deep) {
	if (deep == TOTAL_STEP) {
		std::cout << "--------------------------------------------" << std::endl;
		print_result(_chess);
		flag = true;
		return;
	}
	if (flag) return;

	sort_direction(x, y);  // 对八个方向进行排序
	
	int x_next=0, y_next=0;
	for (int i = 0; i < 8; i++) {  // 八个方向
		x_next = x + _move[_nRoad[i][1]][0];
		y_next = y + _move[_nRoad[i][1]][1];

		if (check(x_next, y_next)) {
			_chess[y_next][x_next] = deep + 1;
			run(x_next, y_next, deep + 1);
			_chess[y_next][x_next] = 0;
			nBack++;
		}
	}
	return;
}

void DFS::print_result(vector<vector<int>>& chess) const{
	for (int i = 0; i < CHESS_SIZE; i++) {
		for (int j = 0; j < CHESS_SIZE; j++) {
			std::cout << std::left << std::setw(5) << chess[i][j];
		}
		std::cout << std::endl;
	}
}

void DFS::print_nroad(vector<vector<int>>& nRoad) {
	for (auto& row : nRoad) {
		for (auto& col : row) {
			std::cout << std::setw(3) << std::left << col;
		}
		std::cout << std::endl;
	}
	std::cout << "************************" << std::endl << std::endl;
}

void DFS::init_nRoad(vector<vector<int>>& nRoad) {
	nRoad = {
		{8, 0},
		{8, 1},
		{8, 2},
		{8, 3},
		{8, 4},
		{8, 5},
		{8, 6},
		{8, 7},
	};
}
#pragma once
#include <vector>

extern int CHESS_SIZE;
extern int TOTAL_STEP;

using std::vector;

class DFS {
public:
	DFS(vector<vector<int>> chess, int x, int y) : _chess(chess), _x(x), _y(y),
		_move({
			{ - 2,  - 1},
			{ - 2,  + 1},
			{ - 1,  + 2},
			{ + 1,  + 2},
			{ + 2,  + 1},
			{ + 2,  - 1},
			{ + 1,  - 2},
			{ - 1,  - 2}
			}) {
		_chess[_y][_x] = 1;
	}

	bool check(int x, int y);
	void run(int x, int y, int deep = 1);
	void sort_direction(int x, int y);
	void sort_index(vector<vector<int>>& score);

	void print_nroad(vector<vector<int>>& nRoad);
	void print_result(vector<vector<int>>& chess) const;

	inline void get_nBack() {
		std::cout << nBack << std::endl;
	}

	void init_nRoad(vector<vector<int>>& nRoad);

private:
	int _x, _y;
	vector<vector<int>> _chess;
	vector<vector<int>> _move;

	int nBack = 0; // »ØËÝ´ÎÊý
	bool flag = false;
	vector<vector<int>> _nRoad;  // num of road + index of road
};
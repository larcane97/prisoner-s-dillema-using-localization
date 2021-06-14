#include <iostream>
#include <vector>
#include <time.h>
using namespace std;

int board[20][20];
int cost[20][20];
int dir[4][2] = {
	{0,1},
	{1,0},
	{-1,0},
	{0,-1}
};
const int DIR_SIZE = 4;

// 0 : altru , 1 : non-altru
void init() {
	srand(time(NULL));
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			board[i][j] = (rand() % 2);
	return;
}

int get_cost(int x, int y, int dx, int dy) {
	if (board[x][y] == 0 && board[dx][dy] == 0)
		return 2;
	else if (board[x][y] == 1 && board[dx][dy] == 0)
		return 3;
	else if (board[x][y] == 0 && board[dx][dy] == 1)
		return -1;
	else if (board[x][y] == 1 && board[dx][dy] == 1)
		return 0;
	return -1;
}

void trans_coor(int& dx, int& dy) {
	if (dx == 20) dx = 0;
	if (dy == 20) dy = 0;
	if (dx == -1) dx = 19;
	if (dy == -1) dy = 19;
	return;
}

void calc_cost(int x, int y) {
	for (int i = 0; i < DIR_SIZE; i++) {
		int dx = x + dir[i][0];
		int dy = y + dir[i][1];
		trans_coor(dx, dy);

		// 게임 진행
		cost[x][y] += get_cost(x, y, dx, dy);
	}
}

double print_state() {
	int altru = 0;
	int non_altru = 0;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			printf("%d ", board[i][j]);
			if (board[i][j] == 0)
				altru++;
			else non_altru++;
		}
		printf("\n");
	}
	printf("altru : %.2f%%\n", altru / 400.0 * 100.0);
	printf("non-altru : %.2f%%\n", non_altru / 400.0 * 100.0);
	return (double)altru / 400.0 * 100.0;
}

void change_strategy(int x, int y) {
	double altru = 0;
	double non_altru = 0;
	int altru_num = 0;
	int non_altru_num = 0;
	for (int i = 0; i < DIR_SIZE; i++) {
		int dx = x + dir[i][0];
		int dy = y + dir[i][1];
		trans_coor(dx, dy);

		if (board[dx][dy] == 0) {
			altru += (double)cost[dx][dy];
			altru_num++;
		}
		else {
			non_altru += (double)cost[dx][dy];
			non_altru_num++;
		}
	}
	altru = altru_num==0? 0: altru/(double)altru_num;
	non_altru = non_altru_num==0? 0 : non_altru/(double)non_altru_num;
	if (altru > non_altru)
		board[x][y] = 0;
	else if (non_altru > altru)
		board[x][y] = 1;
	return;
}

void reset_cost() {
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			cost[i][j] = 0;
}

int main() {
	int a,c;
	cout << "set total game :";
	cin >> a;
	cout << "set iteration :";
	cin >> c;
	double max_ratio = -1;
	double mean_ratio = 0;
	for (int t = 0; t < a; t++) {
		printf("----------------------------------------------------\n");
		init();
		printf("init state..\n");
		print_state();

		for (int iter = 0; iter < c; iter++) {
			//reset_cost();
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 20; j++)
					calc_cost(i, j);
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 20; j++)
					change_strategy(i, j);
		/*	printf("%d-th stage..\n", iter + 1);
			print_state();
			printf("\n\n");*/
		}
		printf("last state(%d-th stage)..\n",c);
		double tmp = print_state();
		max_ratio = max(max_ratio,tmp);
		mean_ratio += tmp;
		printf("----------------------------------------------------\n");
		printf("\n");
	}
	printf("max ratio : %.2f%%\n", max_ratio);
	printf("mean ratio : %.2f%%\n", mean_ratio / a);

}
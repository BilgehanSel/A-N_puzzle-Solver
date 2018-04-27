/*
	Npuzzle solver
	even though it assumes you'll enter 4x4 puzzle (15 puzzle),
	you achieve others by changing the loop in main func.
	and also changing goal state to your exact needs

*/

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <ctime>

using namespace std;

struct node {
	vector<vector<int>> puzzle;
	int cost;
	int depth;
	node* right;
	node* left;
	node* up;
	node* down;
	node* parent;
};

void display_puzzle(vector<vector<int>> p) {
	for (int i = 0; i < p.size(); i++) {
		for (int j = 0; j < p.size(); j++) {
			cout << p[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}


class Npuzzle {
public:

	Npuzzle() {
		start_state = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 0 } };
		root = new node;
		goal_state = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 0 } };
		//goal_state = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 0 } };
		root = new node;
		root->puzzle = start_state;
		root->parent = NULL;
		root->cost = cost_puzzle(start_state);
		root->depth = 0;
	}

	Npuzzle(vector<vector<int>> p) {
		start_state = p;
		goal_state = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 0 } };
		root = new node;
		root->puzzle = start_state;
		root->parent = NULL;
		root->cost = cost_puzzle(start_state);
		root->depth = 0;
	}

	void solve_puzzle() {
		open_list.insert({ puzzle_to_string(root->puzzle), root });
		pair < unordered_map<string, node*>::iterator, double> smallest;

		while (!open_list.empty()) {
			double weight_cost = 1;
			double weight_depth = 0.1; // Keeping it low like this can help to move deeper and quickly solve the puzzle
			smallest.first = open_list.begin();
			smallest.second = weight_cost*smallest.first->second->cost + weight_depth*smallest.first->second->depth;

			for (unordered_map<string, node*>::iterator it = open_list.begin(); it != open_list.end(); it++) {
				if (smallest.second > weight_cost*it->second->cost + weight_depth*it->second->depth) {
					smallest.first = it;
					smallest.second = weight_cost*it->second->cost + weight_depth*it->second->depth;
				}
			}
			current = (*(smallest.first)).second;
			swap_puzzle();
			std::cout << "Depth is: " << current->depth << std::endl;

			while (solved) {
				spuzzle.push_back(current->puzzle);
				if (current->parent != NULL) {
					current = current->parent;
				}
				else {
					cout << "Number of puzzles generated: " << open_list.size() + closed_list.size() << endl;
					goto endloop;
				}
			}

			closed_list.insert({ puzzle_to_string(current->puzzle), current });

			open_list.erase(puzzle_to_string(current->puzzle));
			//cout << current->depth << " ---- " << open_list.size() + closed_list.size() << " ---- " << current->cost << endl;
			//cout << "Number of puzzles generated: " << open_list.size() + closed_list.size() << endl;
			//cout << open_list.size() << endl;
		}
	endloop:;
	}

	vector<vector<vector<int>>> get_spuzzle() {
		return spuzzle;
	}

	bool contains_puzzle(vector<vector<int>> &p) {

		//unordered_map<string, node*>::const_iterator got = closed_list.find(puzzle_to_string(p));
		if (closed_list.count(puzzle_to_string(p)) > 0) {
			return false;
		}

		return true;
	}

	void swap_puzzle() {
		for (int i = 0; i < current->puzzle.size(); i++) {
			for (int j = 0; j < current->puzzle.size(); j++) {
				if (current->puzzle[i][j] == 0) {
					if (j + 1 < current->puzzle.size()) {
						vector<vector<int>> temp = current->puzzle;
						swap(temp[i][j + 1], temp[i][j]);
						if (temp == goal_state) {
							spuzzle.push_back(temp);
							solved = true;
							cout << "Solved" << endl;
						}
						else if (contains_puzzle(temp)) {
							current->right = new node;
							current->right->depth = current->depth + 1;
							current->right->parent = current;
							current->right->puzzle = temp;
							open_list.insert({ puzzle_to_string(temp), current->right });
						}
					}
					//Left
					if (j - 1 >= 0) {
						vector<vector<int>> temp = current->puzzle;
						swap(temp[i][j - 1], temp[i][j]);
						if (temp == goal_state) {
							spuzzle.push_back(temp);
							solved = true;
							cout << "solved" << endl;
						}
						else if (contains_puzzle(temp)) {
							current->left = new node;
							current->left->cost = cost_puzzle(temp);
							current->left->depth = current->depth + 1;
							current->left->parent = current;
							current->left->puzzle = temp;
							open_list.insert({ puzzle_to_string(temp), current->left });
						}
					}
					//Up
					if (i - 1 >= 0) {
						vector<vector<int>> temp = current->puzzle;
						swap(temp[i - 1][j], temp[i][j]);
						if (temp == goal_state) {
							spuzzle.push_back(temp);
							solved = true;
							cout << "solved" << endl;
						}
						else if (contains_puzzle(temp)) {
							current->up = new node;
							current->up->cost = cost_puzzle(temp);
							current->up->depth = current->depth + 1;
							current->up->parent = current;
							current->up->puzzle = temp;
							open_list.insert({ puzzle_to_string(temp), current->up });
						}
					}
					//Down
					if (i + 1 < current->puzzle.size()) {
						vector<vector<int>> temp = current->puzzle;
						swap(temp[i + 1][j], temp[i][j]);
						if (temp == goal_state) {
							spuzzle.push_back(temp);
							solved = true;
							cout << "solved" << endl;
						}
						else if (contains_puzzle(temp)) {
							current->down = new node;
							current->down->cost = cost_puzzle(temp);
							current->down->depth = current->depth + 1;
							current->down->parent = current;
							current->down->puzzle = temp;
							open_list.insert({ puzzle_to_string(temp), current->down });
						}
					}
				}
			}
		}
	}

	int cost_puzzle(vector<vector<int>> p) {
		int cost = 0;

		for (int i = 0; i < p.size(); i++) {
			for (int j = 0; j < p.size(); j++) {
				if (p[i][j] != 0) {
					if (p[i][j] % p.size() != 0) {
						int temp_i = p[i][j] / p.size();
						int temp_j = p[i][j] % p.size() - 1;
						cost = absolute(temp_i - i) + absolute(temp_j - j) + cost;
					}
					else {
						int temp_i = p[i][j] / p.size() - 1;
						int temp_j = p.size() - 1;
						cost = absolute(temp_i - i) + absolute(temp_j - j) + cost;
					}
				}
				else {
					cost = absolute(p.size() - 1 - i) + absolute(p.size() - 1 - j) + cost;
				}
			}
		}
		return cost;
	}

	int absolute(int val) {
		if (val >= 0) {
			return val;
		}
		else {
			return -val;
		}
	}

	string puzzle_to_string(vector<vector<int>> p) {
		string str;
		for (int i = 0; i < p.size(); i++) {
			for (int j = 0; j < p.size(); j++) {
				str += to_string(p[i][j]);
			}
		}
		return str;
	}


private:
	unordered_map<string, node*> open_list;
	unordered_map<string, node*> closed_list;
	vector<vector<int>> start_state;
	vector<vector<int>> goal_state;
	vector<vector<vector<int>>> spuzzle;

	node* current;
	node* root;

	bool solved = false;
};



int main() {
	vector<vector<int>> start_puzzle = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 0 } };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << "Enter the " << i * 4 + j + 1 << ". number: ";
			cin >> start_puzzle[i][j];
			cout << endl;
		}
	}

	int start_s = clock();
	Npuzzle init(start_puzzle);
	init.solve_puzzle();
	vector<vector<vector<int>>> spuzzle = init.get_spuzzle();

	for (int i = 0; i < spuzzle.size(); i++) {
		display_puzzle(spuzzle[i]);
	}
	cout << "Steps: " << spuzzle.size() << endl;
	cout << "the end" << endl;
	int stop_s = clock();
	cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << endl;
}

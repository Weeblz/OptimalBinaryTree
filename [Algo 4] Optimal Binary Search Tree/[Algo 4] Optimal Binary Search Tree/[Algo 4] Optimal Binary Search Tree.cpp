#include "stdafx.h"
#include <vector>

class Node {
	Node* left;
	Node* right;
	Node* parent;
	int value;
	int frequency;

public:
	Node() : left(nullptr), right(nullptr), parent(nullptr), value(0), frequency(0) {}
	Node(int v) : left(nullptr), right(nullptr), parent(nullptr), value(v), frequency(0) {}
	void setLeft(Node* l) {
		left = l;
		l->parent = this;
	}
	void setRight(Node* r) {
		right = r;
		r->parent = this;
	}
	void setValue(int v) { value = v; }
	Node* getLeft() { return left; }
	Node* getRight() { return right; }
};

std::vector<std::vector<std::pair<int, int>>> costMatrix;
std::vector<int> keys{ 5,6,8,20 };
std::vector<int> freq{ 4,2,6,3 };

void build(Node* root, int begin, int end) {
	if (end < begin) return;
	root->setValue(keys[costMatrix[begin][end].second]);
	root->setLeft(new Node());
	root->setRight(new Node());
	build(root->getLeft(), begin, costMatrix[begin][end].second - 1);
	build(root->getRight(), costMatrix[begin][end].second + 1, end);
}

int sum(std::vector<int> freq, int i, int j) {
	int s = 0;
	for (int k = i; k <= j; k++)
		s += freq[k];
	return s;
}

Node* optimalSearchTree(const std::vector<int>& keys, const std::vector<int>& freq) {
	int n = keys.size();
	std::vector<std::vector<std::pair<int, int>>> cost(n, std::vector<std::pair<int, int>>(n, std::make_pair(0,0)));

	for (int i = 0; i < n; i++) {
		cost[i][i] = std::make_pair(freq[i], i);
	}

	for (int L = 2; L <= n; L++) {
		for (int i = 0; i < n - L + 1; i++)	{
			int j = i + L - 1;
			cost[i][j] = std::make_pair(INT_MAX, INT_MAX);

			for (int r = i; r <= j; r++) {
				int c = ((r > i) ? cost[i][r - 1].first : 0) + ((r < j) ? cost[r + 1][j].first : 0) + sum(freq, i, j);
				if (c < cost[i][j].first) {
					cost[i][j] = std::make_pair(c, r);
				}
			}
		}
	}

	costMatrix = cost;

	Node* root = new Node();
	build(root, 0, n - 1);

	return root;
}

int main() {
	Node* root = new Node();
	
	root = optimalSearchTree(keys, freq);
	system("pause");
	return 0;
}
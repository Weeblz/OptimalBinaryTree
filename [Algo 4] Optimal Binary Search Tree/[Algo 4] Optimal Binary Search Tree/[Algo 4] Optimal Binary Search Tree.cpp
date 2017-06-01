#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

class student;
class group {
public:
	std::string name;
	std::vector<student*> groupList;

	group(std::string a) {
		name = a;
	}
};

class student {
public:
	std::string name;
	std::string homeCountry;
	std::string homeCity;
	double rating;
	int frequency;
	group* specialization;

	student(std::string n, std::string c, std::string c_, double r, group* s) : name(n), homeCountry(c), homeCity(c_), rating(r), specialization(s), frequency(0) {}

	void show() const {
		std::cout << "Student Name: " << name << "\nStudent Country: " << homeCountry << "\nStudent City: " << homeCity << "\nStudent Grade: " << rating << std::endl;
	}

	bool operator<(const student &right) const { return name < right.name; }

	friend std::ostream& operator<<(std::ostream& output, const student &st);
};

std::ostream& operator<<(std::ostream& output, const student &st) {
	output << "[" << st.name << "]";

	return output;
}

class Node {
	Node* left;
	Node* right;
	Node* parent;
	student* value;

public:
	Node() : left(nullptr), right(nullptr), parent(nullptr), value(0) {}
	Node(student* v) : left(nullptr), right(nullptr), parent(nullptr), value(v) {}
	~Node() {
		if (left) delete left;
		if (right) delete right;
	}

	void setLeft(Node* l) {
		if (left) delete left;
		left = l;
		if(l) l->parent = this;
	}
	void setRight(Node* r) {
		if (right) delete right;
		right = r;
		if(r) r->parent = this;
	}
	void frequencyIncrement() { value->frequency++; }
	void setValue(student* v) { value = v; }
	void setParent(Node* p, bool left) {
		parent = p;
		if (p) {
			left ? parent->setLeft(this) : parent->setRight(this);
		}
	}
	student* getValue() const { return value; }
	Node* getLeft() const { return left; }
	Node* getRight() const { return right; }

	friend std::ostream& operator<<(std::ostream& output, const Node &node);
};

std::ostream& operator<<(std::ostream& output, const Node &node) {

	output << "(";
	if (node.value) output << *node.value; else output << "()";
	output << " ";
	if (node.left) output << *node.left; else output << "()";
	output << " ";
	if (node.right) output << *node.right; else output << "()";
	output << ")";

	return output;
}

std::vector<std::vector<std::pair<int, int>>> costMatrix;

void build(const std::vector<student*> Students, bool left, Node* parent, int begin, int end) {
	if (end < begin) return;
	Node* temp = new Node(Students[costMatrix[begin][end].second]);
	temp->setParent(parent, left);
	build(Students, 1, temp, begin, costMatrix[begin][end].second - 1);
	build(Students, 0, temp, costMatrix[begin][end].second + 1, end);
}

int sum(const std::vector<student*>& Students, int i, int j) {
	int s = 0;
	for (int k = i; k <= j; k++)
		s += Students[k]->frequency;
	return s;
}

Node* optimalSearchTree(const std::vector<student*>& Students) {
	int n = Students.size();
	std::vector<std::vector<std::pair<int, int>>> cost(n, std::vector<std::pair<int, int>>(n, std::make_pair(0,0)));

	for (int i = 0; i < n; i++) {
		cost[i][i] = std::make_pair(Students[i]->frequency, i);
	}

	for (int L = 2; L <= n; L++) {
		for (int i = 0; i < n - L + 1; i++)	{
			int j = i + L - 1;
			cost[i][j] = std::make_pair(INT_MAX, INT_MAX);

			for (int r = i; r <= j; r++) {
				int c = ((r > i) ? cost[i][r - 1].first : 0) + ((r < j) ? cost[r + 1][j].first : 0) + sum(Students, i, j);
				if (c < cost[i][j].first) {
					cost[i][j] = std::make_pair(c, r);
				}
			}
		}
	}

	costMatrix = cost;

	Node* root = new Node(Students[costMatrix[0][n-1].second]);
	build(Students, 1, root, 0, costMatrix[0][n-1].second - 1);
	build(Students, 0, root, costMatrix[0][n-1].second + 1, n-1);

	return root;
}

bool comp(student* l, student* r) {
	return l->name < r->name;
}

Node* search(Node* root, std::string studentName) {
	Node* current = root;
	while (current) {
		if (studentName == current->getValue()->name) {
			current->frequencyIncrement();
			return current;
		}
		else if (studentName < current->getValue()->name) {
			current = current->getLeft();
		}
		else {
			current = current->getRight();
		}
	}
	
	return nullptr;
}

int main() {
	std::ifstream in("base.txt");
	std::string temp, tempCountry, tempCity, tempSpec, tempR;
	double tempRating;

	std::vector<group*> Groups(10);
	std::vector<student*> Students(5);

	for (int i = 0; i < 10; i++) {
		getline(in, temp);
		Groups[i] = new group(temp);
	}

	getline(in, temp);

	for (int i = 0; i < 5; i++) {
		getline(in, temp);
		getline(in, tempR);
		getline(in, tempCity);
		getline(in, tempCountry);
		getline(in, tempSpec);
		tempRating = atof(tempR.c_str());
		Students[i] = new student(temp, tempCountry, tempCity, tempRating, new group(tempSpec));
		for (int j = 0; j < 10; j++) {
			if (Groups[j]->name == temp) {
				Groups[j]->groupList.push_back(Students[i]);
			}
		}
		getline(in, temp);
	}

	sort(Students.begin(), Students.end(), comp);

	std::string pendingSearch;

	while (true) {
		getline(std::cin, pendingSearch);
		Node* root = optimalSearchTree(Students);
		Node* searchResult = search(root, pendingSearch);
		if (searchResult) searchResult->getValue()->show();
		else std::cout << "Not found!" << std::endl;
		if (searchResult) std::cout << *searchResult;
		std::cout << std::endl << *root << std::endl;
		delete root;
	}
	
	system("pause");
	return 0;
}
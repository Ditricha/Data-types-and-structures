#pragma

#ifndef HUFFTREE_H
#define HUFFTREE_H

#include <iostream>
#include <queue>
#include <map>
#include <climits> // for CHAR_BIT
#include <iterator>
#include <algorithm>
#include <fstream>
#include <string>

using namespace std;

class INode
{
public:
	const int f;
	virtual ~INode() {}

protected:
	INode(int f) : f(f) {}
};

class InternalNode : public INode
{
public:
	INode *const left;
	INode *const right;

	InternalNode(INode* in_Left, INode* in_Right) : INode(in_Left->f + in_Right->f), left(in_Left), right(in_Right) {}
	~InternalNode()
	{
		delete left;
		delete right;
	}
};

class LeafNode : public INode
{
public:
	const char c;
	LeafNode(int f, char c) : INode(f), c(c) {}
};


struct NodeCmp
{
	bool operator()(const INode* lhs, const INode* rhs) const { return lhs->f > rhs->f; }
};

INode* build_tree(ifstream& input)
{
	map <char, int> letter_frequency;
	map<char, int>::iterator i_letter;
	char c;
	while (!input.eof()) {
		input.get(c);
		if (c != ('\n')) {
			if (letter_frequency.find(c) == letter_frequency.end()) {
				letter_frequency.insert(pair<char, int>(c, 1));
			}
			else {
				i_letter = letter_frequency.find(c);
				i_letter->second++;
			}
		}
	}

	letter_frequency.insert(pair<char, int>('^', 1));
	priority_queue<INode*, vector<INode*>, NodeCmp> trees;

	for (i_letter = letter_frequency.begin(); i_letter != letter_frequency.end(); i_letter++) {
		trees.push(new LeafNode(i_letter->second, i_letter->first));
	}

	while (trees.size() > 1) {
		INode* left = trees.top();
		trees.pop();

		INode* right = trees.top();
		trees.pop();

		INode* parent = new InternalNode(left, right);
		trees.push(parent);
	}
	return trees.top();
}

#endif

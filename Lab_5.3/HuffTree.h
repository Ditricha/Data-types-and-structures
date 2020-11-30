#pragma
#include <iostream>
#include <queue>
#include <map>
#include <climits>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <string>

#ifndef HUFFTREE_H
#define HUFFTREE_H

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

	InternalNode(INode* inLeft, INode* inRight) : INode(inLeft->f + inRight->f), left(inLeft), right(inRight) {}
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

INode* Build_Tree(ifstream& input)
{
	map <char, int> letterFrequency;
	map<char, int>::iterator i_letter;
	char c;
	while (!input.eof()) {
		input.get(c);
		if (c != ('\n')) {
			if (letterFrequency.find(c) == letterFrequency.end()) {
				letterFrequency.insert(pair<char, int>(c, 1));
			}
			else {
				i_letter = letterFrequency.find(c);
				i_letter->second++;
			}
		}
	}

	letterFrequency.insert(pair<char, int>('^', 1));
	priority_queue<INode*, vector<INode*>, NodeCmp> trees;

	for (i_letter = letterFrequency.begin(); i_letter != letterFrequency.end(); i_letter++) {
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

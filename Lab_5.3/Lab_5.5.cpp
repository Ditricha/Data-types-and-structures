#include <string>
#include <iostream>
#include <algorithm>

#include "HuffTree.h"
#include "HuffCode.h"



using namespace std;

int main()
{
	ifstream input;
	input.open("input.txt");
	INode* root = Build_Tree(input);
	input.close();

	string code;
	HuffCode hc;
	hc.Generate_Codes(root, code);

	ofstream output;
	output.open("encoded.txt");

	input.open("input.txt");
	string encodedString = hc.encode(input);
	output << encodedString;
	output.close();
	input.close();

	/*cout << encodedString << endl;*/

	string decodedString;
	hc.decodeFromRoot(root, encodedString, decodedString);

	output.open("decoded.txt");
	output << decodedString;
	output.close();

	getchar();

	return 0;
}

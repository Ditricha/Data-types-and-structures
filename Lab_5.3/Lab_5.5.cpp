#include <string>
#include <iostream>
#include <algorithm>

#include "HuffTree.h"
#include "HuffCode.h"



using namespace std;

int main()
{
	ifstream input;
	input.open("input.txt");  //  The input file
	INode* root = BuildTree(input);
	input.close();

	string code;
	HuffCode hc;
	hc.GenerateCodes(root, code);

	ofstream output;
	output.open("encodedFile.txt");

	input.open("input.txt");
	string encodedString = hc.encode(input);
	output << encodedString;
	output.close();
	input.close();

	/*cout << encodedString << endl;*/

	string decodedString;
	hc.decodeFromRoot(root, encodedString, decodedString);

	output.open("decodedFile.txt");
	output << decodedString;
	output.close();

	getchar();

	return 0;
}
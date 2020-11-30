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
	INode* root = build_tree(input);
	input.close();

	string code;
	HuffCode hc;
	hc.generate_codes(root, code);

	ofstream output;
	output.open("encoded.txt");

	input.open("input.txt");
	string encoded_string = hc.encode(input);
	output << encoded_string;
	output.close();
	input.close();

	string decoded_string;
	hc.decode_from_root(root, encoded_string, decoded_string);

	output.open("decoded.txt");
	output << decoded_string;
	output.close();

	getchar();

	return 0;
}

#pragma
#include "HuffTree.h"
#include <bitset>

using namespace std;

typedef map<char, string> huff_code_map;
class HuffCode
{
public:
	HuffCode();
	huff_code_map codes;
	void decode_from_root(const INode* root, string encoded_string, string &decoded_string);
	void generate_codes(const INode* node, const string code);
	string encode(ifstream& input);
private:
	void decode(const INode* root, const INode* node, string encoded_string, string &decoded_string);
};

HuffCode::HuffCode()
{
}

void HuffCode::generate_codes(const INode* node, const string code)
{
	if (const LeafNode* lf = dynamic_cast<const LeafNode*>(node)) {
		codes[lf->c] = code;
		cout << code << " - " << lf->c << endl;
	}
	else if (const InternalNode* in = dynamic_cast<const InternalNode*>(node)) {
		string left_Code = code;
		left_Code.append("0");
		generate_codes(in->left, left_Code);

		string right_Code = code;
		right_Code.append("1");
		generate_codes(in->right, right_Code);
	}
}

string HuffCode::encode(ifstream& input)
{
	map<char, string>::iterator i_code = codes.begin();
	string encoded_string;
	char c;

	while (input.get(c)) {
		i_code = codes.find(c);
		encoded_string.append(i_code->second);
	}
	i_code = codes.find('^');
	encoded_string.append(i_code->second);
	return encoded_string;
}


void HuffCode::decode_from_root(const INode* root, string encoded_string, string &decoded_string)
{
	const INode* node = root;
	if (const InternalNode* in = dynamic_cast<const InternalNode*>(node)) {
		if (encoded_string[0] == '0') {
			encoded_string.erase(encoded_string.begin());
			decode(root, in->left, encoded_string, decoded_string);
		}
		else {
			encoded_string.erase(encoded_string.begin());
			decode(root, in->right, encoded_string, decoded_string);
		}
	}
}
void HuffCode::decode(const INode* root, const INode* node, string encoded_string, string &decoded_string)
{
	if (const LeafNode* lf = dynamic_cast<const LeafNode*>(node)) {
		if (lf->c != '^') {
			decoded_string += lf->c;
			decode_from_root(root, encoded_string, decoded_string);
		}
	}
	else if (const InternalNode* in = dynamic_cast<const InternalNode*>(node)) {
		if (encoded_string[0] == '0') {
			encoded_string.erase(encoded_string.begin());
			decode(root, in->left, encoded_string, decoded_string);
		}
		else {
			encoded_string.erase(encoded_string.begin());
			decode(root, in->right, encoded_string, decoded_string);
		}
	}
}

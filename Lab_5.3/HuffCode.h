#pragma


#include "HuffTree.h"
#include <bitset>

using namespace std;

typedef map<char, string> HuffCodeMap;
class HuffCode
{
public:
	HuffCode();
	HuffCodeMap codes;
	void decodeFromRoot(const INode* root, string encodedString, string &decodedString);
	void GenerateCodes(const INode* node, const string code);
	string encode(ifstream& input);
private:
	void decode(const INode* root, const INode* node, string encodedString, string &decodedString);
};

HuffCode::HuffCode()
{
}

void HuffCode::GenerateCodes(const INode* node, const string code)
{
	if (const LeafNode* lf = dynamic_cast<const LeafNode*>(node))
	{
		codes[lf->c] = code;
		cout << code << " - " << lf->c << endl;
	}
	else if (const InternalNode* in = dynamic_cast<const InternalNode*>(node))
	{
		string leftCode = code;
		leftCode.append("0");
		GenerateCodes(in->left, leftCode);

		string rightCode = code;
		rightCode.append("1");
		GenerateCodes(in->right, rightCode);
	}
}

string HuffCode::encode(ifstream& input)
{
	map<char, string>::iterator i_code = codes.begin();
	string encodedString;
	char c;

	while (input.get(c))
	{
		i_code = codes.find(c);
		encodedString.append(i_code->second);
	}

	i_code = codes.find('^');
	encodedString.append(i_code->second);
	return encodedString;
}


void HuffCode::decodeFromRoot(const INode* root, string encodedString, string &decodedString)
{
	const INode* node = root;
	if (const InternalNode* in = dynamic_cast<const InternalNode*>(node))
	{
		if (encodedString[0] == '0')
		{
			encodedString.erase(encodedString.begin());
			decode(root, in->left, encodedString, decodedString);
		}
		else
		{
			encodedString.erase(encodedString.begin());
			decode(root, in->right, encodedString, decodedString);
		}
	}
}
void HuffCode::decode(const INode* root, const INode* node, string encodedString, string &decodedString)
{
	if (const LeafNode* lf = dynamic_cast<const LeafNode*>(node))
	{
		if (lf->c != '^')
		{
			decodedString += lf->c;
			decodeFromRoot(root, encodedString, decodedString);
		}

	}
	else if (const InternalNode* in = dynamic_cast<const InternalNode*>(node))
	{
		if (encodedString[0] == '0')
		{
			encodedString.erase(encodedString.begin());
			decode(root, in->left, encodedString, decodedString);
		}
		else
		{
			encodedString.erase(encodedString.begin());
			decode(root, in->right, encodedString, decodedString);
		}
	}
}

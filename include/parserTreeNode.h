#ifndef _PARSER_TREE_Node_
#define _PARSER_TREE_Node_

#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <stdarg.h>
using namespace std;

// class Element{
// public:
//     Element(string eleName, string eleVal);
//     Element(Element *ele);
//     string eleName;
//     string eleVal;
// };

class Node{
public:
	
    Node(string type, int eleNum,...);

    Node(string type, string val);

    string getType();

    string getVal();
    vector<Node*> children;

private:
    string type;
    // Element *val;
    string val;
	int line;
};

#endif
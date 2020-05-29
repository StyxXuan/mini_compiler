#include <iostream>
#include "parserTreeNode.h"
#include <list>
#include <fstream>

using namespace std;

class saveTree{
public:
    saveTree(Node* node);
    void addJson(string line);
    void genJson(int level, Node *node, bool hasSib);
    void writeToFile(string filePath);

private:
    Node *root;
    list<string> jsonLine;
};
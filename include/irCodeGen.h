#ifndef _IRCODEGEN_H_
#define _IRCODEGEN_H_
#include "parserTreeNode.h"
#include "block.h"
#include <string>
#include <map>
#include <list>
#include <fstream>
#define DEBUG true

class irCodeGenerator{
public:
    irCodeGenerator();
    void genIrParserTree(Node *root);

    void genIrGlobalVal(Type type, Node* node);
    void genIrFunDec(Type type, Node* decList, Node *compSt);


    void genIrCompSt(funNode *func, Node* node);
    void genIrLocalDef(funNode *func, Node *node);
    void genIrDef(funNode *func, Node* node);
    void genIrDecList(funNode *func, Type type, Node *node);
    void genIrDec(funNode *func, Type type, Node *node);

    void genIrVarList(funNode *func, Node *node);
    void genIrParamDec(funNode *func, Node *node);

    void genIrStateList(funNode *func, Node *node);
    void genIrState(funNode *func, Node *node);

    void genIrArgs(funNode* func, Node *node);

    C_Value genIrExp(funNode *func, Node *node);
    void genIrIf(funNode *func, Node *node);
    void genIrIfElse(funNode *func, Node *node);
    void genIrWhile(funNode *func, Node *node);
    void genIrReturn(funNode *func, Node *node);

    Type getType(Node *node);
    void insertQuad(Quad quad);

    string getTempVar();
    void releaseTemp();
    string getLabel();

    // 0 for in global, 1 for in local, 2 for in paralist, -1 for not found
    int findVar(funNode* func, string key);
    string toVarName (string ID);
    string addID(string ID);
    void writeToFile(string filePath);

private:
    // void writeIrFile();
    map<string, funNode*> funcNodes;
    map<string, varNode*> varNodes;
    map<string, string> IDVarMap;
    
    list<Quad> IrCodes;
    int lineNum;
    int tempVarNum;
    int labelNum;
    int VarNum;
};

#endif

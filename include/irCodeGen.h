#ifndef _IRCODEGEN_H_
#define _IRCODEGEN_H_
#include "parserTreeNode.h"
#include "block.h"
#include <string>
#include <map>
#include <list>
#include <stdlib.h>
#include <fstream>
#define DEBUG false

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

    strNode* genIrStructSpecifier(Node* node);
    void genIrStrDefList(strNode* snode, Node* node);


    void genIrVarList(funNode *func, Node *node);
    void genIrParamDec(funNode *func, Node *node);

    void genIrStateList(funNode *func, Node *node);
    void genIrState(funNode *func, Node *node);

    void genIrArgs(funNode* func, Node *node);

    C_Value genIrExp(funNode *func, Node *node);
    void genIrIf(funNode *func, Node *node);
    void genIrIfElse(funNode *func, Node *node);
    void genIrWhile(funNode *func, Node *node);
    void genIrFor(funNode *func, Node *node);
    void genIrDoWhile(funNode *func, Node *node);
    void genIrReturn(funNode *func, Node *node);
    varNode* genIrArray(Type type, Node *node);

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

    void addvarNode(varNode* vnode){
        this->varNodes.insert(pair<string, varNode*>(vnode->name, vnode));
    }
    void addStrNode(strNode* snode){
        this->strMap.insert(pair<string, strNode*>(snode->name, snode));
    }

    void addArrayNode(arrayNode* anode){
        this->arrayMap.insert(pair<string, arrayNode*>(anode->name, anode));
    }

private:
    // void writeIrFile();
    map<string, funNode*> funcNodes;
    map<string, varNode*> varNodes;
    map<string, string> IDVarMap;
    map<string, strNode*> strMap;
    map<string, arrayNode*> arrayMap;
    
    list<Quad> IrCodes;
    int lineNum;
    int tempVarNum;
    int labelNum;
    int VarNum;
};

#endif

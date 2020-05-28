#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;
union valType{
    int intVal;
    float floatVal; 
    bool isVoid;
};

enum Type{
    INT_TYPE, FLOAT_TYPE, VOID_TYPE, Identifier
};

class C_Value{
public:
    C_Value(valType val, Type type){
        this->val = val;
        this->type = type;
        this->varName = "";
    }
    C_Value(){};

    valType val;
    Type type;
    string varName;
};


class varNode{
public:
    varNode(string name, Type type){
        this->name = name;
        this->type = type;
        if(type == INT_TYPE){
            this->varValue.intVal = 0;
        }else if(type == FLOAT_TYPE){
            this->varValue.floatVal = 0;
        }else if(type == VOID_TYPE){
            this->varValue.isVoid = true;
        }
    }

    varNode(string name, Type type, valType val){
        this->name = name;
        this->type = type;
        this->varValue = val;
    }

    void setVal(valType val){
        this->varValue = val;
    }

    string name;
    Type type;
    valType varValue;
};



class funNode{
public:
    funNode(string name, Type type, int lineNum){
        this->name = name;
        this->type = type;
        this->lineNum = lineNum;
        int paraNum = 0;
    }

    
    void addLocalVar(varNode* vnode){
        this->localVar.insert(pair<string, varNode*>(vnode->name, vnode));
    }

    void addParaList(varNode* vnode){
        string paramName = "#" + to_string(paraNum++);
        this->paralist.insert(pair<string, string>(vnode->name, paramName));
    }


    string name;
    Type type;
    int lineNum;
    map<string, string> paralist;
    map<string, varNode*> localVar;
    int paraNum;
};

// typedef struct{
//     string name;
//     string type;
//     vector<valType> vals;
// }arrayNode;

class Quad{
public:
    Quad(string add1, string add2, string add3, string Op){
        this->add1 = add1;
        this->add2 = add2;
        this->add3 = add3;
        this->Op = Op;
    };

    string printQuad(){
        string ir;
        if(add1.compare("IF_NOT") && add1.compare("")){
            ir += add1 + " := " + add2 + " " + Op + " " + add3 + "\n";
        }else{
            ir += add1 + " " + add2 + " " + add3 + " " + Op + "\n"; 
        }
        return ir;   
    }

    string add1;
    string add2;
    string add3;
    string Op;
};

#endif
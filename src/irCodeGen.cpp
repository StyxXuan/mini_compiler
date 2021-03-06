#include "irCodeGen.h"

irCodeGenerator::irCodeGenerator(){
    lineNum = 0;
    tempVarNum = 0;
    labelNum = 0;
    VarNum = 0;
}

// generate IR for the hole parse tree
void irCodeGenerator::genIrParserTree(Node* node){
    if(DEBUG)
        cout << ">> genIrParserTree" << endl;
    if(node == NULL || !node->getType().compare("empty"))
        return;
    
    if(!node->getType().compare("ExtDefList")){
        genIrParserTree(node->children[0]);
        genIrParserTree(node->children[1]);
    }else if(!node->getType().compare("ExtDef")){
        Type type;
        if(node->children[0]->getVal().compare("INT")){
            type = INT_TYPE;
        }else{
            type = FLOAT_TYPE;
        }

        // cout << node->children[1]->getType() << " << "<< endl;
        if(node->children.size()<3){ 
            // global viriable
            genIrGlobalVal(type, node->children[1]);
        }else{
            genIrFunDec(type, node->children[1], node->children[2]);
        }
    }else if(!node->getType().compare("ExtDefStr")){
        strNode *snode = genIrStructSpecifier(node->children[0]);
        // Quad quad("", "struct:", snode->name, to_string(snode->memVarNum));
        // this->insertQuad(quad);
    }
}

strNode* irCodeGenerator::genIrStructSpecifier(Node* node){
    if(DEBUG)
        cout << ">> genIrStructSpecifier" << endl;
    
    

    string ID = node->children[0]->getVal();
    strNode *snode;
    if(node->children.size() > 1){
        snode = new strNode(ID);
        genIrStrDefList(snode, node->children[1]);
        this->addStrNode(snode);
    }else{
        snode = this->strMap[ID];
    }
    if(DEBUG)
        cout << "<< end genIrStructSpecifier" << endl;

    return snode;
}

// DefList -> Def DefList | empty
void irCodeGenerator::genIrStrDefList(strNode* snode, Node* node){
    if(DEBUG)
        cout << ">> genIrStrDefList" << endl;

    if(!node->getType().compare("empty"))
        return;

    if(node->children.size()>1){
        genIrStrDefList(snode, node->children[1]);
    }

    Type type = getType(node->children[0]->children[0]);
    string id = node->children[0]->children[1]->children[0]->children[0]->getVal();
    // cout << "var id" << node->children[0]->children[1]->children[0]->getType() << endl;
    // cout << "var id" << id << endl;
    varNode* vnode = new varNode(id, type);
    snode->addVar(vnode);
    if(DEBUG)
        cout << "<< end genIrStrDefList" << endl;
}

// ExtDecList -> VarDec | VarDec, ExtDecList
void irCodeGenerator::genIrGlobalVal(Type type, Node* node){
    if(DEBUG)
        cout << ">> genIrGlobalVal" << endl;

    if(node->children.size() > 1){
        varNode* vnode = genIrArray(type, node->children[0]);
        this->addvarNode(vnode);
    }else{
        string ID = node->children[0]->getVal();
        varNode* vnode = new varNode(ID, type);
        this->varNodes.insert(pair<string, varNode*>(ID, vnode));
        string varName = addID(ID);
        Quad quad("", "global:", varName, "");
        this->insertQuad(quad);
    }
    
    if(node->children.size() > 1){
        genIrGlobalVal(type, node->children[1]);

    }
}

// ExtDef -> Specifier FunDec CompSt
// FunDec -> ID (VarList)
void irCodeGenerator::genIrFunDec(Type type, Node* FunDec, Node *compSt){
    if(DEBUG)
        cout << ">> genIrFunDec" << endl;

    string ID = FunDec->children[0]->getVal();
    map<string, varNode*> decVarList;
    funNode* func = new funNode(ID, type, this->lineNum);
    this->funcNodes.insert(pair<string, funNode*>(ID, func));

    if(FunDec->children.size() > 1){
        genIrVarList(func, FunDec->children[1]);
    }

    Quad quad("", "Func:", ID, "");
    this->insertQuad(quad);
    genIrCompSt(func, compSt);
}

// VarList -> ParamDec, VarList | ParamDec
void irCodeGenerator::genIrVarList(funNode *func, Node *node){
    if(DEBUG)
        cout << ">> genIrVarList, " << node->children.size() << endl;

    if(node->children.size()==0)
        return;

    if(node->children.size() > 1){
        genIrVarList(func, node->children[1]);
    }

    genIrParamDec(func, node->children[0]);
}

// ParamDec -> Specifier VarDec
void irCodeGenerator::genIrParamDec(funNode *func, Node *node){
    // :TYPE VarDec
    if(DEBUG)
        cout << ">> genIrParamDec" << endl;
        
    if(!node->children[0]->getType().compare("TYPE")){
        Type type = this->getType(node->children[0]);
        if(node->children[1]->children.size()>1){
            varNode* vnode = genIrArray(type, node->children[0]);
            func->addLocalVar(vnode);
        }else{
            string ID = node->children[1]->getVal();
            varNode* vnode = new varNode(ID, type);
            func->addParaList(vnode);
        }    
    }else{
        Type type = STRUCT_TYPE;
        string ID = node->children[1]->getVal();
        varNode* vnode = new varNode(ID, type);
        Node* strSpecifer = node->children[0];
         cout <<"here now" << endl; 
        strNode *snode = new strNode(genIrStructSpecifier(strSpecifer));
        cout <<"here now" << endl; 
        strVar *svar = new strVar(ID, snode);
        func->addStrVar(svar);
    }
}


// CompSt -> DefList StmtList
void irCodeGenerator::genIrCompSt(funNode *func, Node* node){
    if(DEBUG)
        cout << ">> genIrCompSt " << node->children.size() << endl;

    for(int i=0; i<node->children.size(); i++){
        Node *child = node->children[i];
        if(!child->getType().compare("DefList")){
            genIrLocalDef(func, child);
        }else{
            genIrStateList(func, child);
        }
    }

    if(DEBUG)
        cout << ">> end genIrCompSt " << node->children.size() << endl;
}

// DefList -> Def DefList | empty
void irCodeGenerator::genIrLocalDef(funNode *func, Node* node){
    if(DEBUG)
        cout << ">> genIrLocalDef __" << node->getType() << node->children.size() << endl;
    
    if(!node->getType().compare("empty"))
        return;
    
    genIrDef(func, node->children[0]);

    if(node->children.size() > 1)
        genIrLocalDef(func, node->children[1]);

    if(DEBUG)
        cout << ">> end genIrLocalDef " << node->children.size() << endl;
}

// Def->Specifier DecList;
void irCodeGenerator::genIrDef(funNode *func, Node* node){
    if(DEBUG)
        cout << ">> genIrDef " << node->children.size() << endl;

    if(!node->children[0]->getType().compare("TYPE")){
        Type type = getType(node->children[0]);
        genIrDecList(func, type, node->children[1]);
    }else{
        strNode* snode = genIrStructSpecifier(node->children[0]);
        // for struct, only one dec once
        string ID = node->children[1]->children[0]->children[0]->getVal();
        strVar *svar = new strVar(ID, snode);
        map<string, int>::iterator iter;

        for(iter=snode->varList.begin(); iter!=snode->varList.end(); iter++){
            string VarName = ID + "." + iter->first; 
            // cout <<  "addID: " << VarName << endl;
            string varName = addID(VarName);
            Quad quad("", "var:", varName, "");
            this->insertQuad(quad);
        }

        func->addStrVar(svar);
        // Quad quad("", "struct: "+snode->name, ID, "");
        // this->insertQuad(quad);
    }

    if(DEBUG)
        cout << ">> end genIrDef" << endl;
}

// // DecList -> Dec | Dec, DecList
void irCodeGenerator::genIrDecList(funNode *func, Type type, Node* node){
    if(DEBUG)
        cout << ">> genIrDecList" << endl;

    if(node->children.size()>1){
        genIrDecList(func, type, node->children[1]);
    }

    genIrDec(func, type, node->children[0]);
    if(DEBUG)
        cout << ">> end genIrDecList" << endl;
}

// Dec -> VarDec | VarDec = Exp
void irCodeGenerator::genIrDec(funNode *func, Type type, Node *node){
    if(DEBUG)
        cout << ">> genIrDec " << node->children.size() << endl;

    
    varNode* vnode;
    if(node->children[0]->children.size()>1){
        varNode* vnode = genIrArray(type, node->children[0]);
    }else{
        string ID = node->children[0]->getVal();
        string varName = addID(ID);
        vnode = new varNode(ID, type);
        Quad quad("", "var:", varName, "");
        this->insertQuad(quad);

        if(node->children.size()>1){
            C_Value value = genIrExp(func, node->children[1]);
            vnode->setVal(value.val);
            Quad quad(varName, value.varName, "", "");
            this->insertQuad(quad);
        }
    }
    func->addLocalVar(vnode);
    if(DEBUG)
        cout << ">> end genIrDec " << node->children.size() << endl;
}

// StmtList -> Stmt StmtList
void irCodeGenerator::genIrStateList(funNode* func, Node *node){
    if(DEBUG)
        cout << ">> genIrStateList" << endl;

    if(!node->getType().compare("empty"))
        return;
    
    genIrState(func, node->children[0]);
    if(node->children.size()>1){
        genIrStateList(func, node->children[1]);
    }
}

// Stmt -> Exp; | RETURN Exp | IF(Exp) stmt | IF(EXP) Stmt ELSE Stmt 
// | WHILE (Exp) Stmt | CompSt  | For | do_while
void irCodeGenerator::genIrState(funNode* func, Node *node){
    if(DEBUG)
        cout << ">> genIrState" << endl;

    if(!node->getType().compare("Stmt_exp")){
        genIrExp(func, node->children[0]);
    }else if(!node->getType().compare("Stmt_return")){
        genIrReturn(func, node->children[0]);
    }else if(!node->getType().compare("Stmt_if")){
        genIrIf(func, node);
    }else if(!node->getType().compare("Stmt_if_else")){
        genIrIfElse(func, node);
    }else if(!node->getType().compare("Stmt_while")){
        genIrWhile(func, node);
    }else if(!node->getType().compare("CompSt")){
        genIrCompSt(func, node);
    }else if(!node->getType().compare("Stmt_for")){
        genIrFor(func, node);
    }else if(!node->getType().compare("Stmt_do_while")){
        genIrDoWhile(func, node);
    }
}

// Exp -> Exp ASSIGNOP Exp | Exp AND Exp | Exp OR Exp				
// 	| Exp RELOP Exp	| Exp PLUS Exp	| Exp MINUS Exp				
//     | Exp STAR Exp	| Exp DIV Exp	| LP Exp RP	
//     | MINUS Exp	| NOT Exp | ID LP Args RP
// 	| ID LP RP	| Exp LB Exp RB	 | Exp DOT ID	
//     | ID | INT	| FLOAT	

C_Value irCodeGenerator::genIrExp(funNode* func, Node *node){
    if(DEBUG)
        cout << ">> genIrExp" << endl;

    C_Value value;
    if(!node->getType().compare("INT")){
        int number;
        sscanf(node->getVal().c_str(), "%d", &number);
        value.val.intVal = number;
        value.type = INT_TYPE;
        value.varName = getTempVar();
        Quad quad(value.varName, to_string(number), "", "");
        this->insertQuad(quad);
        return value;
    }else if(!node->getType().compare("FLOAT")){
        float number;
        sscanf(node->getVal().c_str(), "%f", &number);
        value.val.floatVal = number;
        value.type = FLOAT_TYPE;
        value.varName = getTempVar();
        Quad quad(value.varName, to_string(number), "", "");
        this->insertQuad(quad);
        return value;
    }else if(!node->getType().compare("ID")){
        string ID = node->getVal();
        int place = findVar(func, ID);
        string varName;
        switch (place)
        {
        case 2:
            varName = func->paralist[ID]; break;
        default:
            varName = toVarName(ID); break;
        }
        value.varName = varName;
        return value;
    }else if(!node->getType().compare("Ass_Exp")){
        C_Value exp1 = genIrExp(func, node->children[0]);
        C_Value exp2 = genIrExp(func, node->children[1]);
        Quad quad(exp1.varName, exp2.varName, "", "");
        this->insertQuad(quad);
        return value;
    }else if(!node->getType().compare("Ternary_Exp")){
        C_Value exp1 = genIrExp(func, node->children[0]);
        C_Value exp2 = genIrExp(func, node->children[2]);
        value.varName = getTempVar();
        Quad quad(value.varName, exp1.varName, exp2.varName, node->children[1]->getVal());
        this->insertQuad(quad);
        return value;
    }else if(!node->getType().compare("Binary_Exp")){
        C_Value exp1 = genIrExp(func, node->children[1]);
        value.varName = getTempVar();
        Quad quad(value.varName, "", exp1.varName, node->children[0]->getVal());
        this->insertQuad(quad);
        return value;
    }else if(!node->getType().compare("Fun_Arg_Exp")){
        if (!node->children[0]->getVal().compare("Print"))
        {
            C_Value exp = genIrExp(func, node->children[1]->children[0]);
            Quad quad("", "Print", exp.varName, "");
            this->insertQuad(quad);
            return exp;
        }
        genIrArgs(func, node->children[1]);
        value.varName = getTempVar();
        Quad quad(value.varName, "CALL", node->children[0]->getVal(), "");
        this->insertQuad(quad);
        return value;
    }else if(!node->getType().compare("Fun_Exp")){
        value.varName = getTempVar();
        Quad quad(value.varName, "CALL", node->children[0]->getVal(), "");
        this->insertQuad(quad);
        return value;
    }else if(!node->getType().compare("Dot_Exp")){
        string ID_1 = node->children[0]->getVal();
        string ID_2 = node->children[1]->getVal();
        string varName = ID_1 + "." + ID_2;
        varName = toVarName(varName);

        value.varName = varName;
        return value;
    }else if(!node->getType().compare("ArrayExp")){
        string ID = node->children[0]->getVal();
        C_Value exp = genIrExp(func, node->children[1]);
        string tempVar = getTempVar();
        Quad quad(tempVar, exp.varName, "&"+ID, "+");
        this->insertQuad(quad);        
        value.varName = "*"+tempVar;
    }
    return value;
}

void irCodeGenerator::genIrArgs(funNode* func, Node *node){
    if(DEBUG)
        cout << ">> genIrArgs" << endl;

    C_Value exp = genIrExp(func, node->children[0]);
    Quad quad("", "ARG", exp.varName, "");
    this->insertQuad(quad);
    if(node->children.size()>1){
        genIrArgs(func, node->children[1]);
    }
}

void irCodeGenerator::genIrReturn(funNode* func, Node *node){
    if(DEBUG)
        cout << ">> genIrReturn" << endl;

    C_Value exp = genIrExp(func, node);
    Quad quad("", "RETURN", exp.varName, "");
    this->insertQuad(quad);
    this->releaseTemp();
}

void irCodeGenerator::genIrIf(funNode* func, Node *node){
    if(DEBUG)
        cout << ">> genIrIf" << endl;

    string label = getLabel();
    C_Value exp = genIrExp(func, node->children[0]);
    Quad quad("IF_NOT", exp.varName, "GOTO", label);
    this->insertQuad(quad);

    genIrState(func, node->children[1]);

    Quad quad2("", "Label", label, "");
    this->insertQuad(quad);
}

void irCodeGenerator::genIrIfElse(funNode* func, Node *node){
    if(DEBUG)
        cout << ">> genIrIfElse" << endl;

    string label1 = getLabel();
    string label2 = getLabel();

    C_Value exp = genIrExp(func, node->children[0]);
    Quad quad1("IF_NOT", exp.varName, "GOTO", label1);
    this->insertQuad(quad1);

    genIrState(func, node->children[1]);
    Quad quad2("", "", "GOTO", label2);
    this->insertQuad(quad2);
    Quad quad3("", "Label", label1, "");
    this->insertQuad(quad3);

    genIrState(func, node->children[2]);

    Quad quad4("", "Label", label2, "");
    this->insertQuad(quad4);
}

void irCodeGenerator::genIrWhile(funNode* func, Node *node){
    if(DEBUG)
        cout << ">> genIrWhile" << endl;

    string label1 = getLabel();
    string label2 = getLabel();

    Quad quad1("", "Label", label1, "");
    this->insertQuad(quad1);
    C_Value exp = genIrExp(func, node->children[0]); 
    Quad quad2("IF_NOT", exp.varName, "GOTO", label2);
    this->insertQuad(quad2); 

    genIrState(func, node->children[1]);
    Quad quad3("", "", "GOTO", label1);
    this->insertQuad(quad3);
    Quad quad4("", "Label", label2, "");
    this->insertQuad(quad4); 
}

void irCodeGenerator::genIrFor(funNode* func, Node *node){
    if(DEBUG)
        cout << ">> genIrWhile" << endl;

    string label1 = getLabel();
    string label2 = getLabel();

    C_Value exp = genIrExp(func, node->children[0]); 
    Quad quad1("", "Label", label1, "");
    this->insertQuad(quad1);
    C_Value exp2 = genIrExp(func, node->children[1]); 
    Quad quad2("IF_NOT", exp2.varName, "GOTO", label2);
    this->insertQuad(quad2); 

    genIrState(func, node->children[3]);
    C_Value exp3 = genIrExp(func, node->children[2]); 
    Quad quad3("", "", "GOTO", label1);
    this->insertQuad(quad3);
    Quad quad4("", "Label", label2, "");
    this->insertQuad(quad4); 
}

void irCodeGenerator::genIrDoWhile(funNode* func, Node *node){
    if(DEBUG)
        cout << ">> genIrWhile" << endl;

    string label1 = getLabel();
    string label2 = getLabel();

    Quad quad1("", "Label", label1, "");
    this->insertQuad(quad1);

    genIrState(func, node->children[0]);

    C_Value exp = genIrExp(func, node->children[1]); 
    Quad quad2("IF_NOT", exp.varName, "GOTO", label2);
    this->insertQuad(quad2); 

    Quad quad3("", "", "GOTO", label1);
    this->insertQuad(quad3);
    Quad quad4("", "Label", label2, "");
    this->insertQuad(quad4); 
}
	
varNode* irCodeGenerator::genIrArray(Type type, Node *node){
    string ID = node->children[0]->getVal();
    string Num = node->children[1]->getVal();
    int eleNum = atoi(Num.c_str());
    Quad quad("", "Array", ID, Num);
    this->insertQuad(quad); 
    return new varNode(ID, type);
}

Type irCodeGenerator::getType(Node *node){
    if(DEBUG)
        cout << ">> getType" << endl;

    Type type;
    if(!node->getVal().compare("INT")){
        type = INT_TYPE;
    }else if(!node->getVal().compare("FLOAT")){
        type = FLOAT_TYPE;
    }else{
        type = VOID_TYPE;
    }
    return type;
}

void irCodeGenerator::insertQuad(Quad quad){
        this->lineNum++;
        this->IrCodes.push_back(quad);
        quad.printQuad();
    }

string irCodeGenerator::getTempVar(){
    string varName = "t";
    varName += to_string(tempVarNum);
    tempVarNum++;
    return varName;
}

void irCodeGenerator::releaseTemp(){
    tempVarNum = 0;
}

string irCodeGenerator::getLabel(){
    string varName = "label";
    varName += to_string(labelNum);
    labelNum++;
    return varName;
}

int irCodeGenerator::findVar(funNode* func, string key){
    if (func->paralist.count(key) != 0){
        return 2;
    }

    if (func->localVar.count(key) != 0){
        return 1;
    }

    if (this->varNodes.count(key) != 0){
        return 0;
    }

    return -1;
}

string irCodeGenerator::toVarName (string ID){
    return IDVarMap[ID];
}

string irCodeGenerator::addID(string ID){
    string VarName = "var" + to_string(VarNum++);
    IDVarMap.insert(pair<string, string>(ID, VarName));
    return VarName;
}

void irCodeGenerator::writeToFile(string filePath){
    ofstream outfile;
    outfile.open(filePath);
    list<Quad>::iterator it; //声明一个迭代器
    for(it = IrCodes.begin();it!=IrCodes.end();it++){
        string code = it->printQuad();
        outfile << code;
    }
    outfile.close();
}
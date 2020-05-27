#include "parserTreeNode.h"

// Element::Element(string eleName, string eleVal){
//     this->eleName = eleName;
//     this->eleVal = eleVal;
// }

// Element::Element(Element *ele){
//     this->eleName = ele->eleName;
//     this->eleVal = ele->eleVal;
// }

Node::Node(string type, int eleNum,...){
    this->type = type;
    // this->val = NULL;
    cout << "type: " << type << endl;
    this->val = "";
    va_list ap;
    va_start(ap, eleNum);
    for(int i=0;i<eleNum;i++){
        Node *node = va_arg(ap, Node*);
        if(!node->type.compare("empty"))
            continue;
        
        this->children.push_back(node);
    }
}

Node::Node(string type, string val){
    // this->val = new Element(type, val); 
    cout << "type: "<< type << "val: " << val << endl; 
    this->type = type;
    this->val = val;
}

string Node::getType(){
    return this->type;
}

string Node::getVal(){
    return this->val;
}
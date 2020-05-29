#include "saveTree.h"

saveTree::saveTree(Node* node){
    this->root = node;
}

void saveTree::addJson(string line){
    cout << line << endl;
    this->jsonLine.push_back(line);
}

void saveTree::genJson(int level, Node *node, bool hasSib){
    string jsonTab;
    for(int i=0; i<level; i++)
        jsonTab += "\t";
    string id = node->getType();
    addJson(jsonTab+"{");
    
    if(node->children.size()!=0){
        addJson(jsonTab+"\t\"id\": \"" + id +"\"" + ",");
        addJson(jsonTab+"\t\"children\": [");
    }else
        addJson(jsonTab+"\t\"id\": \"" + id +"\"");

    for(int j=0; j<node->children.size(); j++){
        if(j==(node->children.size()-1)){
            genJson(level+1, node->children[j], false);
        }else{
            genJson(level+1, node->children[j], true);
        }
    }
    if(node->children.size()!=0)
        addJson(jsonTab+"\t]");

    if(hasSib){
        addJson(jsonTab+"},");
    }else{
        addJson(jsonTab+"}");
    }
    
    
}

void saveTree::writeToFile(string filePath){
    ofstream outfile;
    outfile.open(filePath.c_str());
    list<string>::iterator it; //声明一个迭代器
    for(it = jsonLine.begin();it!=jsonLine.end();it++){
        outfile << *it << endl;
    }
    outfile.close();
}


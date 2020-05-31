#include"compiler.hpp"
#include"irCodeGen.h"
#include "tclap/CmdLine.h"
#include "saveTree.h"

extern FILE *yyin;
extern Node *root;

string filename = "";
string funcname = "";
string line = "";

void parseFilename(string inputFile){//获取文件名
    const char *tmp = strrchr(inputFile.c_str(),'/');
    filename = string(tmp).substr(1);
}

int yyparse();

using namespace std;

int main(int argc, char **argv)
{
  string inputFile;
  string outputFile;
  string jsonFile;

  try {
    TCLAP::CmdLine cmd("compiler", ' ', "1.0.0");
    TCLAP::ValueArg<string> src_arg("s", "src",
          "Path to a c file",
          false, "../test.c", "string");

    TCLAP::ValueArg<string> dst_arg(
        "d", "dst", "output file to the dst file", false,
        "../result/mid.txt", "string");
      
    TCLAP::ValueArg<string> json_arg(
        "j", "json", "path of the json file", false,
        "../result/mid.json", "string");

    cmd.add(src_arg);
    cmd.add(dst_arg);
    cmd.add(json_arg);

    // Parse the argv array.
    cmd.parse(argc, argv);

    // Get the value parsed by each arg.
    inputFile = src_arg.getValue();
    outputFile = dst_arg.getValue();
    jsonFile = json_arg.getValue();
    parseFilename(inputFile);
  } catch (TCLAP::ArgException& e)  // Catch any exceptions
  {\
    std::cerr << "Error: " << e.error() << " for arg " << e.argId()
              << std::endl;
  }

  yyin = fopen(inputFile.c_str(), "r");
  yyparse();
  // cout << "parsing" << endl;
  // cout<<endl<<endl;  

  saveTree s(root);
  s.genJson(0, root, false);
  s.writeToFile(jsonFile.c_str());
  
  irCodeGenerator irG;
  irG.genIrParserTree(root);

  irG.writeToFile(outputFile.c_str());

  cout << "parsing done" << endl;
  cout << "ir code file is saved to path " << outputFile << endl;
  cout << "json file is saved to path " << jsonFile << endl;
  return 0;
}
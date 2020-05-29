#include"compiler.hpp"
#include"irCodeGen.h"
#include "tclap/CmdLine.h"
#include "saveTree.h"

extern FILE *yyin;
extern Node *root;

int yyparse();

using namespace std;

int main(int argc, char **argv)
{
  string inputFile;
  string outputFile;

  try {
    TCLAP::CmdLine cmd("compiler", ' ', "1.0.0");
    TCLAP::ValueArg<string> src_arg("s", "src",
                                         "Path to a c file",
                                         false, "", "string");
    TCLAP::ValueArg<string> dst_arg(
        "d", "dst", "output file to the dst file", false,
        "", "string");
    cmd.add(src_arg);
    cmd.add(dst_arg);

    // Parse the argv array.
    cmd.parse(argc, argv);

    // Get the value parsed by each arg.
    inputFile = src_arg.getValue();
    outputFile = dst_arg.getValue();
  } catch (TCLAP::ArgException& e)  // Catch any exceptions
  {
    std::cerr << "Error: " << e.error() << " for arg " << e.argId()
              << std::endl;
  }

  yyin = fopen(inputFile.c_str(), "r");
  yyparse();
  cout<<endl<<endl;  

  saveTree s(root);
  s.genJson(0, root, false);
  s.writeToFile("../test.json");
  
  irCodeGenerator irG;
  irG.genIrParserTree(root);

  irG.writeToFile(outputFile.c_str());
  return 0;
}
#include "errorHandler.h"

Exception::Exception(string _filename,string _function, int _line){
    error_filename = _filename;
    error_function = _function;
    error_line = _line;
}
void Exception::display(){
    fprintf(stderr,"%s: In function '%s':\n",error_filename.c_str(), error_function.c_str());//错误行号
	fprintf(stderr,"%s: syntax error at line %d\n",error_filename.c_str(),error_line);
    char cmd[100] = {0};
    sprintf(cmd,"sed -n '%d,%dp' ../test/%s",error_line,error_line,error_filename.c_str());
    system(cmd);//调用系统命令打印错误行 windows 不支持
    cout<<setfill(' ')<<setw(12)<<"^~"<<endl;
}
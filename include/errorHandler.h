#ifndef _ERRORHANDLER_H_
#define _ERRORHANDLER_H_
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;


class Exception
{
public:
	Exception(string _filename,string _function,int _line);
	void display();
private:
    string error_filename;
    string error_function;
    int error_line;
};
#endif
#ifndef LOG_HPP
#define LOG_HPP

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <ios>
#include <algorithm>

namespace gm
{

class Log
{
public:
	static mLevel;
	static streambuf* clogbuf = clog.rdbuf(); //save old buf;
	static ofstream o_f;
	
	static void setLevel(int level){
		mLevel = level;
	}
	
};



}

#endif // LOG_HPP

#pragma once

#include <fstream>
#include <string.h>

class Log
{
	std::ofstream _os;
	std::string filename;
public:
	Log();
	Log(std::string const);
	bool is_open();
	void open(std::string const);
	void write(std::string const);
	void close();
	static int dump(std::ostream&,std::string const,std::string const);
	~Log();
};

#include "Log.cpp"
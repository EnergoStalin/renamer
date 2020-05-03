#pragma once

#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <ctime>
#include <dirent.h>
#include <algorithm>
#include <chrono>
#include "Filesystem.h"

#define RENAMER_STRING_BUFFER_SIZE 256

class renamer {
	std::vector<std::string> ignore; //ignored files
	std::vector<std::string> ext; //file extension to be processed
	std::string gDir; //store good files
	std::string bDir; //store bad files
	std::string cDir; //processing directory
	//total
	uint64_t total_good = 0;
	uint64_t total_bad = 0;
	enum RUNTIME_ERROR_CODE
	{
		CANT_OPEN_DIR = 104,
		CANT_STAT_FILE
	};
	enum PARSE_ARGUMENTS_RETURN_CODE
	{
		ERROR_NOT_ENOUGH_ARGS = 1,
		ERROR_BAD_DIR_NOT_SPECIFIED,
		ERROR_GOOD_DIR_NOT_SPECIFIED,
		ERROR_NO_IGNORED_FILES_SPECIFIED,
		ERROR_CANT_PARSE_FILE,
		ERROR_NO_FILES_EXTENSION_SPECIFIED,
		ERROR_EMPTY_FILE,
		ERROR_ARGUMENT_ORDER,
		CANT_CREATE_DIR,
		CANT_OPEN_LOG,
		PARSE_SUCCESS,
		STOP
	};
	PARSE_ARGUMENTS_RETURN_CODE parseArguments(int,const char**);
	std::string processFilename(const char*,size_t);
	bool check_param();
	bool validate_name(std::string&);
	void fileToVector(const char*,std::vector<std::string>&,char);
	void stringToVector(std::string s,std::vector<std::string> &v,char delim);
	int processDir();
	void Usage();
public:
	int run(int&,char const**);
	renamer();
	~renamer();
	
};

#include "../src/renamer.cpp"
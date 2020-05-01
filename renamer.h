#pragma once

#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <ctime>
#include "Filesystem.h"

#define RENAMER_STRING_BUFFER_SIZE 256

class renamer {
	//Flags
		bool writeLog = false;
		bool hasDefis = false;
	//
	std::vector<std::string> ignore; //ignored files
	std::vector<std::string> ext; //file extension to be processed
	std::string gDir; //store good files
	std::string bDir; //store bad files
	std::string cDir; //processing directory
	std::string logFilename; //log file name
	enum PARSE_ARGUMENTS_RETURN_CODE
	{
		ERROR_NOT_ENOUGH_ARGS = 0,
		ERROR_BAD_DIR_NOT_SPECIFIED,
		ERROR_GOOD_DIR_NOT_SPECIFIED,
		ERROR_NO_IGNORED_FILES_SPECIFIED,
		ERROR_CANT_PARSE_FILE,
		ERROR_NO_FILES_EXTENSION_SPECIFIED,
		PARSE_SUCCESS,
		UNRECOGNIZED_ERROR
	};
	PARSE_ARGUMENTS_RETURN_CODE parseArguments(int,const char**);
	void fileToVector(const char*,std::vector<std::string>&,char);
	void stringToVector(std::string s,std::vector<std::string> &v,char delim);

	void Usage();
public:
	renamer(int,char const**);
	~renamer();
	
};

#include "renamer.cpp"
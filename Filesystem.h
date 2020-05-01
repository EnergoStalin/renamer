#pragma once

#include <exception>
#include <string.h>
#include "Log.h"

class Filesystem {
	Filesystem();
public:
	class FilesystemException : public std::exception
	{
		std::string msg;
		int code;
	public:
		FilesystemException(std::string const,int);
		std::string what();
		int getCode();
	};
	static void _mkdir(const char*);
	static void _rename(const char*, const char*);
	static char slash;
	static Log log;
};

#include "Filesystem.cpp"
#include "Filesystem.h"

Filesystem::Filesystem() {}


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#include <windows.h>

	char Filesystem::slash = '\\';

	void Filesystem::_mkdir(const char *name)
	{
		CreateDirectory(name,NULL);
	}
//if linux will use this
#elif defined(__linux__) || defined(__unix__)
	#include <sys/stat.h>
	#include <sys/types.h>

	char Filesystem::slash = '/';

	void Filesystem::_mkdir(const char *name)
	{
		mkdir(name,0);
	}

#endif

//both for unix and windows
Log Filesystem::log;

void Filesystem::_rename(const char *oldname,const char *newname)
{
	rename(oldname,newname);
}

//Exception
Filesystem::FilesystemException::FilesystemException(std::string const msg,int code) 
{
	this->msg = msg;
	this->code = code;
}

std::string Filesystem::FilesystemException::what()
{
	char code[20];
	snprintf(code, 20, "%d", this->code);
	return this->msg + " code - " + std::to_string(this->code);
}

int Filesystem::FilesystemException::getCode()
{
	return code;
}
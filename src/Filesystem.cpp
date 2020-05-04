#include "Filesystem.h"

Filesystem::Filesystem() {}


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#include <windows.h>

	char Filesystem::slash = '\\';

	void Filesystem::_mkdir(const char *name)
	{
		switch(CreateDirectory(name,NULL))
		{
			case ERROR_ALREADY_EXISTS:
				throw FilesystemException(std::string("Directory not created '") + name + "'.\n[Warning]: Directory allready exist.",ERROR_ALREADY_EXISTS);
			break;
			case ERROR_PATH_NOT_FOUND:
				throw FilesystemException(std::string("Unable create directory '") + name + "'.\n[Error]: Path doesent exist.",ERROR_PATH_NOT_FOUND);
			break;
			default:
			break;
		}
	}
//if linux will use this
#elif defined(__linux__) || defined(__unix__)
	#include <sys/stat.h>
	#include <sys/types.h>

	char Filesystem::slash = '/';

	void Filesystem::_mkdir(const char *name)
	{
		switch(mkdir(name,0))
		{
			case ENOTDIR:
				throw FilesystemException(std::string("Unable create directory '") + name + "'.\n[Error]: Path component is not directory.",ENOTDIR);
			break;
			case ENAMETOOLONG:
				throw FilesystemException(std::string("Unable create direcory '") + name + "'.\n[Error] Too long name.",ENAMETOOLONG);
			break;
			case EROFS:
				throw FilesystemException(std::string("Unable create directory '") + name + "'.\n[Error]: Directory read only.",EROFS);
			break;
			case EEXIST:
				throw FilesystemException(std::string("Directory not created '") + name + "'.\n[Warning]: Directory allready exist.",EEXIST);
			break;
			case ENOENT:
				throw FilesystemException(std::string("Unable create directory '") + name + "'.\n[Error]: Path doesent exist.",ENOENT);
			break;
			case EACCES:
				throw FilesystemException(std::string("Unable create directory '") + name + "'.\n[Error]: No acess to this path.",EACCES);
			break;
		}

	}

#endif

//both for unix and windows
Log Filesystem::log;

void Filesystem::_rename(const char *oldname,const char *newname)
{
	switch(rename(oldname,newname))
	{
		case EACCES:
			throw FilesystemException(std::string("Unable rename file '") + oldname + "' to '" + newname + "'.\n[Error]: No acess to this path.",EACCES);
		break;
		case ENOENT:
			throw FilesystemException(std::string("Unable rename file '") + oldname + "' to '" + newname + "'.\n[Error]: Path doesent exist.",ENOENT);
		break;
		#if defined(__linux__) || defined(__unix__)
		case ENOTNAM:
			throw FilesystemException(std::string("Unable rename file '") + oldname + "' to '" + newname + "'.\n[Error]: Not in the same disk.",ENOTNAM);
		break;
		#endif
	}
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
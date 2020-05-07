#include "Filesystem.h"

Filesystem::Filesystem() {}


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#include <windows.h>

	char Filesystem::slash = '\\';

	void Filesystem::_mkdir(const char *name)
	{
		if(CreateDirectory(name,NULL) == 0)
			switch(GetLastError())
			{
				case ERROR_ALREADY_EXISTS:
					throw FilesystemException(std::string("Directory not created '") + name + "'.\n[Warning]: Directory allready exist.",ERROR_ALREADY_EXISTS);
				break;
				case ERROR_PATH_NOT_FOUND:
					throw FilesystemException(std::string("Unable create directory '") + name + "'.\n[Error]: Path doesent exist.",-1);
				break;
			}
	}
	void Filesystem::_rename(const char *oldname,const char *newname)
	{
		rename(oldname,newname);
		switch(GetLastError())
		{
			case EACCES:
				throw FilesystemException(std::string("Unable rename file '") + oldname + "' to '" + newname + "'.\n[Error]: No acess to this path.",-1);
			break;
			case ENOENT:
				throw FilesystemException(std::string("Unable rename file '") + oldname + "' to '" + newname + "'.\n[Error]: Path doesent exist.",-1);
			break;
			case EEXIST:
				throw FilesystemException(std::string("Unable rename file '") + oldname + "' to '" + newname + "'.\n[Error]: File exist.",-1);
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
		mkdir(name,0);
		switch(errno)
		{
			case ENOTDIR:
				throw FilesystemException(std::string("Unable create directory '") + name + "'.\n[Error]: Path component is not directory.",-1);
			break;
			case ENAMETOOLONG:
				throw FilesystemException(std::string("Unable create direcory '") + name + "'.\n[Error] Too long name.",-1);
			break;
			case EROFS:
				throw FilesystemException(std::string("Unable create directory '") + name + "'.\n[Error]: Directory read only.",-1);
			break;
			case EEXIST:
				throw FilesystemException(std::string("Directory not created '") + name + "'.\n[Warning]: Directory allready exist.",EEXIST);
			break;
			case ENOENT:
				throw FilesystemException(std::string("Unable create directory '") + name + "'.\n[Error]: Path doesent exist.",-1);
			break;
			case EACCES:
				throw FilesystemException(std::string("Unable create directory '") + name + "'.\n[Error]: No acess to this path.",-1);
			break;
		}

	}
	void Filesystem::_rename(const char *oldname,const char *newname)
	{
		rename(oldname,newname);
		switch(errno)
		{
			case EACCES:
				throw FilesystemException(std::string("Unable rename file '") + oldname + "' to '" + newname + "'.\n[Error]: No acess to this path.",-1);
			break;
			case EEXIST:
				throw FilesystemException(std::string("File with same name exist '") + oldname + "'\t'" + newname + "'.\n[Warning]: File exist.", 0);
			break;
			case ENOTNAM:
				throw FilesystemException(std::string("Unable rename file '") + oldname + "' to '" + newname + "'.\n[Error]: Not in the same disk.",-1);
			break;
			case ETXTBSY: case EBUSY: throw FilesystemException(std::string("One of this files busy '") + oldname + "'\t'" + "'\n[Warning]: Files busy.",-1);
		}
	}


#endif

//both for unix and windows
Log Filesystem::log;


//Exception
Filesystem::FilesystemException::FilesystemException(std::string const msg,int code) 
{
	this->msg = msg;
	this->code = code;
}

std::string Filesystem::FilesystemException::what()
{
	return this->msg + " code - " + std::to_string(this->code) + "\n";
}

int Filesystem::FilesystemException::getCode()
{
	return code;
}
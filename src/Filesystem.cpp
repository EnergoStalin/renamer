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
					throw FilesystemException(std::string("Directory not created '") + name + "'.\n[Warning]: Directory allready exist.",0);
				break;
				case ERROR_PATH_NOT_FOUND:
					throw FilesystemException(std::string("Unable create directory '") + name + "'. Reason: Path dont exist.",-1);
				break;
			}
	}
	void Filesystem::_rename(const char *oldname,const char *newname)
	{
		if(rename(oldname,newname) != 0)
			switch(errno)
			{
				case EACCES:
					throw FilesystemException(std::string("Unable rename file '") + oldname + "' to '" + newname + "'. Reason: No acess to this path.",-1);
				break;
				case ENOENT:
					throw FilesystemException(std::string("Unable rename file '") + oldname + "' to '" + newname + "'. Reason: Path dont exist.",-1);
				break;
				case EEXIST:
					throw FilesystemException(std::string("Unable rename file '") + oldname + "' to '" + newname + "'. Reason: File exist.",0);
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
		if(mkdir(name,0) == -1)
			switch(errno)
			{
				case ENOTDIR:
					throw FilesystemException(std::string("Unable create directory '") + name + "'. Reason: Path component is not directory.",-1);
				break;
				case ENAMETOOLONG:
					throw FilesystemException(std::string("Unable create direcory '") + name + "'. Reason: Too long name.",-1);
				break;
				case EROFS:
					throw FilesystemException(std::string("Unable create directory '") + name + "'. Reason: Directory read only.",-1);
				break;
				case EEXIST:
					throw FilesystemException(std::string("Directory not created '") + name + "'. [Warning]: Directory allready exist.",0);
				break;
				case ENOENT:
					throw FilesystemException(std::string("Unable create directory '") + name + "'. Reason: Path dont exist.",-1);
				break;
				case EACCES:
					throw FilesystemException(std::string("Unable create directory '") + name + "'. Reason: No acess to this path.",-1);
				break;
			}
	}
	void Filesystem::_rename(const char *oldname,const char *newname)
	{
		if(rename(oldname,newname) != 0)
			switch(errno)
			{
				case EACCES:
					throw FilesystemException(std::string("Unable rename file '") + oldname + "' to '" + newname + "'. Reason: No acess to this path.",-1);
				break;
				case EEXIST:
					throw FilesystemException(std::string("File with same name exist '") + oldname + "'\t'" + newname + "'. [Warning]: File exist.", 0);
				break;
				case ENOTNAM:
					throw FilesystemException(std::string("Unable rename file '") + oldname + "' to '" + newname + "'. Reason: Not in the same disk.",-1);
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
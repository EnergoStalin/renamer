#include "Log.h"

Log::Log()
{
	this->filename.reserve(50);
}

Log::Log(std::string const file) : Log()
{
	this->open(file);
}

void Log::open(std::string const file)
{
	this->close();
		this->_os.open(file,std::ios::out | std::ios::trunc);
		if(!this->_os.is_open())
		{
			this->filename.clear();
			this->filename += file;
		}
}
void Log::write(std::string const msg)
{
	if(this->_os.is_open())
		this->_os << msg;
}

void Log::close()
{
	if(this->_os.is_open())
		this->_os.close();
}

bool Log::is_open()
{
	return this->_os.is_open();
}

int Log::dump(std::ostream &stream,std::string const filename,std::string const expr)
{
		std::ifstream is(filename,std::ios::in);

		if(!is.is_open()) return 1;

		std::string buff;
		while(getline(is,buff,'\n'))
		{
			if(buff.find(expr) != std::string::npos) stream << buff << '\n';
		}

		is.close();

		return 0;
}

Log::~Log()
{
	this->close();
}
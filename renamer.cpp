//Refactoring code to OOP style
//Another implementatiof of renamer

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <dirent.h>

#define RENAMER_RUNTIME_LOG_NAME "renamer_runtime_latest_log.txt"

enum EXIT_STATUS
{
	EXIT_STATUS_SUCCESS = 0x0000000,
	ERROR_CANT_ACESS_DIR,
	ERROR_CANT_OPEN_CONFIG_FILE,
};

class Log
{
	std::ofstream _os;
	std::string filename;
public:
	Log() {}
	Log(std::string file)
	{
		this->open(file);
	}
	bool is_open()
	{
		return this->_os.is_open();
	}
	void open(std::string file)
	{
		this->close();
		this->_os.open(file,std::ios::out);
		if(!this->_os.is_open())
			this->filename = file;
	}
	void write(std::string msg)
	{
		if(this->_os.is_open())
			this->_os << msg;
	}
	void close()
	{
		if(this->_os.is_open())
			this->_os.close();
	}
	static int dump(std::string,std::string);
	~Log()
	{
		this->close();
	}
};

int Log::dump(std::string filename,std::string expr)
{
		std::ifstream is(filename,std::ios::in);

		if(!is.is_open()) return 1;

		std::string buff;
		while(getline(is,buff,'\n'))
		{
			if(buff.find(expr) != std::string::npos) std::cout << buff << '\n';
		}

		is.close();

		return 0;
}

//Universal filesystem class
class Filesystem
{
public:
	static int _rename(std::string const&,std::string const&);
	static int _mkdir(std::string const&);
	static int _remove(std::string);
	static std::string statfile(std::string const&);
	static Log log;
};

//if compiling for windows will use this implimentation
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#include <windows.h>

	char slash = '\\';

	int Filesystem::_mkdir(std::string &name)
	{
		return CreateDirectory(name.c_str(),NULL);
	}
	std::string Filesystem::statfile(std::string const &file)
	{

		return "";
	}
	int Filesystem::_remove(std::string name)
	{
		return remove(name.c_str());
	}
//if linux will use this
#elif defined(__linux__) || defined(__unix__)
	#include <sys/stat.h>
	#include <sys/types.h>

	char slash = '/';

	int Filesystem::_remove(std::string name)
	{
		return remove(name.c_str());
	}
	int Filesystem::_mkdir(std::string const &name)
	{
		return mkdir(name.c_str(),0);
	}
	std::string Filesystem::statfile(std::string const &file)
	{

		return "";
	}

#endif

//both for unix and windows
Log Filesystem::log;

int Filesystem::_rename(std::string const &oldname,std::string const &newname)
{
	return rename(oldname.c_str(),newname.c_str());
}

//vars
std::string processdir = ""; //processing directory
std::string errdir = ""; //error directory
std::string gooddir = ""; //for good dir
std::vector<std::string> procfiles; //file extensions who be processed
std::vector<std::string> ignorfiles; //ignored files such as log file if specified ".txt" extension
int defis;
bool writelog = false;

//match fileextenson check
bool in_vector(std::vector<std::string> const &v,std::string const &fe)
{
	for(int i = 0; i < v.size(); i++)
	{
		if(v[i] == fe) return true;
	}
	return false;
}

//Usage function
void Usage()
{
	std::cout << "Usage:\n";
	std::cout << "\tOptions:\n\t\t-gd\t-\tGood Dir\tDir to store right files.\n";
	std::cout << "\t\t-bd\t-\tBad Dir\t\tDir to store dont right files.\n";
	std::cout << "\t\t-h || ?\t-\tHelp page\tShows this page and stop programm.\n";
	std::cout << "\t\t-wl\t-\tWrite Log\tWrite runtime log to file(file cant be specified) by default \"renamer_runtime_log.txt\".\n";
	std::cout << "\t\t-ext\t-\tExtensions\tFile extensions to be processed.\n";
	std::cout << "\t\t\tYou can read it from file or specify manually by syntax \".txt .pdf .obj .ttf\".\n";
	std::cout << "\t\t-i\t-\tIgnore\tFiles to be ignored only read from file supported.\n";
	std::cout << "\t\t\tWrite fileneames separately by new line. Specified file will also be ignored.\n\t\t\tTo make remove ignore file automatically specify -r flag after file name\n";
	std::cout << "\t\t-dl\t-\tDump log\tDump log and exit.\n";
	std::cout << "\t\t\tTakes string. Display all strings where mathes been found. Such as very simplyfied linux grep command.\n\t\t\t(For windows users who cant use \"cat $file | grep $expr\")\n";
	std::cout << "Example:\n\trenamer ./ -bd fail -gd success -i ignore.txt -wl -ext .pdf .ttf\n";
}

//Process arguments
int ProcessArgs(int argc, char const *argv[])
{
	if(argc == 1) { Usage(); return 0; }
	std::string arg;
	for(int i = 1; i < argc; i++)
	{
		arg = argv[i];
		if(arg[0] == '?') { Usage(); break; }
		else if(arg[0] == '-')
		{
			arg = arg.substr(1);
			if(arg == "bd")
			{
				if(i+1 < argc)
				{
					errdir = argv[++i];
					errdir += slash;
				}
				else
					return -1;
			} else if(arg == "gd")
			{
				if(i+1 < argc)
				{
					gooddir = argv[++i];
					gooddir += slash;
				}
				else
					return -1;
			} else if(arg == "i")
			{
				std::ifstream is(argv[++i],std::ios::in);

				if(!is.is_open())
				{
					std::cout << "Cant read ignored file list.\n";
					return -1;
				}

				ignorfiles.push_back(argv[i]);

				std::string buff;
				while(getline(is,buff,'\n'))
				{
					ignorfiles.push_back(buff);
				}

				if((i+1) < argc && std::string(argv[i+1]) == "-r")
					Filesystem::_remove(argv[i]);
			}
			else if(arg == "dl")
			{
				Log::dump(RENAMER_RUNTIME_LOG_NAME,argv[++i]);
			}
			else if(arg == "wl")
			{
				writelog = true;
			} else if(arg == "ext")
			{
				if(argv[++i][0] == '.')
				{
					for(; i < argc; i++)
					{
						if(argv[i][0] != '.') break;
						procfiles.push_back(argv[i]);
					}
				}
				else
				{
					std::ifstream is(argv[i],std::ios::in);

					if(!is.is_open())
					{
						std::cout << "Cant read file extension list.\n";
						return -1;
					}
					std::string buff;
					while(is >> buff)
					{
						procfiles.push_back(buff);
					}
				}
			}
			switch(arg[0])
			{
				case 'h':
					Usage();
					return 0;
				break;
				default:
				break;
			}
		}
		else
		{
			processdir = arg;
			processdir += slash;
		}
	}


	return 0;
}

int main(int argc, char const *argv[])
{
	if(ProcessArgs(argc,argv) != 0 || processdir.empty() || errdir.empty() || gooddir.empty()) return 1;

	if(writelog) Filesystem::log.open(RENAMER_RUNTIME_LOG_NAME);
	if(writelog) ignorfiles.push_back(RENAMER_RUNTIME_LOG_NAME);

	DIR *dir = opendir(processdir.c_str());
	if(!dir)
	{
		std::cerr << "Cant open dir " << processdir << '\n';
		return ERROR_CANT_ACESS_DIR;
	}

	//Warning!!!
	Filesystem::_mkdir(errdir);
	Filesystem::_mkdir(gooddir);
	//end Warning!!!

	int goodcount,badcount;
	goodcount = badcount = 0;

	dirent *file;
	while((file = readdir(dir)) != NULL)
	{
		std::string oldname,newname,newfullpath,oldfullpath,fileextension;
		
		oldname = file->d_name;
		if(oldname[0] == '.') continue;
		size_t dotindex = oldname.find_last_of('.');
		if(dotindex != std::string::npos)
		{

			if(!in_vector(procfiles,oldname.substr(dotindex)) || in_vector(ignorfiles,oldname)) continue;

			for(int i = 0; i < (oldname.size()-(oldname.size()-dotindex)); i++)
			{
				if(isdigit(oldname[i])) newname+=oldname[i];
				else
				{
					switch(oldname[i])
					{
						case 'b': newname+='6'; break;
						case 'q': case 'g': newname+='9'; break;
						case '+': newname+='4'; break;
						case '!': case 'L': case 'l': case 'I': case 'i': case 'J': case 'j':
						case ']': case '[': case 't': case 'r': case '|' : newname+='1'; break;
						case 'S': case 's': newname+='5'; break;
						case 'O': case 'o': newname+='0'; break;
						case 'B': newname+='8'; break;
						case '-': newname+='-'; defis=1; break;
					}
				}
				
			}
			newname += oldname.substr(dotindex);
			if(((defis==0) && ((newname.size()==9)||(newname.size()==10)))	||	((defis==1) && ((newname.size()>=11 && newname.size() <= 13))))
			{
				newname = gooddir+newname;
				std::cout << "newfilename:" << newname << '\n';
				oldfullpath = processdir+oldname;
				newfullpath = processdir+newname;
				std::cout << "oldfullpath:" <<  oldfullpath << '\n';
				std::cout << "newfullpath:" <<  newfullpath << "\n\n";

				if(writelog)
				{
					Filesystem::log.write("newfilename:" + newname + "\n");
					Filesystem::log.write("oldfullpath:" + oldfullpath + "\n");
					Filesystem::log.write("newfullpath:" + newfullpath + "\n\n");
				}

				if(Filesystem::_rename(oldfullpath, newfullpath))
				{
					std::cout << "movecmp:" << oldfullpath << '\n';
					if(writelog)
						Filesystem::log.write("movecmp:" + oldfullpath + "\n");
					oldfullpath = processdir+errdir+newname;
					std::cout << "errdircmp:" << oldfullpath << '\n';
					if(writelog)
						Filesystem::log.write("errdircmp:" + oldfullpath + "\n\n");
					Filesystem::_rename(oldfullpath, newfullpath);
					badcount++;
				}
				else
				{
					goodcount++;
					if(writelog)
					{
						Filesystem::log.write("\n");
					}
				}
			}
			else
			{
				badcount++;
				newfullpath = errdir+newname; // Sorry for this
				oldfullpath = processdir+oldname; // And this also
				std::cout << "move:" << oldfullpath << '\n';
				std::cout << "errdir:" << newfullpath << "\n\n";
				Filesystem::_rename(oldfullpath,newfullpath);
				if(writelog)
				{
					Filesystem::log.write("move:" + oldfullpath + "\n");
					Filesystem::log.write("errdir:" + oldfullpath + "\n\n");
				}
			}
		} else continue;
	}

	std::cout << "Total good files " << goodcount << "\nTotal bad files " << badcount << '\n';
	if(writelog)
	{
		std::cout << "Dump log grab information by expr.\n";
		Filesystem::log.write("Total good files " + std::to_string(goodcount) + "\nTotal bad files " + std::to_string(badcount) + '\n');
	}



	closedir(dir);

	return EXIT_STATUS_SUCCESS;
}

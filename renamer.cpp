#include "renamer.h"

renamer::renamer(int argc,char const *argv[])
{
	this->parseArguments(argc,argv);
}

void renamer::fileToVector(char const *file,std::vector<std::string> &v,char delim)
{
	v.push_back(file);
	std::ifstream is(file,std::ios::in);

	if(!is.is_open()) throw 1;

	std::string buff;
	buff.reserve(256);
	while(getline(is,buff,delim))
	{
		v.push_back(buff);
	}
}

void renamer::Usage()
{

}

void renamer::stringToVector(std::string s,std::vector<std::string> &v,char delim)
{
	int beg = 0,cur = 0,end = s.length();
	for(; cur < end; cur++)
	{
		if(s[cur] == delim)
		{
			v.push_back(s.substr(beg,cur-beg));
			beg = ++cur;
		}
	}
	v.push_back(s.substr(end-(end-beg),end));
}

renamer::PARSE_ARGUMENTS_RETURN_CODE renamer::parseArguments(int argc, const char **argv) 
{
	for(size_t i = 0; i < argc; i++)
	{
		switch(argv[i][0])
		{
			case '-':
			{
				if(strcmp(argv[i]+1,"wl") == 0)
				{
					time_t _time = time(0);
					char time_string[30];
					if((i+1) < argc && argv[i+1][0] != '-')
					{
						strftime(time_string,sizeof(time_string),argv[++i],localtime(&_time));
					}
					else
					{
						std::cout << "[Warning]: Log format not specified. As default %d-%m-%Y\n";
						strftime(time_string,sizeof(time_string),"%d-%m-%Y",localtime(&_time));
					}
					this->logFilename = "logs/" + std::string(time_string) + ".log";

					Filesystem::_mkdir("logs");
					Filesystem::log.open(this->logFilename.c_str());

					this->writeLog = true;
				}
				else if(strlen(argv[i]) >= 2)
				{
					switch(argv[i][1])
					{
						case 'b':
						{
							if(++i >= argc || argv[i][0] == '-')
							{
								std::cout << "[Error]: Dir for bad files not specified.\n";
								return ERROR_BAD_DIR_NOT_SPECIFIED;
							}
							this->bDir = argv[i];
							break;
						}
						case 'g':
						{
							if(++i >= argc || argv[i][0] == '-')
							{
								std::cout << "[Error]: Dir for good files not specified.\n";
								return ERROR_GOOD_DIR_NOT_SPECIFIED;
							}
							this->gDir = argv[i];
							break;
						}
						case 'i': //rewrite to function
						{
							if(++i >= argc || argv[i][0] == '-')
							{
								std::cout << "[Error]: No ignored files specified for this flag.\n";
								return ERROR_NO_IGNORED_FILES_SPECIFIED;
							}
							try
							{
								this->fileToVector(argv[i], this->ignore, '\n');
							}
							catch(int &ex)
							{
								if(argv[i][0] != '-')
								{
									this->stringToVector(argv[i],this->ignore,',');
								}
								else
								{
									return ERROR_CANT_PARSE_FILE;
								}
							}
						}
						case 'e': //rewrite to function
						{
							if(++i >= argc || argv[i][0] == '-')
							{
								std::cout << "[Error]: No files extension specified for this flag.\n";
								return ERROR_NO_FILES_EXTENSION_SPECIFIED;
							}
							try
							{
								this->fileToVector(argv[i], this->ext, '\n');
							}
							catch(int &ex)
							{
								if(argv[i][0] != '-')
								{
									this->stringToVector(argv[i],this->ext,',');
								}
								else
								{
									return ERROR_CANT_PARSE_FILE;
								}
							}
							break;
						}
					}
				}

				break;
			}
			case '?':
			{
				this->Usage();

				break;
			}
			case 'h':
			{
				this->Usage();

				break;
			}
		}
	}

	return PARSE_SUCCESS;
}

renamer::~renamer() {}
#include "renamer.h"

renamer::renamer()
{
	this->gDir.reserve(100);
	this->bDir.reserve(100);
	this->bDir = "error/";
	this->cDir.reserve(100);
	this->ignore.push_back("n$$$.pdf");
}

int renamer::run(int &argc,char const *argv[])
{
	std::chrono::system_clock::time_point tm1 = std::chrono::system_clock::now(),
	tm2;
	time_t tm = std::chrono::system_clock::to_time_t(tm1);
	std::string log;
	((log += "Start ") += ctime(&tm)) += "\n";

	std::cout << log;
	int code = this->parseArguments(argc,argv);
	if(code != ERROR_ARGUMENT_ORDER || code != CANT_CREATE_DIR || code != CANT_OPEN_LOG)
		if(this->check_param())
		{
			Filesystem::log.write(log);
			code = this->processDir();
		}
	log.clear();
	

	
	
	tm2 = std::chrono::system_clock::now();
	int milleseconds_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(tm2-tm1).count();
	((log += "\nTotal ")+=std::to_string(this->total_good+this->total_bad))+=".\n";
	Filesystem::log.write(log);
	std::cout << log;
	log.clear();
	(log += "Total good ")+=(std::to_string(this->total_good)+=".\n");
	Filesystem::log.write(log);
	std::cout << log;
	log.clear();
	(log += "Total bad ")+=(std::to_string(this->total_bad)+=".\n");
	std::cout << log;
	Filesystem::log.write(log);
	log.clear();
	((log += "Elapsed time ")+=std::to_string(milleseconds_elapsed))+="ms.\n\n";
	Filesystem::log.write(log);
	std::cout << log;
	log.clear();

	tm = std::chrono::system_clock::to_time_t(tm2);
	(log += "End ") += ctime(&tm);
	std::cout << log;
	Filesystem::log.write(log);

	return code;
}

bool renamer::check_param()
{
	bool may_continue = 1;
	if(cDir.length() == 0)
	{
		std::cerr << "[Error]: Process directory must be specified.\n";
		Filesystem::log.write("[Error]: Process directory must be specified.\n");
		may_continue = 0;
	}
	if(bDir == "error/")
	{
		std::cerr << "[Warning]: Bad directory must be specified.\n";
		Filesystem::log.write("[Warning]: Bad directory must be specified.\n");
	}
	if(gDir.length() == 0)
	{
		std::cerr << "[Warning]: Good directory not specified.\n";
		Filesystem::log.write("[Warning]: Good directory not specified.\n");
	}
	if(ext.size() == 0)
	{
		std::cerr << "[Error]: Must be 1 or more file extension.\n";
		Filesystem::log.write("[Error]: Must be 1 or more file extension.\n");
		may_continue = 0;
	}

	return may_continue;
}

int renamer::processDir()
{
	std::string log;
	log.reserve(200);
	DIR *dir = opendir(cDir.c_str());
	if(!dir)
	{
		log += "Cant open dir '";
		log += cDir;
		log += "'.\n";
		std::cerr << log;
		Filesystem::log.write(log);

		return CANT_OPEN_DIR;
	}

	try
	{
		if(gDir.length() != 0)
			Filesystem::_mkdir((cDir+gDir).c_str());
		Filesystem::_mkdir((cDir+bDir).c_str());
	}
	catch(Filesystem::FilesystemException &ex)
	{
		std::cerr << ex.what() << '\n';
		Filesystem::log.write(ex.what());

		return CANT_CREATE_DIR;
	}


	
	dirent *dent;
	size_t ei = 0; //Extension index
	std::string _ext, filename;
	_ext.reserve(10);
	filename.reserve(200);
	while((dent = readdir(dir)))
	{
		//Clear buffers
		filename.clear();
		_ext.clear();

		//Check if dir
		if(dent->d_type == DT_DIR) continue;

		if(dent->d_name[0] != 'n') continue;

		//Set filename to buffer
		filename += dent->d_name;

		//Exclude ignored files
		auto it = find(this->ignore.begin(),this->ignore.end(),filename); //Ignore check
		if(it != this->ignore.end()) { continue; }

		ei = filename.rfind('.'); //get dot index
		if(ei == std::string::npos) continue; //if dot dont exist continue

		//Get file extension
		_ext += filename.substr(ei);
		filename.erase(ei,_ext.length()); //Erase extension from filename

		it = find(this->ext.begin(),this->ext.end(),_ext); //Extension check
		if(it == this->ext.end()) { continue; }

		if(this->rename == true)
			Filesystem::_rename((this->cDir+dent->d_name).c_str(), this->processFilename(dent->d_name,_ext.length()).c_str());
		else this->processFilename(dent->d_name,_ext.length());
	}

	closedir(dir);

	return 0;
}

bool renamer::validate_name(std::string &str,size_t defis)
{
	int length = str.length();
	if(defis <= 0) return false;
	if(length == 9)
	{
		if(str.find("05") == 0)
		{
			str.insert(0,"0");
		}
	}
	else if(length > 10 || length < 12)
	{
		if(str[10] == '-')
		{
			if(str.substr(0,3) == "005")
			{
				return true;
			}
			else
			{
				str.replace(0,3,"005");
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	// if(defis == 1 && newname.find("005") == 0 && (newname.length()-newname.find_last_of('-')) == 2)


	return true;
}

std::string renamer::processFilename(const char *name,size_t ext_size)
{
	std::string newname,newpath,log;
	size_t defis = 0;
	newname.reserve(50);
	newpath.reserve(256);
	log.reserve(256);
	newpath += cDir;
	size_t len = strlen(name)-ext_size, i = 0;
	for(; i < len; i++)
	{
		if(isdigit(name[i])) { newname += name[i]; continue; } //if digit dont touch and continue
		switch (name[i]) //if not try to replace known associations
		{
			case 'b': newname += '6'; break;
			case 'q': case 'g': newname += '9'; break;
			case '+': newname += '4'; break;
			case '!': case 'L': case 'l': case 'I': case 'i': case 'J': case 'j':
			case ']': case '[': case 't': case 'r': case '|' : newname +='1'; break;
			case 'S': case 's': newname += '5'; break;
			case 'O': case 'o': case ')': newname += '0'; break;
			case 'B': newname += '8'; break;
			case '-': newname += '-'; defis++; break;
		}
	}
	if(this->validate_name(newname,defis))
	{
		this->total_good++;
		if(gDir.length() != 0)
			newpath += gDir;
		newname += (name + (strlen(name) - ext_size));
		(log += "Move good dir [") += (cDir+name += "] to [") += (newpath += newname) + "]\n";
	}
	else
	{
		this->total_bad++;
		newname += (name + (strlen(name) - ext_size));
		newpath += bDir;
		(log += "Move error dir [") += (cDir+name += "] to [") += (newpath += newname) + "]\n";
	}
	std::cout << log;
	Filesystem::log.write(log);

	return newpath;
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
		std::cout << "Usage: renamer [DIR] -g [GOOD DIR] -b [BAD DIR] -e [FILE EXTENSIONS] -i [IGNORED FILES] -wl [FORMAT] -nr\n" 
		<< "Renames files in specified directory by pattern and puts it in specified bad folder or specified good folder.\n"
		<< "If good dir not specified puts renamed files at the same folder, if bad dir not specified creates error dir in working directory and puts bad files in it.\n"
		<< "File extensions must starts with dot and be in file with -ef or be splitted by ',' with -e flag.\n"
		<< "-wl Flag enable logging who creates 'logs' directory and puts to it all logs who been writed. You may specify log filename. Format you can see here 'http://www.cplusplus.com/reference/ctime/strftime/'\n"
		<< "-i Specifies the same as -e flag ignored files also may be read from file whos also ignored if extension match such as '.txt'\n"
		<< "-nr Flag used for debug it disbales rename and leaves only 'preprocessing' filenames.\n"
		<< "Warning!!! I recommend this arguments order and forbid variate it. -nr Position doesen't matter.\n\n";
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
	if(argc == 1) { this->Usage(); return STOP; }
	for(size_t i = 1; i < argc; i++)
	{
		switch(argv[i][0])
		{
			case '-':
			{
				if(strcmp(argv[i]+1,"wl") == 0)
				{
					std::string log('\0',50);
					time_t _time = time(0);
					char time_string[256];
					if((i+1) < argc && argv[i+1][0] != '-')
					{
						strftime(time_string,sizeof(time_string),argv[++i],localtime(&_time));
					}
					else
					{
						std::cout << "[Warning]: Log format not specified. As default %Y-%m-%d\n";
						strftime(time_string,sizeof(time_string),"%Y-%m-%d",localtime(&_time));
					}
					if(cDir.length() == 0)
					{
						std::cerr << "[Error]: Wrong arguments order.\n";
						return ERROR_ARGUMENT_ORDER;
					}
					log += cDir;
					log += "logs/";
					log += std::string(time_string);
					log += ".log";

					try
					{
						Filesystem::_mkdir((cDir+"logs").c_str());
					}
					catch(Filesystem::FilesystemException &ex)
					{
						std::cerr << ex.what();
						Filesystem::log.write(ex.what()+"\n");

						return CANT_CREATE_DIR;
					}
					Filesystem::log.open(log.c_str());
					if(!Filesystem::log.is_open())
					{
						std::cerr << "Cant write log '" << log << "'\n";
						return CANT_OPEN_LOG;
					}
				}
				else if(strlen(argv[i]) >= 2)
				{
					switch(argv[i][1])
					{
						case 'b':
						{
							if(cDir.length() == 0)
							{
								std::cerr << "[Error]: Wrong arguments order.\n";
								return ERROR_ARGUMENT_ORDER;
							}
							if(++i >= argc || argv[i][0] == '-')
							{
								std::cout << "[Warning]: Dir for bad files not specified.\n";
								//return ERROR_BAD_DIR_NOT_SPECIFIED;
							}
							this->bDir = argv[i];
							if(this->bDir.back() != Filesystem::slash)
								this->bDir += Filesystem::slash;
							break;
						}
						case 'g':
						{
							if(cDir.length() == 0)
							{
								std::cerr << "[Error]: Wrong arguments order.\n";
								return ERROR_ARGUMENT_ORDER;
							}
							if(++i >= argc || argv[i][0] == '-')
							{
								std::cout << "[Warning]: Dir for good files not specified.\n";
								//return ERROR_GOOD_DIR_NOT_SPECIFIED;
							}
							this->gDir = argv[i];
							if(this->gDir.back() != Filesystem::slash)
								this->gDir += Filesystem::slash;
							break;
						}
						case 'i': //rewrite to function
						{
							if(cDir.length() == 0)
							{
								std::cerr << "[Error]: Wrong arguments order. Ignored file list must be \n";
								return ERROR_ARGUMENT_ORDER;
							}
							if(++i >= argc || argv[i][0] == '-')
							{
								std::cout << "[Error]: No ignored files specified for this flag.\n";
								return ERROR_NO_IGNORED_FILES_SPECIFIED;
							}
							if(strcmp(argv[i-1],"-if") == 0)
							{
								this->fileToVector(argv[i], this->ignore, '\n');
								this->ignore.push_back(argv[i]);
							}
							else
							{
								this->stringToVector(argv[i],this->ignore,',');
							}
							break;
						}
						case 'e': //rewrite to function
						{
							if(cDir.length() == 0)
							{
								std::cerr << "[Error]: Wrong arguments order.\n";
								return ERROR_ARGUMENT_ORDER;
							}
							if(++i >= argc || argv[i][0] == '-')
							{
								std::cout << "[Error]: No files extension specified for this flag.\n";
								return ERROR_NO_FILES_EXTENSION_SPECIFIED;
							}
							if(strcmp(argv[i-1],"-ef") == 0)
							{
								this->fileToVector(argv[i], this->ext, '\n');
								if(this->ext.size() == 0)
								{
									return ERROR_EMPTY_FILE;
								}
								this->ignore.push_back(argv[i]);
							}
							else
							{
								this->stringToVector(argv[i],this->ext,',');
								if(this->ext.size() == 0)
								{
									return ERROR_EMPTY_FILE;
								}
							}
							break;
						}
						case 'n':
						{
							if(strcmp(argv[i],"-nr") == 0)
							{
								this->rename = false;
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
			default:
			{
				cDir = argv[i];
				if(cDir.back() != Filesystem::slash) cDir += Filesystem::slash;

				break;
			}
		}
	}

	return PARSE_SUCCESS;
}

renamer::~renamer() {}
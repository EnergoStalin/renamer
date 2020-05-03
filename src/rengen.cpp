//#include <windows.h>
#include <sys/stat.h>
#include <fstream>
#include <ctime>
#include <random>
#include <string.h>

#define FILEDIR "gendir"
#define COUNT 10

char filename[24];
char first[] = ")0oO5";
char middle_end[] = "0123456789oObqg+!LlIiJj[]trsSB_' $.@^-";

void saddch(char *s,char c)
{
	int len = strlen(s);
	s[len] = c;
	s[len+1] = '\0';
}

void generateFilename(const char *prefix)
{
	filename[0] = 0;
	strcat(filename,prefix);
	strcat(filename,"/n");
	
	saddch(filename,first[rand()%4]);
	saddch(filename,first[rand()%4]);
	saddch(filename,first[rand()%4]);
	
	saddch(filename,middle_end[rand()%38]);
	saddch(filename,middle_end[rand()%38]);
	saddch(filename,middle_end[rand()%38]);
	saddch(filename,middle_end[rand()%38]);
	saddch(filename,middle_end[rand()%38]);
	saddch(filename,middle_end[rand()%38]);
	saddch(filename,middle_end[rand()%38]);
	saddch(filename,middle_end[rand()%38]);
	saddch(filename,middle_end[rand()%38]);
	saddch(filename,middle_end[rand()%38]);
	saddch(filename,'-');
	saddch(filename,middle_end[rand()%10]);
	
	int i = strlen(filename);
	for(; i < 20; i++)
	{
		if(((float)rand() / RAND_MAX) > 0.5)
		{
			filename[i] = (rand() % 25) + 65;
		}
		else
		{
			filename[i] = (rand() % 25) + 97;
		}
	}
	filename[i] = 0;
	
	strcat(filename,".txt");
}

int main(int argc, char const *argv[])
{
	int count = atoi(argv[1]);
	srand(time(0));
	//CreateDirectory(FILEDIR,NULL);
	mkdir(argv[2], 0);
	std::FILE *ptr = NULL;
	
	for(int i = 0; i < count; i++)
	{
		generateFilename(argv[2]);
		ptr = std::fopen(filename,"w");
		fclose(ptr);
		/*CreateFileA(
			filename,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);*/
	}
	
	return 0;
}
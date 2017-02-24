#ifndef __DIR__H_
#define __DIR__H_


#include <string>
#include <vector>
#include "file.h"
#include "Idirectory.h"


#include <string>
#include <vector>
#include "file.h"
#include "windir.h"
#include "unixdir.h"
#define MAX_PATH 4096
class IDir
{
	//virtual void Chdir();
    //virtual void MkDir(const char *dirName) = 0;
	//virtual char *ListFiles(const char *dirName) = 0;
    //virtual char *NListFiles(const char *dirName) = 0;
};

class Dir: public IDir
{
public:
	IDir *dir;
	char path[MAX_PATH];
	Dir()
	{
#ifdef _WIN32

#elif __unix__

#endif
	}


	//char *ListFiles(const char *dirName)
	//{
	//	return NULL;
	//}

/*	static char *NListFiles(const char *dirName)
	{
		return NULL;
    }*/

	static void MkDir(const char *dirName)
	{
#ifdef _WIN32

#elif __unix__
      UnixDir::MkDir(dirName);
#endif
	}

    static char * ListFiles(const char *path, char ***listFiles)
    {
#ifdef _WIN32
        return WindowsDir::ListFiles(path);
#elif __unix__
        return UnixDir::ListFiles(path, listFiles);
#endif
    }

    static char *ListFiles(const char *path)
	{
#ifdef _WIN32
		return WindowsDir::ListFiles(path);
#elif __unix__
		return UnixDir::ListFiles(path);
#endif
	}
	
};

//
//using namespace std;
//
//class WindowsDir
//{
//public:
//
//	void GetFilesInDirectory();
//
//
//};
//
//class UnixDir
//{
//
//};
//
//
//class Dir
//{
//	static void GetFilesInDirectory()
//	{
//
//	}
//
//	static void GetFilesWithInformationInDirectory()
//	{
//
//	}
//}
//void GetFilesInDirectory(std::vector<string> &out, const string &directory)
//{
//#ifdef _WIN32
//	HANDLE dir;
//	WIN32_FIND_DATA file_data;
//
//	if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
//		return; /* No files found */
//
//	do 
//	{
//		const string file_name = file_data.cFileName;
//		const string full_file_name = directory + "/" + file_name;
//		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
//
//		if (file_name[0] == '.')
//			continue;
//		string info;
//		if (is_directory)
//			info += 'd';
//		else if (file_data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT != 0)
//			info += 'l';
//		else
//			info += '-';
//
//		file_data.ftCreationTime;
//		file_data.
//
//		FileTimeToSystemTime(file.da)
//		if (is_directory)
//			continue;
//
//		out.push_back(full_file_name);
//	} 
//	while (FindNextFile(dir, &file_data));
//
//	FindClose(dir);
//#else
//	DIR *dir;
//	class dirent *ent;
//	class stat st;
//
//	dir = opendir(directory);
//	while ((ent = readdir(dir)) != NULL) {
//		const string file_name = ent->d_name;
//		const string full_file_name = directory + "/" + file_name;
//
//		if (file_name[0] == '.')
//			continue;
//
//		if (stat(full_file_name.c_str(), &st) == -1)
//			continue;
//
//		const bool is_directory = (st.st_mode & S_IFDIR) != 0;
//
//		if (is_directory)
//			continue;
//
//		out.push_back(full_file_name);
//	}
//	closedir(dir);
//#endif
//} // GetFilesInDirectory
//
//shareimprove this answer
//
//edited Nov 18 '13 at 11:19
//lahjaton_j
//360310
//
//answered Dec 19 '09 at 12:58
//Andreas Bonini
//23.4k20101140
//
//
//
//The Windows code doesn't quite work when compiled with Unicode enabled. I got around this by explicitly calling the ASCII functions, which have an 'A' appended. Specifically, use WIN32_FIND_DATAA, FindFirstFileA, and FindNextFileA and everything works. Obviously this is a hack, but I live in an English speaking country so it works for me. :0 Thanks for the example code! – Joe Dec 18 '15 at 19:29
//
//
//Which headers do you include for this to work ? – tuple_cat Apr 23 at 12 : 40
//
//
//Can't use dirent if you are making a lib. – Katianie May 25 at 22:37
//add a comment
//up vote
//22
//down vote
//
//
//Here's an example in C on Linux. That's if, you're on Linux and don't mind doing this small bit in ANSI C.
//
//#include <dirent.h>
//
//DIR *dpdf;
//struct dirent *epdf;
//
//dpdf = opendir("./");
//if (dpdf != NULL) {
//	while (epdf = readdir(dpdf)) {
//		printf("Filename: %s", epdf->d_name);
//		// std::cout << epdf->d_name << std::endl;
//	}
//}
//
#endif

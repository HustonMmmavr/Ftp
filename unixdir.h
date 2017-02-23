#ifndef UNIXDIR_H
#define UNIXDIR_H
#ifdef __unix__
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "exeption.h"

class UnixDir
{
private:
    static void SetPermision(char *charPermission, char value, long permissionsFlag, long flag)
    {
        if (permissionsFlag & flag)
            *charPermission = value;
        else
            *charPermission = '-';
    }

    static void CharPermissions(char* permissionBuffer, long permissionsFlag)
    {
        SetPermision(&permissionBuffer[0], 'r', permissionsFlag, S_IRUSR);
        SetPermision(&permissionBuffer[1], 'w', permissionsFlag, S_IWUSR);
        SetPermision(&permissionBuffer[2], 'x', permissionsFlag, S_IXUSR);
        SetPermision(&permissionBuffer[3], 'r', permissionsFlag, S_IRGRP);
        SetPermision(&permissionBuffer[4], 'w', permissionsFlag, S_IWGRP);
        SetPermision(&permissionBuffer[5], 'x', permissionsFlag, S_IXGRP);
        SetPermision(&permissionBuffer[6], 'r', permissionsFlag, S_IROTH);
        SetPermision(&permissionBuffer[7], 'w', permissionsFlag, S_IWOTH);
        SetPermision(&permissionBuffer[8], 'x', permissionsFlag, S_IXOTH);
    }

    static char FileType(const char *fileName)
    {
        struct stat st;
        stat(fileName, &st);
        mode_t mode = st.st_mode;

        if(S_ISDIR(mode)) return 'd';
        if(S_ISCHR(mode)) return 'c';
        if(S_ISBLK (mode)) return 'b';
        if(S_ISREG(mode)) return '-';
        if(S_ISFIFO(mode)) return 'p';
        if(S_ISLNK(mode)) return 'l';
        if(S_ISSOCK(mode)) return 's';
    }

    static char FileType(mode_t mode)
    {
        if(S_ISDIR(mode)) return 'd';
        if(S_ISCHR(mode)) return 'c';
        if(S_ISBLK (mode)) return 'b';
        if(S_ISREG(mode)) return '-';
        if(S_ISFIFO(mode)) return 'p';
        if(S_ISLNK(mode)) return 'l';
        if(S_ISSOCK(mode)) return 's';
    }

#define STRING_LIST_SIZE 32
#define SIZE_FILE_STRING 512
public:

    static void  MkDir(const char *dirName, mode_t mode = S_IRWXO | S_IRWXU | S_IRWXG)
    {
        if( mkdir(dirName, mode)  == -1)
            ThrowExceptionWithCode("Cant create dir, errno = ", errno);
    }

    //static MkDir(const char *dirName, DirMode mode)

    static int ListFiles(const char *dirName, char ***listFiles)
    {
        DIR *dir;
        dirent *ent;
        std::vector<char*> vec;
        char **list = new char*[STRING_LIST_SIZE];
        for (int i = 0; i < STRING_LIST_SIZE; i++)
            list[i] = new char[SIZE_FILE_STRING];
        struct stat st;
        int currentCountStrings = 0;
        int sizeStringsList = STRING_LIST_SIZE;
        int sizeListFiles = STRING_LIST_SIZE;
        dir = opendir(dirName);
        char fileName[PATH_MAX];
        char absoluteFileName[PATH_MAX];
        char uName[128], gName[128];
        char permissions[10];
        char fileType;
        unsigned long long fSize;
        struct passwd* pwd;
        struct group* gr;
        struct tm * ptm;
        uName[0] = '\0';

        while ((ent = readdir(dir)) != NULL)
        {
            strcpy(fileName, ent->d_name);
            sprintf(absoluteFileName, "%s%c%s", dirName, '/', fileName);

            //if (fileName[0] != '.')
            {
                if (stat(absoluteFileName, &st) == -1)
                    ThrowExceptionWithCode("Error stat, errno", errno);

                mode_t mode = st.st_mode;
                fileType = FileType(mode);

                // Get Name user and group
                pwd = getpwuid(st.st_uid);
                gr = getgrgid(st.st_gid);
                if (pwd) strcpy(uName, pwd->pw_name);
                if (gr) strcpy(gName, gr->gr_name);

                // get permissions rw...
                CharPermissions(permissions, mode);

                fSize = st.st_size;

                ptm = gmtime(&(st.st_mtime));
                if (!ptm)
                    ThrowExceptionWithCode("Error gmtime, errno", errno);

                // write data to resultString

                if (currentCountStrings >= sizeStringsList - 1)
                {
                    *listFiles = list;
                    return currentCountStrings - 1;
                }
                    ResizeStringArray(&list, &sizeStringsList, 2);

                sprintf(list[currentCountStrings++], "%c%s %s %s %llu %d %d %d %s %c%c", fileType, permissions, uName, gName, fSize, ptm->tm_mon, ptm->tm_mday, ptm->tm_year, fileName, '\r', '\n');

            }
        }
        //sprintf(resultString, "%c%c", '\r','\n');
        *listFiles = list;
        closedir(dir);
        return currentCountStrings;
    }

    static char* ListFiles(const char *dirName)
    {
        DIR *dir;
        dirent *ent;
        struct stat st;
        char resultString[1024 + PATH_MAX];
        int sizeListFiles = 1024;
        int currentLen = 0;
        char *listFiles = new char[sizeListFiles];
        dir = opendir(dirName);
        char fileName[PATH_MAX];
        char absoluteFileName[PATH_MAX];
        char uName[128], gName[128];
        char permissions[10];
        char fileType;
        unsigned long long fSize;
        struct passwd* pwd;
        struct group* gr;
        struct tm * ptm;
        uName[0] = '\0';

        while ((ent = readdir(dir)) != NULL)
        {
            strcpy(fileName, ent->d_name);
            sprintf(absoluteFileName, "%s%c%s", dirName, '/', fileName);

            //if (fileName[0] != '.')
            {
                if (stat(absoluteFileName, &st) == -1)
                    ThrowExceptionWithCode("Error stat, errno", errno);

                mode_t mode = st.st_mode;
                fileType = FileType(mode);

                // Get Name user and group
                pwd = getpwuid(st.st_uid);
                gr = getgrgid(st.st_gid);
                if (pwd) strcpy(uName, pwd->pw_name);
                if (gr) strcpy(gName, gr->gr_name);

                // get permissions rw...
                CharPermissions(permissions, mode);

                fSize = st.st_size;

                ptm = gmtime(&(st.st_mtime));
                if (!ptm)
                    ThrowExceptionWithCode("Error gmtime, errno", errno);

                // write data to resultString
                sprintf(resultString, "%c%s %s %s %llu %d %d %d %s %c", fileType, permissions, uName, gName, fSize, ptm->tm_mon, ptm->tm_mday, ptm->tm_year, fileName, '\n');
                int len = strlen(resultString)+1;

                if (currentLen + len >= sizeListFiles)
                    ResizeString(&listFiles, &sizeListFiles, 2);

                currentLen += len;
                strcat(listFiles, resultString);
            }
        }
        int c = strlen(listFiles);
        strcat(listFiles, "\r\n");
        closedir(dir);
        return listFiles;
    }
};
#endif
#endif // UNIXDIR_H

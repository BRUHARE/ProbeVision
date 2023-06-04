#pragma once
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <vector>
#include <sys/io.h>
using namespace std;

static void find_dir_file(std::string path, vector<string>& files){
    struct dirent **namelist;
    int n;
    n = scandir(path.c_str(),&namelist,0,alphasort);
    if(n < 0)
    { 
        cout << "scandir return "<< n  << endl;
    }
    else
    {
        int index=0;
        while(index < n)
        {
            if(strcmp(namelist[index]->d_name, ".") != 0 && strcmp(namelist[index]->d_name, "..") != 0){
                files.push_back(namelist[index]->d_name);
            }
            free(namelist[index]);
            index++;
        }
        free(namelist);
    }
}

static void StringSplit(string str,const char split, vector<string>& str_vec)
{
	istringstream iss(str);	
	string token;		
    str_vec.clear();	
	while (getline(iss, token, split))	
	{
        str_vec.push_back(token);
	}
}

/* Check whether the path identified by 'filename' is a file */
static bool isFile(const std::string& filename) {
    struct stat   buffer;
    return (stat (filename.c_str(), &buffer) == 0 && S_ISREG(buffer.st_mode));
}
 
/* Check whether the path identified by 'filefodler' is a fodler */
static bool isDirectory(const std::string& filefodler) {
    struct stat   buffer;
    return (stat (filefodler.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

static int removeFileAndDirectory(const std::string& path)
{
    int result = 0;
    DIR * p_dir;
    struct dirent * p_dirent;
    if(isDirectory(path))
    {
        if((p_dir = opendir(path.c_str())) == NULL )  
        {  
            std::cout << "Opendir error: " << strerror(errno) << std::endl;
            return -1;  
        }  
 
        while((p_dirent = readdir(p_dir)) != NULL)
        {
            std::string file_name = path + "/" + p_dirent->d_name;
            /* It is a directory */
            if(isDirectory(file_name) && (0 != strcmp(p_dirent->d_name, ".")) && (0 != strcmp(p_dirent->d_name, "..")))
            {
                result = removeFileAndDirectory(file_name);
                if(result < 0)
                {
                    return result;
                }
            }
            /* It is a file */
            else if((0 != strcmp(p_dirent->d_name, ".")) && (0 != strcmp(p_dirent->d_name, "..")))
            {
                result = remove(file_name.c_str());
                if(result < 0)
                {
                    return result;
                }
            }
            else
            {
                /* do nothing */
            }
        }
        closedir(p_dir);
        result = rmdir(path.c_str());
    }
    else if(isFile(path))
    {
        result = remove(path.c_str());
    }
    else
    {
        /* do nothing */
    }
    return result;
}

static void mkdir_path(string path)
{
    removeFileAndDirectory(path);
    int isCreate = mkdir(path.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
    if( !isCreate )
        printf("create path:%s\n",path.c_str());
    else
        printf("create path failed! error code : %s \n",path.c_str());
}

static string float2string(float f){
    char s[20];
    sprintf(s,"%.4f",f);
    return string(s);
}

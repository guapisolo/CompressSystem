#pragma once
#include <cstdio>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include "File.h"
#include "ZipAccessor.h"
#include "FileAccessor.h"

struct ZipFileList{
std::map<int,File>mp;
int cnt;
const char* listPath = "../archives/ZipFileList.txt";
std::ifstream inputFile;
std::ofstream outputFile;
ZipAccessor* zipAccessor;
ZipFileList(){
    cnt = 0;
    zipAccessor = NULL;
}
void load();
void exit();
int findFile(std::string path);
int addFile(File file, std::vector<char>&str);
int delFile(int id);
File get_file_info(int id);
std::pair<int,int> calc_offset(int id);
std::vector<char> ReadFile(int id);

int zip_file(std::string path);
int unzip_file(std::string path);
};

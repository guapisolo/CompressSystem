#pragma once
#include "File.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>
#include <cstring>
#include <string>
#include <vector>
#include <filesystem>
#include <regex>

namespace FileAccessor{
std::string getFileName(const std::string& filePath);
std::string getAbsolutePath(const std::string& filePath);
std::string getFileExtension(const std::string& fileName);
File get_info(std::string filePath);
std::vector<char> readFileStream(const std::string& filePath);
int writeFileStream(const std::string& filePath, std::vector<char>&str);
std::vector<std::string> selectFiles(const std::string& rootPath, const std::string& regex_str);
};

#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <vector>

struct ZipAccessor{
const char *filePath = "../archives/Files.zip";
std::ifstream inputFile;
std::ofstream outputFile;
std::vector<char> buffer;
ZipAccessor(){}
~ZipAccessor(){}
int openZip();
void closeZip(int writeBack);
int add(std::vector<char> &str);
int del(int L, int R);
std::vector<char> query(int L, int R);
};



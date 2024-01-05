#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <vector>

struct ZipAccessor{
const char *filePath = "../archives/Files.zip";
const char *ConfigureFilePath = "../archives/Configure.txt";
std::ifstream inputFile;
std::ofstream outputFile;
std::vector<char> buffer, zip;
std::vector<char> password;
int son[512][2];
int freq[256];
std::vector<int> mpToIssac[256],tmp;
int zipLen;
bool isempty;
ZipAccessor(){}
~ZipAccessor(){
    inputFile.close();
    outputFile.close();
}
void dfs(int x);
void encode();
int decode();
int openZip();
void closeZip(int writeBack);
void en_decry(std::vector<char> &s);
int add(std::vector<char> &str);
int del(int L, int R);
std::vector<char> query(int L, int R);
};



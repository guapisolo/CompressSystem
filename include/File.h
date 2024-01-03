#pragma once
#include <string>
#include <cstring>
#include "Time.h"

struct File{
std::string path, name, type, group, owner;
Time time;
int auth;
size_t size;
char md5[32];
File(){
    path = name = type = group = owner = "";
    auth = 0;
    size = 0;
    memset(md5, 0, sizeof(md5));
}
};

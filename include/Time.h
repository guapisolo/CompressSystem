#pragma once
#include <ctime>

struct Time{
std::time_t rawTime;
Time(){
    rawTime = 0;
}
Time(std::time_t t){
    rawTime = t;
}
//select time interval
};

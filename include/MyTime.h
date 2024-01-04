#pragma once
#include <ctime>

struct MyTime{
std::time_t rawMyTime;
MyTime(){
    rawMyTime = 0;
}
MyTime(std::time_t t){
    rawMyTime = t;
}
//select time interval
};

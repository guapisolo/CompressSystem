#include <chrono>
#include <sstream>
#include <iomanip>
#include "ZipFileList.h"

namespace CmdInterface{
std::vector<std::string> splitStringBySpace(const std::string& str);
time_t convertToDate(const std::string& dateStr);
void splitString(const std::string& str, std::string& part1, std::string& part2);
void setup();
};


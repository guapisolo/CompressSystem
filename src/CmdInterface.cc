#include "CmdInterface.h"

namespace CmdInterface{
    
std::vector<std::string> splitStringBySpace(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> results;
    std::string token;
    while (iss >> token) {
        results.push_back(token);
    }
    return results;
}

// YYYY.DD.MM -> time_t
time_t convertToDate(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);

    // 解析字符串
    ss >> std::get_time(&tm, "%Y.%m.%d");

    if (ss.fail()) {
        std::cerr << "解析错误: " << dateStr << std::endl;
        exit(0);
    }

    // 转换为time_t
    std::time_t time = std::mktime(&tm);
    if (time == -1) {
        std::cerr << "转换错误: " << dateStr << std::endl;
        exit(0);
    }

    return time;
}

//将字符串str分割为'-'前、后两部分
void splitString(const std::string& str, std::string& part1, std::string& part2) {
    size_t pos = str.find('-');
    if (pos != std::string::npos) {
        part1 = str.substr(0, pos);
        part2 = str.substr(pos + 1);
    } else {
        part1 = str;  // 如果没有找到'-'，整个字符串都是第一部分
        part2 = "";   // 第二部分为空
    }
}

void setup()
{
    ZipFileList core;
    std::string userInput;
    // system("clear");
    std::cerr << "欢迎来到文件备份系统:" << std::endl;
    std::cerr << "输入 help 获取帮助" << std::endl;
    std::cerr << "请务必不要使用 ctrl+c 等方式终止程序，请使用 exit 指令推出" << std::endl;
    core.load();
    while(1) {
        std::cerr << ":";
        std::getline(std::cin, userInput);
        std::vector<std::string> cmds = splitStringBySpace(userInput);
        if(cmds[0] == "help" && cmds.size() == 1) {
            std::cerr << "备份文件  arch [file path]" << std::endl;
            std::cerr << "提取文件  get [file path] ([password])" << std::endl;
            std::cerr << "设置加密  encry [new password]" << std::endl;
            std::cerr << "取消加密  decry [old password]" << std::endl;
            std::cerr << "重置密码  reset [new password] [old password]" << std::endl;
            std::cerr << "筛选备份  select [folder] [file name (regexp)] [file types split with \'|\'] [last edit time (YYYY.MM.DD-YYYY.MM.DD)] [file size (B) (s1-s2)]" << std::endl;
            std::cerr << "筛选备份中不需要的项用'*'代替" << std::endl;
            std::cerr << "上传备份  upload" << std::endl;
            std::cerr << "下载备份  download" << std::endl;
            std::cerr << "后台备份  autoarch" << std::endl;
            std::cerr << "退出程序  exit" << std::endl;
        }else if(cmds[0] == "arch" && cmds.size() == 2) {
            core.zip_file(cmds[1]);
        }else if(cmds[0] == "get" && cmds.size() >= 2 && cmds.size() <=3) {
            if(cmds.size() == 2) {
                core.unzip_file(cmds[1]);
            }else if(cmds.size() == 3) {
                core.unzip_file(cmds[1], cmds[2]);
            }
        }else if(cmds[0] == "encry" && cmds.size() == 2) {
            core.set_password(cmds[1]);
        }else if(cmds[0] == "decry" && cmds.size() == 2) {
            core.reset_password(cmds[1]);
        }else if(cmds[0] == "reset" && cmds.size() == 3) {
            core.set_password(cmds[1], cmds[2]);
        }else if(cmds[0] == "select" && cmds.size() <= 6) {
            int l = cmds.size();
            std::string tmp1, tmp2;
            time_t timeL = 0, timeR = 0; //初值都赋值为特殊值，以初值没变代表无限制
            long long sizeL = -1, sizeR = -1;
            if (l > 2 && cmds[2] == "*") cmds[2] = ".*";
            if (l > 3 && cmds[3] == "*") cmds[3] = ".*";
            if (l > 4 && cmds[4] != "*") {
                splitString(cmds[4], tmp1, tmp2);
                timeL = convertToDate(tmp1);
                timeR = convertToDate(tmp2);
            }
            if (l > 5 && cmds[5] != "*") {
                splitString(cmds[5], tmp1, tmp2);
                sizeL = std::stoll(tmp1);
                sizeR = std::stoll(tmp2);
            }
            std::vector<std::string> files = FileAccessor::selectFiles(cmds[1], 
                                                cmds[2] + "\\.(" + cmds[3] + ")$",
                                                timeL, timeR,
                                                sizeL, sizeR); //".*\\.(txt|pdf|gif)$"
            std::cerr << "选择了下列文件：" << std::endl;
            for (auto p : files) {
                std::cerr << p << std::endl;
            }
            core.zip_file(files);
        }else if(cmds[0] == "upload" && cmds.size() == 1) {
            core.netDiskInit();
            core.netDiskUpload();
        }else if(cmds[0] == "download" && cmds.size() == 1) {
            core.netDiskInit();
            core.netDiskUpload();
        }else if(cmds[0] == "autoload" && cmds.size() == 1) {
            core.auto_zip_file();
        }else if(cmds[0] == "exit" && cmds.size() == 1) {
            core.exit();
            break;
        }else {
            std::cerr << "不合法的输入" << std::endl;
        }
    }
}

};
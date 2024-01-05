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
            std::cerr << "筛选备份  select [folder] [file types split with \'|\']" << std::endl;
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
        }else if(cmds[0] == "select" && cmds.size() == 3) {
            std::vector<std::string> files = FileAccessor::selectFiles(cmds[1], 
                                                ".*\\.(" + cmds[2] + ")$"); //".*\\.(txt|pdf|gif)$"
            core.zip_file(files);
        }else if(cmds[0] == "upload" && cmds.size() == 1) {
            core.netDiskInit();
            core.netDiskUpload();
        }else if(cmds[0] == "download" && cmds.size() == 1) {
            core.netDiskInit();
            core.netDiskUpload();
        }else if(cmds[0] == "autoload" && cmds.size() == 1) {
            // core.auto_load();
        }else if(cmds[0] == "exit" && cmds.size() == 1) {
            core.exit();
            break;
        }else {
            std::cerr << "不合法的输入" << std::endl;
        }
    }
}

};
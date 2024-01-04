#ifdef test
#include "../include/FileAccessor.h"
#else
#include "FileAccessor.h"
#endif

std::string FileAccessor::getFileName(const std::string& filePath) {
    // 查找最后一个路径分隔符
    size_t lastSlash = filePath.find_last_of("/\\");
    if (lastSlash == std::string::npos) {
        return filePath; // 没有路径分隔符，整个字符串是文件名
    } else {
        return filePath.substr(lastSlash + 1); // 提取文件名
    }
}

std::string FileAccessor::getFileExtension(const std::string& fileName) {
    // 查找最后一个点号，点号后面是文件后缀
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos == std::string::npos) {
        return ""; // 没有找到点号，没有后缀
    } else {
        return fileName.substr(dotPos + 1); // 提取后缀
    }
}

File FileAccessor::get_info(std::string filePath)
{
    struct stat fileInfo;
    File file;
    if (stat(filePath.c_str(), &fileInfo) != 0) {
        std::cerr << "无法获取文件信息" << std::endl;
        return file;
    }

    file.path = filePath;
    file.name = getFileName(file.path);
    file.type = getFileExtension(file.name);

    // 获取并打印文件属主
    struct passwd *pw = getpwuid(fileInfo.st_uid);
    struct group  *gr = getgrgid(fileInfo.st_gid);
    file.owner = pw->pw_name;
    file.group = gr->gr_name;

    file.time.rawMyTime = fileInfo.st_mtime;
    file.auth = (fileInfo.st_mode & 0777);
    file.size = fileInfo.st_size;

    return file;
}

// read file stream from file system
std::vector<char> FileAccessor::readFileStream(const std::string& filePath)
{
    std::ifstream inputFile(filePath, std::ios::binary);
    std::vector<char> content;
    if(!inputFile) {
        std::cerr << "无法打开文件" << std::endl;
        return content;
    }
    content.assign((std::istreambuf_iterator<char>(inputFile)), 
                   std::istreambuf_iterator<char>());
    inputFile.close();
    return content;
}

int FileAccessor::writeFileStream(const std::string& filePath, std::vector<char>&str)
{
    std::ofstream outputFile(filePath, std::ios::binary | std::ios::trunc);
    if(!outputFile) {
        std::cerr << "无法打开文件" << std::endl;
        return 0;
    }
    outputFile.write(str.data(), str.size());
    outputFile.flush();
    outputFile.close();
    return 1;
}

#ifdef test
int main()
{
    std::string path = "/home/guapisolo/code/SWExp2/code/test.pdf";
    std::string name = FileAccessor::getFileName(path);
    std::string type = FileAccessor::getFileExtension(name);

    std::cout << path << std::endl;
    std::cout << name << std::endl;
    std::cout << type << std::endl;
}
#endif
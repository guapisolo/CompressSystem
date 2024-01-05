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

time_t FileAccessor::getEditTime(const std::string& filePath) {
    namespace fs = std::filesystem;
    // 检查文件是否存在
    if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
        // 获取最后修改时间
        auto ftime = fs::last_write_time(filePath);
        auto cftime = decltype(ftime)::clock::to_time_t(ftime); // 转换为time_t
        return cftime;
    } else {
        std::cerr << "文件不存在或不是一个普通文件" << std::endl;
    }
    exit(0);
    return 0;
}

long long FileAccessor::getFileSize(const std::string& filePath) {
    namespace fs = std::filesystem;
    // 检查文件是否存在
    if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
        // 获取文件大小
        return (long long)(fs::file_size(filePath));
    } else {
        std::cerr << "文件不存在或不是一个普通文件" << std::endl;
    }
    return 0;
}

std::string FileAccessor::getAbsolutePath(const std::string& filePath) {
    return std::filesystem::absolute(filePath).string();
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

//通过正则表达式、大小、修改时间等限制筛选根目录下文件
std::vector<std::string> FileAccessor::selectFiles(const std::string rootPath, const std::string regex_str, time_t timeL, time_t timeR, long long sizeL, long long sizeR)
{
    namespace fs = std::filesystem;
    std::vector<std::string> filteredFiles;
    std::regex pattern(regex_str);

    std::queue<std::string> path_q;
    path_q.push(rootPath);
    try {
        // 检查路径是否存在并且是一个目录
        while(!path_q.empty()) {
            std::string nowPath = path_q.front(); // 广度优先搜索
            path_q.pop();
            if (fs::exists(nowPath) && fs::is_directory(nowPath)) {
                // 遍历目录
                for (const auto& entry : fs::directory_iterator(nowPath)) {
                    const auto& path = entry.path();
                    std::string pathString = path.string();
                    if(fs::is_directory(pathString)) {
                        path_q.push(pathString);
                        continue;
                    }
                    std::string fileName = getFileName(pathString);

                    time_t editTime = getEditTime(pathString);

                    long long fileSize = getFileSize(pathString);
                    

                    // 如果文件名与正则表达式不匹配，跳过该文件
                    if (!std::regex_match(fileName, pattern)) continue;
                    // 如果文件最后修改时间不在区间内，跳过该文件
                    if (timeR > 0 && (editTime < timeL || editTime > timeR)) continue;
                    
                    // 如果文件大小不在区间内，跳过该文件
                    // std::cerr << pathString << "  " << fileSize << "  " << sizeL << "  " << sizeR << " " << (fileSize > sizeL) << std::endl;
                    if (sizeL != -1 && (fileSize < sizeL || fileSize > sizeR)) continue;

                    filteredFiles.push_back(pathString);
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }

    return filteredFiles;
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
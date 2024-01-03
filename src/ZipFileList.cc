#include "ZipFileList.h"

// 每次启动程序，读取压缩文件列表信息
void ZipFileList::load()
{
    inputFile.open(listPath);
    if(!inputFile) {
        std::cerr << "找不到备份表 ZipFileList.txt" << std::endl;
        std::exit(0);
    }
    zipAccessor = new ZipAccessor;
    int id;
    File file;
    while(inputFile >> id) {
        inputFile >> file.path >> file.name >> file.type >> file.group >> file.owner;
        inputFile >> file.time.rawTime >> file.auth >> file.size;
        // inputFile >> file.md5;
        mp[id] = file;
    }
    inputFile.close();
}

// 退出时保存信息表
void ZipFileList::exit()
{
    outputFile.open(listPath, std::ios::trunc);
    for(auto x : mp) {
        outputFile << x.first << " ";
        File file = x.second;
        outputFile << file.path << " " << file.name << " " << file.type << " " << file.group << " " << file.owner << " ";
        outputFile << file.time.rawTime << " " << file.auth << " " << file.size << std::endl;
        /* file.md5 */ 
    }
    outputFile.close();
    delete(zipAccessor);
}

// 在信息表里找文件，找到了返回文件id，找不到返回0
int ZipFileList::findFile(std::string path)
{
    for(auto x : mp) {
        if(x.second.path == path) return x.first;
    }
    return 0;
}

//计算偏移，返回偏移值及文件大小
std::pair<int,int> ZipFileList::calc_offset(int id)
{
    int offset = 0, size = -1;
    for(auto x: mp) {
        if(x.first == id) {
            size = x.second.size;
            break;
        }
        offset += x.second.size;
    }
    if(size == -1) return {0,0};
    return {offset, size};
}

std::vector<char> ZipFileList::ReadFile(int id)
{
    auto info = calc_offset(id);
    return zipAccessor->query(info.first, info.first + info.second);
}

// 添加新的文件到zip中
int ZipFileList::addFile(File file, std::vector<char> &str) 
{
    auto status = zipAccessor->add(str);
    if(status == 0) return 0;
    int id = ++cnt;
    mp[id] = file;
    return 1;
}

// 如果文件有改动，则吧文件从zip中删除
int ZipFileList::delFile(int id)
{
    auto info = calc_offset(id);
    return zipAccessor->del(info.first, info.first + info.second);
}

// 每次检查该文件是否在表里存在，如果存在就覆盖成新的备份
// 他没要求，所以暂时没有保存历史版本的功能
int ZipFileList::zip_file(std::string path)
{
    int id = findFile(path);
    if(id){
        auto s = delFile(id);
        if(!s) return 0;
    }
    File file;
    file.path = path;
    file = FileAccessor::get_info(path);
    std::vector<char>str = FileAccessor::readFileStream(path);
    // file.md5 = Md5::convert(str);
    auto s = addFile(file, str);
    if(!s) return 0;
    return 1;
}

int ZipFileList::unzip_file(std::string path)
{
    int id = findFile(path);
    if(!id){
        std::cerr << "备份中找不到该文件" << std::endl;
        return 0;
    }
    auto x = calc_offset(id);
    std::vector<char>str = zipAccessor->query(x.first, x.second);
    auto s = FileAccessor::writeFileStream(path, str);
    if(!s) return 0;
    return 1;
}

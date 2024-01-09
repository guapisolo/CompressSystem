#ifdef test
#include "../include/ZipFileList.h"
#else
#include "ZipFileList.h"
#endif
#include <unistd.h>

std::vector<char> str_to_vec(std::string str)
{
    std::vector<char> vec(str.begin(), str.end());
    return vec;
}
std::string vec_to_string(std::vector<char> vec)
{
    std::string str(vec.begin(), vec.end());
    return str;
}

// 每次启动程序，读取压缩文件列表信息
void ZipFileList::load()
{
    inputFile.open(listPath);
    if(!inputFile) {
        std::cerr << "找不到备份表 ZipFileList.txt" << std::endl;
        inputFile.close();
        std::exit(0);
        return;
    }
    zipAccessor = new ZipAccessor;
    int id = 0;
    File file;
    std::string strPwd;
    while(inputFile >> id) {
        if(id == -1) { // input password
            inputFile >> strPwd;
            for(auto c : strPwd) 
                zipAccessor->password.push_back(c);
            continue;
        }
        inputFile >> file.path >> file.name >> file.type >> file.group >> file.owner;
        inputFile >> file.time.rawMyTime >> file.auth >> file.size;
        // inputFile >> file.md5;
        mp[id] = file;
    }
    cnt = id;
    zipAccessor->isempty = (id == 0);
    inputFile.close();
}

// 每次都要刷新信息表
void ZipFileList::update_list()
{
    outputFile.open(listPath, std::ios::trunc);
    if(zipAccessor->password.size() > 0) { // output password
        outputFile << "-1 "; 
        for(auto c : zipAccessor->password) 
            outputFile << c;
        outputFile << std::endl;
    }
    for(auto x : mp) {
        outputFile << x.first << " ";
        File file = x.second;
        outputFile << file.path << " " << file.name << " " << file.type << " " << file.group << " " << file.owner << " ";
        outputFile << file.time.rawMyTime << " " << file.auth << " " << file.size << std::endl;
        /* file.md5 */ 
    }
    outputFile.close();
}

// 退出时保存信息表
void ZipFileList::exit()
{
    update_list();
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
    auto status = zipAccessor->del(info.first, info.first + info.second);
    if(!status) return 0;
    mp.erase(id);
    return 1;
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
    std::cerr << "文件 " << path << " 备份成功" << std::endl;
    update_list();
    return 1;
}

//zip_file的多文件版本
int ZipFileList::zip_file(std::vector<std::string> pathVector)
{
    for (auto p : pathVector) {
        if(!zip_file(p))
            return 0;
    }
    return 1;
}

void ZipFileList::auto_zip_file(int t)
{
    while (true)
    {
        sleep(t);
        for(auto x : mp) {
            zip_file(x.second.path);
        }
    }
}

int ZipFileList::unzip_file(std::string path)
{
    if(zipAccessor->password.size() > 0){
        std::cerr << "备份存在密码，需要先输入密码" << std::endl;
        return 0;
    }
    std::string pwd = {};
    return unzip_file(path, pwd);
}
int ZipFileList::unzip_file(std::string path, std::string pwd)
{
    auto password = str_to_vec(pwd);
    if(password != zipAccessor->password){
        std::cerr << "解压密码错误" << std::endl;
        return 0;
    }
    int id = findFile(path);
    if(!id){
        std::cerr << "备份中找不到该文件" << std::endl;
        return 0;
    }
    auto x = calc_offset(id);
    std::vector<char>str = zipAccessor->query(x.first, x.first + x.second);
    auto s = FileAccessor::writeFileStream(path, str);
    if(!s) return 0;
    std::cerr << "文件 " << path << " 提取成功" << std::endl;
    return 1;
}

int ZipFileList::set_password(std::string newpwd)
{
    if(zipAccessor->password.size() != 0) {
        std::cerr << "存在旧密码，必须先输入旧密码才能重置密码" << std::endl; 
        return 0;
    }
    std::string oldpwd;
    return set_password(newpwd, oldpwd);
}

int ZipFileList::set_password(std::string npwd, std::string opwd)
{
    auto newpwd = str_to_vec(npwd);
    auto oldpwd = str_to_vec(opwd);
    if(zipAccessor->password != oldpwd) {
        std::cerr << "旧密码错误，无法重置密码" << std::endl; 
        return 0;
    }
    zipAccessor->openZip();
    zipAccessor->password = newpwd;
    zipAccessor->closeZip(1);
    std::cerr << "已经重置密码为" << npwd << std::endl; 
    update_list();
    return 1;
}

int ZipFileList::reset_password(std::string opwd)
{
    auto oldpwd = str_to_vec(opwd);
    if(zipAccessor->password != oldpwd) {
        std::cerr << "旧密码错误，无法重置密码" << std::endl; 
        return 0;
    }
    zipAccessor->openZip();
    zipAccessor->password = str_to_vec("");
    zipAccessor->closeZip(1);
    std::cerr << "已经解密" << std::endl; 
    update_list();
    return 1;
}

//网盘初始化（如果没有填写API，那么获得和填写API用于传输/如果填写了API那么显示剩余配额）
void ZipFileList::netDiskInit() 
{
    system("bypy info");
}

//上传本地文件到网盘
void ZipFileList::netDiskUpload() 
{
    const bool verbose = 0;
    if(zipAccessor == NULL){
        std::cerr << "ZipAccessor未成功初始化" << std::endl;
    }
    std::string fileUp("bypy upload "), zipListUp("bypy upload "), confUp("bypy upload ");
    
    fileUp += zipAccessor -> filePath;
    confUp += zipAccessor -> ConfigureFilePath;
    zipListUp += listPath;

    if(!verbose) {
        fileUp += " > /dev/null 2>&1";
        confUp += " > /dev/null 2>&1";
        zipListUp += " > /dev/null 2>&1";
    }

    system(zipListUp.c_str());
    std::cerr << "元数据Configure上传完成" << std::endl;
    system(confUp.c_str());
    std::cerr << "元数据ZipFileList上传完成" << std::endl;
    system(fileUp.c_str());
    std::cerr << "压缩包上传完成" << std::endl;
}

//下载网盘备份到本地
void ZipFileList::netDiskDownload() 
{
    const bool verbose = 1;
    if(zipAccessor == NULL){
        std::cerr << "ZipAccessor未成功初始化" << std::endl;
    }
    std::string fileUp("bypy download "), zipListUp("bypy download "), confUp("bypy download ");

    fileUp += FileAccessor::getFileName(zipAccessor -> filePath);
    confUp += FileAccessor::getFileName(zipAccessor -> ConfigureFilePath);
    zipListUp += FileAccessor::getFileName(listPath);

    fileUp += " ";
    confUp += " ";
    zipListUp += " ";

    fileUp += zipAccessor -> filePath;
    confUp += zipAccessor -> ConfigureFilePath;
    zipListUp += listPath;

    if(!verbose) {
        fileUp += " > /dev/null 2>&1";
        confUp += " > /dev/null 2>&1";
        zipListUp += " > /dev/null 2>&1";
    }

    system(zipListUp.c_str());
    system(confUp.c_str());
    system(fileUp.c_str());
}

#ifdef test
void check_password()
{
    ZipFileList zipFileList;;
    std::cerr<<"check password";
    std::string c="abc",d="abcd";
    if (zipFileList.set_password(c)==0)
    {
        std::cerr<<"check: fail set password";
        return;
    }
    if (zipFileList.set_password(c)==1)
    {
        std::cerr<<"check: double set password";
        return;
    }
    if (zipFileList.set_password(d,d)==1)
    {
        std::cerr<<"check: wrong old password";
        return;
    }
    if (zipFileList.set_password(d,c)==1)
    {
        std::cerr<<"check: fail reset password";
        return;
    }
    if (zipFileList.set_password(c)==1)
    {
        std::cerr<<"check: wrong old password";
        return;
    }
    if (zipFileList.set_password(d)==0)
    {
        std::cerr<<"check: fail reset password";
        return;
    }
    std::cerr<<"check: password OK";
}
int main()
{
    return 0;
}
#endif

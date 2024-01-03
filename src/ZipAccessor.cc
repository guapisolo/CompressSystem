#ifdef test
#include "../include/ZipAccessor.h"
#else
#include "ZipAccessor.h"
#endif


// 现在 decode 和 encode 还没实现，现在只能对原串操作
// 注意 vector<char> 和 string 的区别，文件流里有0，所以我都用了 vector<char>实现。string可能会出锅
int ZipAccessor::openZip()
{
    inputFile.open(filePath, std::ios::binary);
    if(!inputFile) {
        std::cerr << "无法打开文件" << std::endl;
        return 0;
    }
    buffer.assign((std::istreambuf_iterator<char>(inputFile)), 
                   std::istreambuf_iterator<char>());
    inputFile.close();
    // decode(buffer);
    return 1;
}

// clear buffer
// write back or not
void ZipAccessor::closeZip(int writeBack)
{
    if(!writeBack){ 
        buffer.clear(); 
        return;
    }
    outputFile.open(filePath, std::ios::binary | std::ios::trunc);
    if(!outputFile) {
        std::cerr << "无法打开文件" << std::endl;
        return;
    }
    outputFile.write(buffer.data(), buffer.size());
    outputFile.flush();
    outputFile.close();
    buffer.clear(); 
    return;
}

// add a new string at back
int ZipAccessor::add(std::vector<char> &str)
{
    auto status = openZip();
    if(!status) return 0;
    if(str.size() == 0) return 0;
    buffer.insert(buffer.end(), str.begin(), str.end());
    // encode(buffer);
    closeZip(1);
    return 1;
}

// delete a substring
// 左闭右开
int ZipAccessor::del(int L, int R)
{
    auto status = openZip();
    if(!status) return 0;
    if(L > R) return 0;
    buffer.erase(buffer.begin() + L, buffer.begin() + R);
    // encode(buffer);
    closeZip(1);
    return 1;
}

// query a substring
// 左闭右开
std::vector<char> ZipAccessor::query(int L, int R)
{
    auto status = openZip();
    std::vector<char> ans;
    if(!status) return ans;
    auto st = buffer.begin() + L;
    auto ed = buffer.begin() + R;
    for(auto it = st; it != ed; it++) {
        ans.push_back(*it);
    }
    closeZip(0);
    return ans;
}

#ifdef test
#include <unistd.h>
int main() {
    ZipAccessor tmp;
    tmp.openZip();
    tmp.closeZip(1);
    // while(1){
    //     tmp.closeZip(1);
    //     sleep(10000);
    // }
    return 0;
}
#endif

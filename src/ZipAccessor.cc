#include<random>
#include<queue>
#include<utility>
#define test
#ifdef test
#include "../include/ZipAccessor.h"
#else
#include "ZipAccessor.h"
#endif

void ZipAccessor::dfs(int x)
{
    if (x<256)
    {
        mpToIssac[x]=tmp;
        return;
    }
    tmp.push_back(0);
    dfs(son[x][0]);
    tmp.pop_back();
    tmp.push_back(1);
    dfs(son[x][1]);
    tmp.pop_back();
}
void ZipAccessor::encode()
{
    for (int i=0;i<256;++i)
        freq[i]=0;
    for (auto c:buffer)
        freq[(unsigned)c]++;
    std::priority_queue<std::pair<int,int> > pq;
    for (int i=0;i<256;++i)
        pq.push({-freq[i],i});
    for (int i=256;i<511;++i)
    {
        // auto [v1,u1]=pq.top();
        int v1=pq.top().first,u1=pq.top().second;
        pq.pop();
        int v2=pq.top().first,u2=pq.top().second;
        // auto [v2,u2]=pq.top();
        pq.pop();
        pq.push({v1+v2,i});
        son[i][0]=u1;
        son[i][1]=u2;
    }
    tmp.clear();
    dfs(510);
    zip.clear();
    zipLen=0;
    for (char c:buffer)
        for (int x:mpToIssac[(unsigned)c])
        {
            if (zipLen%8==0)
                zip.push_back(0);
            zip.back()<<=1;
            zip.back()|=x;
        }
    freopen(ConfigureFilePath,"w",stdout);
    printf("%d\n",zipLen);
    for (int i=256;i<511;++i)
        printf("%d %d\n",son[i][0],son[i][1]);
}
int ZipAccessor::decode()
{
    freopen(ConfigureFilePath,"r",stdin);
    if (scanf("%d",&zipLen)==-1)
        return 0;
    for (int i=256;i<511;++i)
        scanf("%d%d",&son[i][0],&son[i][1]);
    buffer.clear();
    int p=510;
    for (int i=0;i<=zipLen/8;++i)
    {
        for (int j=((i<zipLen/8)?7:zipLen%8-1);j>=0;--j)
        {
            p=son[p][(zip[i]>>j)&1];
            if (p<256)
            {
                buffer.push_back(p);
                p=510;
            }
        }
    }
    return 1;
}
void ZipAccessor::en_decry(std::vector<char> &s)
{
    int mod=998244353,base=233,x=0;
    for (char c:s)
    {
        int p=(unsigned)c;
        x=((long long)x*base+p)%mod;
    }
    std::mt19937 rnd(x);
    for (auto &c: zip)
        c^=(rnd()&255);
}
// 现在 decode 和 encode 还没实现，现在只能对原串操作
// 注意 vector<char> 和 string 的区别，文件流里有0，所以我都用了 vector<char>实现。string可能会出锅
int ZipAccessor::openZip()
{
    inputFile.open(filePath, std::ios::binary);
    if(!inputFile) {
        std::cerr << "无法打开文件" << std::endl;
        return 0;
    }
    zip.assign((std::istreambuf_iterator<char>(inputFile)), 
                   std::istreambuf_iterator<char>());
    if (iscry)
        en_decry(password);
    if (!decode())
    {
        std::cerr << "压缩配置文件丢失" << std::endl;
        return 0;
    }
    inputFile.close();
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
    encode();
    if (iscry)
        en_decry(password);
    outputFile.write(zip.data(), zip.size());
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

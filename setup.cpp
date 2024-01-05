#include "ZipFileList.h"

int main()
{
    // std::vector<std::string> tmp = FileAccessor::selectFiles("/root/CompressSystem/testfile", ".*\\.(txt|pdf|gif)$");
    // std::vector<std::string> tmp = FileAccessor::selectFiles("/home/guapisolo/code/SWExp2/code/testfile", ".*\\.(txt|pdf|gif)$");


    ZipFileList core;
    // core.load();
    // // core.zip_file("/home/guapisolo/code/SWExp2/code/testfile/abaabaaba.txt");
    // // core.zip_file("/home/guapisolo/code/SWExp2/code/testfile/healthylife.gif");
    // // core.zip_file("/home/guapisolo/code/SWExp2/code/testfile/test.pdf");
    // core.zip_file(tmp); //test zip_file (vector)
    // // core.zip_file("/root/CompressSystem/testfile/abaabaaba.txt");
    // // core.zip_file("/root/CompressSystem/testfile/healthylife.gif");
    // // core.zip_file("/root/CompressSystem/testfile/test.pdf");
    // // core.zip_file("/home/guapisolo/code/SWExp2/code/testfile/这就是奎桑提.txt");
    // core.netDiskInit();
    // core.netDiskUpload();
    // core.exit();

    core.load();
    core.unzip_file("/home/guapisolo/code/SWExp2/code/testfile/abaabaaba.txt");
    // core.unzip_file("/home/guapisolo/code/SWExp2/code/testfile/healthylife.gif");
    core.exit();
    return 0;
}

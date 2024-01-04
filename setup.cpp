#include "ZipFileList.h"

int main()
{
    ZipFileList core;
    // core.load();
    // core.zip_file("/home/guapisolo/code/SWExp2/code/testfile/abaabaaba.txt");
    // core.zip_file("/home/guapisolo/code/SWExp2/code/testfile/healthylife.gif");
    // core.zip_file("/home/guapisolo/code/SWExp2/code/testfile/test.pdf");
    // // core.zip_file("/home/guapisolo/code/SWExp2/code/testfile/这就是奎桑提.txt");
    // core.exit();

    core.load();
    core.unzip_file("/home/guapisolo/code/SWExp2/code/testfile/abaabaaba.txt");
    // core.unzip_file("/home/guapisolo/code/SWExp2/code/testfile/healthylife.gif");
    core.exit();
    return 0;
}

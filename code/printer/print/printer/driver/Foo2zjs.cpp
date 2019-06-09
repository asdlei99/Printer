#include <print/Foo2zjs.h>
#include <log/Log.h>
#include <lib/Parse.h>
#include <lib/FileType.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

Foo2zjs::Foo2zjs()
    : PrinterDrv(DrvTypeFoo2zjs)
{
    cmd = new char[200];
    buf = new char[1024];

    foo2zjs = "foo2zjs";
}

Foo2zjs::~Foo2zjs()
{
    if(cmd)
        delete[] cmd;
    cmd = NULL;

    if(buf)
        delete[] buf;
    buf = NULL;
}

bool Foo2zjs::devInit()
{
    /* 判断是否选择了设备 */
    if(devName.empty())
    {
        LOG_ERROR("Please select dev.\n");
        return false;
    }

    /* 找到设备对应的固件，相应的固件在配置文件中描述 */
    std::map<std::string, std::string>::iterator it = Parse::printerFirmMap.find(devName);
    if(it == Parse::printerFirmMap.end())
    {
        LOG_ERROR("Can't spport this dev.\n");
        return false;
    }
    firmware = it->second;

    /* 烧写固件 */
    burnFirmware();

    return true;
}

bool Foo2zjs::printFile(const char *fileName, PrintParm &printParm)
{
    /* 判断文件是否为pbm格式 */
    if(!FileType::isPbmFile(fileName))
    {
        LOG_ERROR("Can't print this format file.\n");
        return false;
    }

    int ret;

    /* 使用foo2zjs驱动打印机打印 */
    sprintf(cmd, "sudo %s -z1 -p9 -n%d -r600x600 %s > %s", foo2zjs, printParm.page, fileName, devPath.c_str());
    LOG_DEBUG("%s\n", cmd);
    ret = system(cmd);
    if(ret != 0)
    {
        LOG_DEBUG("%s error.\n", cmd);
        return false;
    }

    return true;
}

bool Foo2zjs::burnFirmware()
{
    /* 判断是否找到相应的固件 */
    if(firmware.empty())
    {
        LOG_ERROR("No firmware.\n");
        return false;
    }

    /* 判断固件是否存在 */
    sprintf(cmd, "%s%s", Parse::firmwarePath, firmware.c_str());
    if(access(cmd, F_OK) != 0)
    {
        LOG_ERROR("%s no exist.\n", cmd);
        return false;
    }

    int ret;

    /* 烧写固件 */
    sprintf(cmd, "sudo cat %s%s > %s", Parse::firmwarePath, firmware.c_str(), devPath.c_str());
    LOG_DEBUG("%s\n", cmd);
    ret = system(cmd);
    if(ret != 0)
    {
        LOG_DEBUG("%s error.\n", cmd);
        return false;
    }

    return true;
}

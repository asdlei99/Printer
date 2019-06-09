#include <print/ImageDeal.h>
#include <log/Log.h>
#include <lib/Parse.h>
#include <string.h>
#include <stdlib.h>

ImageDeal::ImageDeal()
{
    cmd = new char[200];
    buf1 = new char[200];
    buf2 = new char[200];

    bmp2pnm = "bmptopnm";
    png2pnm = "pngtopnm";
    jpeg2pnm = "jpegtopnm";
    pnm2ps = "pnmtops";
    gs = "gs";
}

ImageDeal::~ImageDeal()
{
    if(cmd)
        delete[] cmd;
    cmd = NULL;

    if(buf1)
        delete[] buf1;
    buf1 = NULL;

    if(buf2)
        delete[] buf2;
    buf2 = NULL;
}

bool ImageDeal::deal2Pbm(const char *inputFile, const char *outputFile)
{
    if(!inputFile || !outputFile)
    {
        LOG_ERROR("File name error.\n");
        return false;
    }

    char *pData = NULL;

    /* 指向输出文件的后缀名 */
    pData = strrchr((char *)outputFile, '.');
    if(!pData)
    {
        LOG_ERROR("Output format error.\n");
        return false;
    }
    
    /* 判断输出文件的格式 */
    if(strcmp(pData, ".pbm"))
    {
        LOG_ERROR("Can't support this output format.\n");
        return false;
    }

    /* 指向输入文件的后缀名 */
    pData = strrchr((char *)inputFile, '.');
    if(!pData)
    {
        LOG_ERROR("Input format error.\n");
        return false;
    }

    strcpy(buf1, inputFile);
    strcpy(buf2, inputFile);

    /* 判断输入文件的格式，使用相应的工具装换格式 */
    if(!strcmp(pData, ".bmp"))
    {
        sprintf(cmd, "sudo %s %s %c %s", bmp2pnm, inputFile, '>', FileType::modifyName2Pnm(buf1));
    }
    else if(!strcmp(pData, ".png"))
    {
        sprintf(cmd, "sudo %s %s %c %s", png2pnm, inputFile, '>', FileType::modifyName2Pnm(buf1));
    }
    else if(!strcmp(pData, ".jpg") || !strcmp(pData, ".jpeg"))
    {
        sprintf(cmd, "sudo %s %s %c %s", jpeg2pnm, inputFile, '>', FileType::modifyName2Pnm(buf1));
    }
    else
    {
        LOG_ERROR("Can't support this input format.\n");
        return false;
    }
    
    int ret;

    LOG_DEBUG("%s\n", cmd);
    ret = system(cmd); // 发起一个系统调用，将图片转换为pnm格式
    if(ret != 0)
    {
        LOG_ERROR("%s error.\n", cmd);
        return false;
    }

    sprintf(cmd, "sudo %s %s %c %s", pnm2ps, FileType::modifyName2Pnm(buf1), '>', FileType::modifyName2Ps(buf2));
    LOG_DEBUG("%s\n", cmd);
    ret = system(cmd); // 将pnm格式转换为ps格式
    if(ret != 0)
    {
        LOG_ERROR("%s error.\n", cmd);
        return false;
    }

    sprintf(cmd, "sudo %s -dBATCH -dSAFER -dQUIET -dNOPAUSE -sPAPERSIZE=a4 -r600x600 -sDEVICE=pbmraw -sOutputFile="
            "%s %s", gs, outputFile, FileType::modifyName2Ps(buf1));
    LOG_DEBUG("%s\n", cmd);
    ret = system(cmd); // 将ps格式转换为pbm格式
    if(ret != 0)
    {
        LOG_ERROR("%s error.\n", cmd);
        return false;
    }

    /* 删除中间文件 */
    sprintf(cmd, "sudo rm -f %s", FileType::modifyName2Ps(buf1));
    LOG_DEBUG("%s\n", cmd);
    ret = system(cmd); // 删除.ps文件
    if(ret != 0)
    {
        LOG_ERROR("%s error.\n", cmd);
        return false;
    }

    sprintf(cmd, "sudo rm -f %s", FileType::modifyName2Pnm(buf1));
    LOG_DEBUG("%s\n", cmd);
    ret = system(cmd); // 删除.pnm文件
    if(ret != 0)
    {
        LOG_ERROR("%s error.\n", cmd);
        return false;
    }

    return true;
}
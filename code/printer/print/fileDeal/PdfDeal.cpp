#include <print/PdfDeal.h>
#include <lib/Parse.h>
#include <stdio.h>
#include <string.h>
#include <log/Log.h>
#include <stdlib.h>

PdfDeal::PdfDeal()
{
    cmd = new char[200];
    buf = new char[200];

    pdf2ps = "pdf2ps";
    gs = "gs";
}

PdfDeal::~PdfDeal()
{
    if(cmd)
        delete[] cmd;
    cmd = NULL;

    if(buf)
        delete[] buf;
    buf = NULL;
}

bool PdfDeal::deal2Pbm(const char *inputFile, const char *outputFile)
{
    if(!inputFile || !outputFile)
    {
        LOG_ERROR("File name error.\n");
        return false;
    }

    char *pData = NULL;

    pData = strrchr((char *)outputFile, '.');
    if(!pData)
    {
        LOG_ERROR("Output format error.\n");
        return false;
    }
    
    if(strcmp(pData, ".pbm"))
    {
        LOG_ERROR("Can't support this output format.\n");
        return false;
    }

    pData = strrchr((char *)inputFile, '.');
    if(!pData)
    {
        LOG_ERROR("Input format error.\n");
        return false;
    }

    if(strcmp(pData, ".pdf"))
    {
        LOG_ERROR("Can't support this input format.\n");
        return false;
    }

    int ret;

    strcpy(buf, inputFile);
    sprintf(cmd, "sudo %s %s %s", pdf2ps, inputFile, FileType::modifyName2Ps(buf));
    LOG_DEBUG("%s\n", cmd);
    ret = system(cmd);
    if(ret != 0)
    {
        LOG_ERROR("%s error.\n", cmd);
        return false;
    }

    sprintf(cmd, "sudo %s -dBATCH -dSAFER -dQUIET -dNOPAUSE -sPAPERSIZE=a4 -r600x600 -sDEVICE=pbmraw -sOutputFile="
            "%s %s", gs, outputFile, buf);
    LOG_DEBUG("%s\n", cmd);
    ret = system(cmd); // 转换为pbm格式
    if(ret != 0)
    {
        LOG_ERROR("%s error.\n", cmd);
        return false;
    }

    sprintf(cmd, "sudo rm -f %s", buf);
    LOG_DEBUG("%s\n", cmd);
    ret = system(cmd); // 删除.ps文件
    if(ret != 0)
    {
        LOG_ERROR("%s error.\n", cmd);
        return false;
    }

    return true;
}

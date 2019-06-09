#include <string.h>
#include <log/Log.h>
#include <lib/FileType.h>

bool FileType::isPdf(char *fileName)
{
    if(!fileName)
        return NULL;

    char *pTmp;

    pTmp = strrchr(fileName, '.');
    if(!pTmp)
    {
        LOG_ERROR("File name format error.\n");
        return NULL;
    }

    if(!strcmp(pTmp, ".pdf"))
        return true;
    else
        return false;
}

bool FileType::isBmp(char *fileName)
{
    if(!fileName)
        return NULL;

    char *pTmp;

    pTmp = strrchr(fileName, '.');
    if(!pTmp)
    {
        LOG_ERROR("File name format error.\n");
        return NULL;
    }

    if(!strcmp(pTmp, ".bmp"))
        return true;
    else
        return false;
}

bool FileType::isPng(char *fileName)
{
    if(!fileName)
        return NULL;

    char *pTmp;

    pTmp = strrchr(fileName, '.');
    if(!pTmp)
    {
        LOG_ERROR("File name format error.\n");
        return NULL;
    }

    if(!strcmp(pTmp, ".png"))
        return true;
    else
        return false;
}

bool FileType::isJpg(char *fileName)
{
    if(!fileName)
        return NULL;

    char *pTmp;

    pTmp = strrchr(fileName, '.');
    if(!pTmp)
    {
        LOG_ERROR("File name format error.\n");
        return NULL;
    }

    if(!strcmp(pTmp, ".jpg") || !strcmp(pTmp, ".jpeg"))
        return true;
    else
        return false;
}

bool FileType::isPbmFile(const char *fileName)
{
    char *p1 = NULL;

    p1 = strrchr((char *)fileName, '.');
    if(!p1)
    {
        LOG_ERROR("File name error.\n");
        return false;
    }

    if(strcmp(p1, ".pbm"))
        return false;
    else
        return true;    
}

char *FileType::modifyName2Ps(char *fileName)
{
    if(!fileName)
        return NULL;

    char *pTmp;

    pTmp = strrchr(fileName, '.');
    if(!pTmp)
    {
        LOG_ERROR("File name format error.\n");
        return NULL;
    }

    strcpy(pTmp, ".ps");

    return fileName;
}

char *FileType::modifyName2Pnm(char *fileName)
{
    if(!fileName)
        return NULL;

    char *pTmp;

    pTmp = strrchr(fileName, '.');
    if(!pTmp)
    {
        LOG_ERROR("File name format error.\n");
        return NULL;
    }

    strcpy(pTmp, ".pnm");

    return fileName;
}

char *FileType::modifyName2Pbm(char *fileName)
{
    if(!fileName)
        return NULL;

    char *pTmp;

    pTmp = strrchr(fileName, '.');
    if(!pTmp)
    {
        LOG_ERROR("File name format error.\n");
        return NULL;
    }

    strcpy(pTmp, ".pbm");

    return fileName;
}

#ifndef _FILETYPE_H_
#define _FILETYPE_H_

class FileType
{
public:

    static bool isPdf(char *fileName);

    static bool isBmp(char *fileName);

    static bool isPng(char *fileName);

    static bool isJpg(char *fileName);

    static bool isPbmFile(const char *fileName);

    static char *modifyName2Ps(char *fileName);

    static char *modifyName2Pnm(char *fileName);

    static char *modifyName2Pbm(char *fileName);

private:

    FileType();

};

#endif //_FILETYPE_H_
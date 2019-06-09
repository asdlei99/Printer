#ifndef _IMAGEDEAL_H_
#define _IMAGEDEAL_H_
#include <print/FileDeal.h>
#include <lib/FileType.h>

class ImageDeal : public FileDeal
{
public:

    ImageDeal();

    ~ImageDeal();

    /*
     * 函数名称： deal2Pbm
     * 功能描述： 将图片转换为pbm格式
     * 输入参数： inputFile - 输入文件的绝对路径
     * 输出参数： outputFile - 输出文件的绝对路径
     * 返 回 值： true - 成功；false - 失败
     */
    virtual bool deal2Pbm(const char *inputFile, const char *outputFile);

private:

    char *cmd;

    char *buf1;

    char *buf2;

    const char *bmp2pnm;

    const char *png2pnm;

    const char *jpeg2pnm;

    const char *pnm2ps;

    const char *gs;
};

#endif //_IMAGEDEAL_H_
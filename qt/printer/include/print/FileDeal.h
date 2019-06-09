#ifndef _FILEDEAL_H_
#define _FILEDEAL_H_

class FileDeal
{
public:

    FileDeal();

    virtual ~FileDeal();

    /*
     * 函数名称： deal2Pbm
     * 功能描述： 将相应的文件转换为pbm格式
     * 输入参数： inputFile - 输入文件的绝对路径
     * 输出参数： outputFile - 输出文件的绝对路径
     * 返 回 值： true - 成功；false - 失败
     */
    virtual bool deal2Pbm(const char *inputFile, const char *outputFile) = 0;

};

#endif //_FILEDEAL_H_
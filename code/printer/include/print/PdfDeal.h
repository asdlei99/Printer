#ifndef _PDFDEAL_H_
#define _PDFDEAL_H_
#include <print/FileDeal.h>
#include <lib/FileType.h>

class PdfDeal : public FileDeal
{
public:

    PdfDeal();

    ~PdfDeal();

    virtual bool deal2Pbm(const char *inputFile, const char *outputFile);

private:

    char *cmd;

    char *buf;

    const char *pdf2ps;

    const char *gs;
};

#endif //_PDFDEAL_H_
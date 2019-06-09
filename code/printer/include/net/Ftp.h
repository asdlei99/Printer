#ifndef _FTP_H_
#define _FTP_H_
#include <net/FileTransfer.h>

class Ftp : public FileTransfer
{
public:

    Ftp();

    ~Ftp();

    virtual bool download(const char *url, const char *fileName);

};

#endif //_FTP_H_
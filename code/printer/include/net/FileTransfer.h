#ifndef _FILE_TRANSGER_H_
#define _FILE_TRANSGER_H_

//int ftpGetFile(const char *url, const char *fileName);

class FileTransfer
{
public:
    
    FileTransfer();

    virtual ~FileTransfer();

    virtual bool download(const char *url, const char *filePath) = 0;

};

#endif //_FILE_TRANSGER_H_
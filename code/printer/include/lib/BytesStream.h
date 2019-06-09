#ifndef _BYTESSTREAM_H_
#define _BYTESSTREAM_H_

class BytesStream
{
public:

    static char *getLineFromBuf(char *buf, char *line, int *lineLen);

private:

    BytesStream();

};

#endif //_BYTESSTREAM_H_
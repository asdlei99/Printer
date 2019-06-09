#include <lib/BytesStream.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

/*
 * 参数：
 *      buf：缓存
 *      line：获取的一行数据，错误返回NULL
 * 返回值：
 *      返回下一行起始地址，结束返回NULL
*/
char *BytesStream::getLineFromBuf(char *buf, char *line, int *lineLen)
{
    if(!buf || !line)
        return NULL;

    char *p1, *p2;
    int len;
    
    p1 = p2 = buf;
    while(*p2 != '\0' && *p2 != '\r' && *p2 != '\n')
        p2++;

    len = p2 - p1;
    *lineLen = len;
    if(len <= 0)
        return NULL;

    strncpy(line, p1, len);
    line[len] = '\0';

    if(*p2 == '\r')
        p2++;

    if(*p2 == '\n')
        p2++;

    return p2;
}

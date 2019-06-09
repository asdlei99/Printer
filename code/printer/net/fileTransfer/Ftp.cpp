#include <net/Ftp.h>
#include <log/Log.h>
#include <curl/curl.h>

Ftp::Ftp()
{

}

Ftp::~Ftp()
{

}

struct FtpFile {
    const char *filename;
    FILE *stream;
};

static size_t myFwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct FtpFile *out=(struct FtpFile *)stream;
    if(out && !out->stream)
    {
        out->stream=fopen(out->filename, "wb");
        if(!out->stream)
            return -1;
    }

    return fwrite(buffer, size, nmemb, out->stream);
}

bool Ftp::download(const char *url, const char *fileName)
{
    CURL *curl;
    CURLcode res;
    struct FtpFile ftpfile = {
        fileName, //文件下载本地路径
        NULL
    };

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if(curl) 
    {
        /* 设置url */
        curl_easy_setopt(curl, CURLOPT_URL, url);

        /* 设置写回调函数 */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myFwrite);
    
        /* 设置写回调函数参数 */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

        /* 设置用户名和密码 */
        //curl_easy_setopt(curl, CURLOPT_USERPWD, "uftp:Sjuntao1988");

        /* 打印调试信息 */
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* 开始下载文件 */
        res = curl_easy_perform(curl);

        /* 下载完成清除 */
        curl_easy_cleanup(curl);

        if(CURLE_OK != res) 
        {
            /* 下载失败 */
            LOG_ERROR("Curl told us %d.\n", res);
            if(ftpfile.stream)
                fclose(ftpfile.stream);
            return false;
        }
    }

    if(ftpfile.stream)
        fclose(ftpfile.stream);

    curl_global_cleanup();

    return true;
}
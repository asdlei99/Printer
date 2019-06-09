#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <lib/Parse.h>
#include <log/Log.h>
#include <Config.h>

std::vector<std::string> Parse::printer;
std::map<std::string, std::string> Parse::printerFirmMap;
std::map<std::string, std::string> Parse::printerPpdMap;
char Parse::logPath[100];
char Parse::firmwarePath[100];
char Parse::filePath[100];

bool Parse::parseConfigFile()
{
    FILE *pFile = fopen(CONFIG_FILE, "r");
    if(!pFile)
    {
        fprintf(stderr, "Open %s error.\n", CONFIG_FILE);
        return false;
    }

    char *buf = new char[1024];
    char *p1 = NULL;
    char *p2 = NULL;
    char *p3 = NULL;

    while(fgets(buf, 1024, pFile))
    {
        if(buf[0] == '#' || buf[0] == '\r' || buf[0] == '\n' || buf[0] == '}')
            continue;
        
        if(!strncmp(buf, "LOG_PATH", strlen("LOG_PATH")))
        {
            p1 = strchr(buf, '=');
            p1++;
            
            p2 = strchr(p1, ' ');
            if(!p2)
                p2 = strchr(p1, '\r');
                    if(!p2)
                        p2 = strchr(p1, '\n');


            *p2 = '\0';
            
            strcpy(Parse::logPath, p1);
  
        }
        else if(!strncmp(buf, "FIRMWARE_PATH", strlen("FIRMWARE_PATH")))
        {
            p1 = strchr(buf, '=');
            p1++;

            p2 = strchr(p1, ' ');
            if(!p2)
                p2 = strchr(p1, '\r');
                    if(!p2)
                        p2 = strchr(p1, '\n');

            *p2 = '\0';

            strcpy(firmwarePath, p1);
        }
        else if(!strncmp(buf, "FILE_PATH", strlen("FILE_PATH")))
        {
            p1 = strchr(buf, '=');
            p1++;
        
            p2 = strchr(p1, ' ');
            if(!p2)
                p2 = strchr(p1, '\r');
                    if(!p2)
                        p2 = strchr(p1, '\n');

            *p2 = '\0';
            
            strcpy(filePath, p1);
        }
        else if(!strncmp(buf, "PRINTER_FIRMWARE", strlen("PRINTER_FIRMWARE")))
        {
            while(fgets(buf, 1024, pFile))
            {
                if(buf[0] == '}')
                    break;

                p1 = strchr(buf, '{');
                p1++;
                p2 = strchr(p1, ',');
                *p2 = '\0';

                if(find(printer.begin(), printer.end(), std::string(p1)) == printer.end())
                    printer.push_back(p1);

                p2++;
                p3 = strchr(p2, '}');
                *p3 = '\0';

                printerFirmMap.insert(make_pair(std::string(p1), std::string(p2)));
            }
        }
        else if(!strncmp(buf, "PRINTER_PPD", strlen("PRINTER_PPD")))
        {
            while(fgets(buf, 1024, pFile))
            {
                if(buf[0] == '}')
                    break;

                p1 = strchr(buf, '{');
                p1++;
                p2 = strchr(p1, ',');
                *p2 = '\0';

                if(find(printer.begin(), printer.end(), std::string(p1)) == printer.end())
                    printer.push_back(p1);

                p2++;
                p3 = strchr(p2, '}');
                *p3 = '\0';

                printerPpdMap.insert(make_pair(std::string(p1), std::string(p2)));
            }        
        }
        else
        {
            
        }
    }

    delete[] buf;
    buf = NULL;

    fclose(pFile);    

    return true;
}

#ifndef _PRINTER_DRIVER_H_
#define _PRINTER_DRIVER_H_
#include <print/PrinterDriver.h>
#include <string>
#include <vector>

class PrinterDriver
{
public:

    PrinterDriver();

    virtual ~PrinterDriver();

    bool setPrinterName(std::string printerName);

    bool setPrinterDev(std::string printerDev);

    bool getSupportPrinter(std::vector<std::string> &v);

    virtual bool burnFirmware(char *filePath) = 0;

    virtual bool printFile(char *fileName) = 0;

    virtual bool isPrintComplet() = 0;

protected:

    std::string printerName;

    std::string printerDev;

};

#endif //_PRINTER_DRIVER_H_
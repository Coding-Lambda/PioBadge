#ifndef PioFilesystem_h
#define PioFilesystem_h

#include <Adafruit_TinyUSB.h>
#include <Adafruit_SPIFlash.h>
#include <Adafruit_ImageReader.h>

enum PioBadge_FilesystemType
{
    FILESYS_NONE,
    FILESYS_SD,
    FILESYS_QSPI,
    FILESYS_SD_AND_QSPI
};

class PioBadgeFilesystem
{
    PioBadge_FilesystemType _filesys_type = FILESYS_NONE;
    FatFileSystem *Qspi;

public:
    FatFileSystem *beginQspi();
    bool beginQspiMassStorageDevice();
};

#endif
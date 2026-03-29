#include "PioBadgeFilesystem.h"

#if defined(USE_TINYUSB)
static Adafruit_USBD_MSC usb_msc;
#endif

#define ARCADA_SD_CS -1

#if defined(ARCADA_SD_SPI_PORT)
#if defined(ENABLE_EXTENDED_TRANSFER_CLASS)
SdFatEX Arcada_SD_FileSys(&ARCADA_SD_SPI_PORT);
#else
SdFat Arcada_SD_FileSys(&ARCADA_SD_SPI_PORT);
#endif
#else
#if defined(ENABLE_EXTENDED_TRANSFER_CLASS)
SdFatEX Arcada_SD_FileSys;
#else
SdFat Arcada_SD_FileSys;
#endif
#endif

int32_t qspi_msc_write_cb(uint32_t lba, uint8_t *buffer, uint32_t bufsize);
int32_t qspi_msc_read_cb(uint32_t lba, void *buffer, uint32_t bufsize);
void qspi_msc_flush_cb(void);

int32_t sd_msc_write_cb(uint32_t lba, uint8_t *buffer, uint32_t bufsize);
int32_t sd_msc_read_cb(uint32_t lba, void *buffer, uint32_t bufsize);
void sd_msc_flush_cb(void);

#if defined(PIN_QSPI_SCK)
Adafruit_FlashTransport_QSPI flashTransport(PIN_QSPI_SCK, PIN_QSPI_CS,
                                            PIN_QSPI_IO0, PIN_QSPI_IO1,
                                            PIN_QSPI_IO2, PIN_QSPI_IO3);
#elif defined(ARCADA_SPIFLASH_CS)
Adafruit_FlashTransport_SPI flashTransport(ARCADA_SPIFLASH_CS,
                                           &ARCADA_SPIFLASH_SPI);
#endif

Adafruit_SPIFlash Arcada_QSPI_Flash(&flashTransport);
FatFileSystem Arcada_QSPI_FileSys;

FatFileSystem *PioBadgeFilesystem::beginQspi()
{
    if (Qspi != nullptr)
        return Qspi;

    if (ARCADA_SD_CS >= 0)
    {
        // if (desiredFilesys == ARCADA_FILESYS_SD ||
        //     desiredFilesys == ARCADA_FILESYS_SD_AND_QSPI)
        // {
        //   Serial.println("Trying SD Card filesystem");
        //   if (Arcada_SD_FileSys.begin(ARCADA_SD_CS))
        //   {
        //     Serial.println("SD card found");
        //     SD_imagereader = new Adafruit_ImageReader(Arcada_SD_FileSys);
        //     _filesys_type = ARCADA_FILESYS_SD;
        //   }
        // }

        // if (_filesys_type == desiredFilesys)
        // {
        //   // we wanted SD, and we got it!
        //   return _filesys_type;
        // }
    }

    if (Arcada_QSPI_Flash.begin())
    {
        Serial.println(F("QSPI filesystem found: "));
        Serial.print(F("QSPI flash chip JEDEC ID: 0x"));
        Serial.println(Arcada_QSPI_Flash.getJEDECID(), HEX);

        // First call begin to mount the filesystem.  Check that it returns true
        // to make sure the filesystem was mounted.
        if (!Arcada_QSPI_FileSys.begin(&Arcada_QSPI_Flash))
        {
            Serial.println(F("Failed to mount filesystem!"));
            Serial.println(F("Was CircuitPython loaded on the board first to create the filesystem?"));

            return nullptr;
        }

        if (_filesys_type == FILESYS_SD)
        {
            _filesys_type = FILESYS_SD_AND_QSPI;
        }
        else
        {
            _filesys_type = FILESYS_QSPI;
        }

        Qspi = &Arcada_QSPI_FileSys;

        Serial.println(F("Mounted QSPI filesystem!"));

        return Qspi;
    }

    return nullptr;
}

bool PioBadgeFilesystem::beginQspiMassStorageDevice()
{
#if defined(USE_TINYUSB)
    auto fsHandle = beginQspi();

    if (fsHandle == nullptr)
    {
        return false;
    }

    // arcadaBegin() could take long time to complete
    // By the time this function is called, usb enumeration is probably completed
    // as CDC only device Therefore we have to
    // - Physically detach the device by disable pull-up resistor
    // - Configure the Mass Stroage interface
    // - Re-attach by enable pull-up resistor
    USBDevice.detach();
    delay(50); // a bit of delay for device to disconnect

    //     if (found == FILESYS_SD || found == FILESYS_SD_AND_QSPI)
    //     { // SD first
    //         // Set disk vendor id, product id and revision with string up to 8, 16, 4
    //         // characters respectively
    //         usb_msc.setID("Adafruit", "SD Card", "1.0");

    //         // Set callback
    //         usb_msc.setReadWriteCallback(sd_msc_read_cb, sd_msc_write_cb,
    //                                      sd_msc_flush_cb);
    //         usb_msc.setUnitReady(false);
    //         usb_msc.begin();

    //         uint32_t block_count = Arcada_SD_FileSys.card()->sectorCount();
    // #ifdef ARCADA_MSD_DEBUG
    //         Serial.print("MSD for SD Card - Volume size (MB):  ");
    //         Serial.println((block_count / 2) / 1024);
    // #endif
    //         // Set disk size, SD block size is always 512
    //         usb_msc.setCapacity(block_count, 512);

    //         // MSC is ready for read/write
    //         usb_msc.setUnitReady(true);

    //         // re-attach to usb bus
    //         USBDevice.attach();

    //         return true;
    //     }

#ifdef ARCADA_MSD_DEBUG
    Serial.println("Found QSPI for MSD");
    Serial.print("JEDEC ID: ");
    Serial.println(Arcada_QSPI_Flash.getJEDECID(), HEX);
    Serial.print("Flash size: ");
    Serial.println(Arcada_QSPI_Flash.size());
#endif

    // Set disk vendor id, product id and revision with string up to 8, 16, 4
    // characters respectively
    usb_msc.setID("Adafruit", "SPI Flash", "1.0");

    // Set callback
    usb_msc.setReadWriteCallback(qspi_msc_read_cb, qspi_msc_write_cb,
                                 qspi_msc_flush_cb);

    // Set disk size, block size should be 512 regardless of spi flash page size
    usb_msc.setCapacity(
        Arcada_QSPI_Flash.pageSize() * Arcada_QSPI_Flash.numPages() / 512, 512);

    // MSC is ready for read/write
    usb_msc.setUnitReady(true);
    usb_msc.begin();

    // re-attach to usb bus
    USBDevice.attach();

    return true;
#endif

    return false;
}

static uint32_t last_access_ms;

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and
// return number of copied bytes (must be multiple of block size)
int32_t qspi_msc_read_cb(uint32_t lba, void *buffer, uint32_t bufsize)
{
#ifdef ARCADA_MSD_DEBUG
    Serial.printf("QSPI Read block %08x\n", lba);
#endif
    // Note: SPIFLash Bock API: readBlocks/writeBlocks/syncBlocks
    // already include 4K sector caching internally. We don't need to cache it,
    // yahhhh!!
    return Arcada_QSPI_Flash.readBlocks(lba, (uint8_t *)buffer, bufsize / 512)
               ? bufsize
               : -1;
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and
// return number of written bytes (must be multiple of block size)
int32_t qspi_msc_write_cb(uint32_t lba, uint8_t *buffer, uint32_t bufsize)
{
#ifdef ARCADA_MSD_DEBUG
    Serial.printf("QSPI Write block %08x\n", lba);
#endif
    // Note: SPIFLash Bock API: readBlocks/writeBlocks/syncBlocks
    // already include 4K sector caching internally. We don't need to cache it,
    // yahhhh!!
    return Arcada_QSPI_Flash.writeBlocks(lba, buffer, bufsize / 512) ? bufsize
                                                                     : -1;
}

// Callback invoked when WRITE10 command is completed (status received and
// accepted by host). used to flush any pending cache.
void qspi_msc_flush_cb(void)
{
#ifdef ARCADA_MSD_DEBUG
    Serial.printf("QSPI Flush block\n");
#endif

    // sync with flash
    Arcada_QSPI_Flash.syncBlocks();

    // clear file system's cache to force refresh
    Arcada_QSPI_FileSys.cacheClear();

    last_access_ms = millis();
}

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and
// return number of copied bytes (must be multiple of block size)
int32_t sd_msc_read_cb(uint32_t lba, void *buffer, uint32_t bufsize)
{
#ifdef ARCADA_MSD_DEBUG
    Serial.printf("SD Read block %08x\n", lba);
#endif

    return Arcada_SD_FileSys.card()->readBlocks(lba, (uint8_t *)buffer,
                                                bufsize / 512)
               ? bufsize
               : -1;
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and
// return number of written bytes (must be multiple of block size)
int32_t sd_msc_write_cb(uint32_t lba, uint8_t *buffer, uint32_t bufsize)
{
#ifdef ARCADA_MSD_DEBUG
    Serial.printf("SD Write block %08x\n", lba);
#endif

    return Arcada_SD_FileSys.card()->writeBlocks(lba, buffer, bufsize / 512)
               ? bufsize
               : -1;
}

// Callback invoked when WRITE10 command is completed (status received and
// accepted by host). used to flush any pending cache.
void sd_msc_flush_cb(void)
{
#ifdef ARCADA_MSD_DEBUG
    Serial.printf("SD Flush block\n");
#endif

    Arcada_SD_FileSys.card()->syncBlocks();

    // clear file system's cache to force refresh
    Arcada_SD_FileSys.cacheClear();

    last_access_ms = millis();
}
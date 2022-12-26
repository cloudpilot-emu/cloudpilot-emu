/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h" /* Declarations of disk functions */

#include "CardVolume.h"
#include "ff.h" /* Obtains integer types */

namespace {
    CardVolume *volumes[FF_VOLUMES];

    class Initializer {
       public:
        Initializer() {
            for (int i = 0; i < FF_VOLUMES; i++) volumes[i] = nullptr;
        }
    };

    Initializer initializer;
}  // namespace

void register_card_volume(unsigned int index, void *cardVolume) {
    if (index < FF_VOLUMES) volumes[index] = reinterpret_cast<CardVolume *>(cardVolume);
}

void unregister_card_volume(unsigned int index) {
    if (index < FF_VOLUMES) volumes[index] = nullptr;
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv /* Physical drive nmuber to identify the drive */
) {
    if (pdrv >= FF_VOLUMES) return STA_NOINIT;

    return volumes[pdrv] ? 0 : STA_NODISK;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv /* Physical drive nmuber to identify the drive */
) {
    return disk_status(pdrv) == 0 ? 0 : STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv,    /* Physical drive nmuber to identify the drive */
                  BYTE *buff,   /* Data buffer to store read data */
                  LBA_t sector, /* Start sector in LBA */
                  UINT count    /* Number of sectors to read */
) {
    if (disk_status(pdrv) != 0) return RES_NOTRDY;
    CardVolume *volume = volumes[pdrv];

    if ((sector + count) * 512 > volume->GetSize()) return RES_PARERR;

    return volume->Read(sector * 512, count * 512, buff) ? RES_OK : RES_ERROR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(BYTE pdrv,        /* Physical drive nmuber to identify the drive */
                   const BYTE *buff, /* Data to be written */
                   LBA_t sector,     /* Start sector in LBA */
                   UINT count        /* Number of sectors to write */
) {
    if (disk_status(pdrv) != 0) return RES_NOTRDY;
    CardVolume *volume = volumes[pdrv];

    if ((sector + count) * 512 > volume->GetSize()) return RES_PARERR;

    return volume->Write(sector * 512, count * 512, buff) ? RES_OK : RES_ERROR;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, /* Physical drive nmuber (0..) */
                   BYTE cmd,  /* Control code */
                   void *buff /* Buffer to send/receive control data */
) {
    if (disk_status(pdrv) != 0) return RES_NOTRDY;

    return RES_OK;
}

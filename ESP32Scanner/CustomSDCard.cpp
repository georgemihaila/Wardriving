#include "CustomSDCard.h"

#include <SPI.h>
#include <SD.h>
#include <SD_MMC.h>

#define HSPI_MISO   27
#define HSPI_MOSI   26
#define HSPI_SCLK   25
#define HSPI_SS     33
SPIClass *sdspi = NULL;

bool CustomSDCard::init(){
    sdspi = new SPIClass(HSPI);
    sdspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS);
    return SD.begin(HSPI_SS, *sdspi);
}

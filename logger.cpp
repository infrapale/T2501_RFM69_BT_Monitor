/*
  SD card datalogger

  This example shows how to log data from three analog sensors
  to an SD card using the SD library. Pin numbers reflect the default
  SPI pins for Uno and Nano models

  The circuit:
   analog sensors on analog pins 0, 1, and 2
   SD card attached to SPI bus as follows:
 ** SDO - pin 11
 ** SDI - pin 12
 ** CLK - pin 13
 ** CS - depends on your SD card shield or module.
 		Pin 10 used here for consistency with other Arduino examples
    (for MKR Zero SD: SDCARD_SS_PIN)

  created  24 Nov 2010
  modified  24 July 2020
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>
#include "main.h"
#include "io.h"
#include "atask.h"
#include "logger.h"
#include "rtc_time.h"
#include "watchdog.h"

const int chipSelect = LOGGER_SD_CS;
logger_st  logger;

void logger_task();


Sd2Card card;
SdVolume volume;
SdFile root;


atask_st logger_handle                 = {"Data Logger    ", 5000,0, 0, 255, 0, 1, logger_task};


void logger_initialize(void) 
{
  Serial.print(F("Initializing SD card..."));

  if (!SD.begin(chipSelect)) {
    Serial.println(F("initialization failed. Things to check:"));
    Serial.println(F("1. is a card inserted?"));
    Serial.println(F("2. is your wiring correct?"));
    Serial.println(F("3. did you change the chipSelect pin to match your shield or module?"));
    Serial.println(F("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!"));
    logger.sd_ok = false;
    logger_handle.state = 0;
    delay(1000);
  }
  else
  {
      logger.sd_ok = true;
      logger.new_msg_in_buff = false;
      atask_add_new(&logger_handle);
      Serial.println(F("initialization done."));
      logger_handle.state = 10;
  }
}

void logger_task() {
  // make a string for assembling the data to log:
  // String dataString = "";
  char line_buff[80];

  Watchdog.reset();
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  if (logger.new_msg_in_buff && logger.sd_ok )
  {
      // strncpy(logger.file_name,"log-",4);
      rtc_time_get_date_str(&logger.file_name[0]);
      strcat(logger.file_name,".txt");
      //strncpy(logger.file_name,"L_250210.txt",12);
      File dataFile = SD.open(logger.file_name, FILE_WRITE);

      // if the file is available, write to it:
      if (dataFile) 
      {
        rtc_time_get_stamp(line_buff);
        dataFile.print(line_buff);
        dataFile.print(";");
        dataFile.println(logger.msg_buff);
        logger.new_msg_in_buff = false;

        dataFile.close();
        // print to the serial port too:
        Serial.println(line_buff);
      }
      // if the file isn't open, pop up an error:
      else {
        Serial.print("error opening: ");
        Serial.println(logger.file_name);
      }
  }
}

void logger_add_msg(char *new_msg)
{
    strncpy(logger.msg_buff, new_msg, LOGGER_DATA_LEN);
    logger.new_msg_in_buff = true;
}

void logger_directory(void)
{

// change this to match your SD shield or module;
// Default SPI on Uno and Nano: pin 10
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// MKRZero SD: SDCARD_SS_PIN


if(logger.sd_ok)
{


      // Open serial communications and wait for port to open:

      Serial.print("\nInitializing SD card...");
      // we'll use the initialization code from the utility libraries
      // since we're just testing if the card is working!
      if (!card.init(SPI_HALF_SPEED, chipSelect)) {
        Serial.println("initialization failed. Things to check:");
        Serial.println("* is a card inserted?");
        Serial.println("* is your wiring correct?");
        Serial.println("* did you change the chipSelect pin to match your shield or module?");
        while (1);

      } else {

        Serial.println("Wiring is correct and a card is present.");

      }

      // print the type of card

      Serial.println();
      Serial.print("Card type:         ");
      switch (card.type()) {
        case SD_CARD_TYPE_SD1:
          Serial.println("SD1");
          break;
        case SD_CARD_TYPE_SD2:
          Serial.println("SD2");
          break;
        case SD_CARD_TYPE_SDHC:
          Serial.println("SDHC");
          break;
        default:
          Serial.println("Unknown");
      }

      // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32

      if (!volume.init(card)) {
        Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
        while (1);
      }

      Serial.print("Clusters:          ");
      Serial.println(volume.clusterCount());
      Serial.print("Blocks x Cluster:  ");
      Serial.println(volume.blocksPerCluster());
      Serial.print("Total Blocks:      ");
      Serial.println(volume.blocksPerCluster() * volume.clusterCount());
      Serial.println();
      // print the type and size of the first FAT-type volume
      uint32_t volumesize;
      Serial.print("Volume type is:    FAT");
      Serial.println(volume.fatType(), DEC);
      volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
      volumesize *= volume.clusterCount();       // we'll have a lot of clusters
      volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
      Serial.print("Volume size (Kb):  ");
      Serial.println(volumesize);
      Serial.print("Volume size (Mb):  ");
      volumesize /= 1024;
      Serial.println(volumesize);
      Serial.print("Volume size (Gb):  ");
      Serial.println((float)volumesize / 1024.0);
      Serial.println("\nFiles found on the card (name, date and size in bytes): ");
      root.openRoot(volume);

      // list all files in the card with date and size

      root.ls(LS_R | LS_DATE | LS_SIZE);
      root.close();
  }
}

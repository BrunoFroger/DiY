// chauffage
// sdcard.cpp
// (c) B. Froger 2020


#include <Arduino.h>
#include <FS.h>
#include <SPI.h>
#include <SD.h>


#define SD_CS    14

char sdcardDataFilename[50];
File root;
File sdcardDataFile;
char sdcardValue[50];

//=================================================
//
//      printDirectory
//
//=================================================
void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

//=================================================
//
//      sdcardListFiles
//
//=================================================
void sdcardListFiles(void){
    root = SD.open("/");
    Serial.println("liste des fichiers sur la carte SD");
    printDirectory(root, 0);
}

//=================================================
//
//      getSdcardInfo
//
//=================================================
char *getSdcardInfo(char *valueName){
    char bufferPrint[200];
    char buffer[200];

    if(!SD.begin(SD_CS)){
        Serial.println("Card Mount Failed");
        return nullptr;
    }

    if (!SD.exists(sdcardDataFilename)){
        // le fichier n'existe pas, on quitte
        Serial.print("le fichier ");
        Serial.println(sdcardDataFilename);
        Serial.println(" n'existe pas");
        return nullptr;
    }

    sdcardDataFile = SD.open(sdcardDataFilename,FILE_READ);

    if (!sdcardDataFile){
        // impossible d'ouvrir le fichier
        Serial.print("Impossible d'ouvrir le fichier ");
        Serial.println(sdcardDataFilename);
        Serial.println();
        return nullptr;
    }

    while (sdcardDataFile.available()){
        String tmp;
        tmp = sdcardDataFile.readStringUntil('\n');
        //Serial.print("ligne du fichier = <");
        //Serial.print(tmp);
        //Serial.print(">");
        //Serial.println();
        tmp.trim();
        //Serial.print("ligne filtree = <");
        //Serial.print(tmp);
        //Serial.print(">");
        //Serial.println();
        if (tmp.length() == 0) continue;
        if (tmp.substring(0,1) == "#") continue;
        int index = tmp.indexOf('=');
        if (index == -1) continue;
        String tmpName = tmp.substring(0,index);
        String tmpValue = tmp.substring(index+1,tmp.length());
        //Serial.print("ligne OK = <");
        //Serial.print(tmpName);
        //Serial.print(">=<");
        //Serial.print(tmpValue);
        //Serial.print(">");
        if (tmpName == String(valueName)){
            //Serial.print("on a trouve la chaine a traiter : ");
            //Serial.println(tmpValue);
            strcpy(sdcardValue, tmpValue.c_str());
            sdcardDataFile.close();
            sprintf(bufferPrint, "lecture donnee sur carte SD : %s = %s", valueName, sdcardValue); Serial.println(bufferPrint);
            return sdcardValue;
        }
    }

    sprintf(bufferPrint, "la variable <%s> n'a pas ete trouvee dans le fichier", valueName); Serial.println(bufferPrint);
    sdcardDataFile.close();
    return nullptr;
}

//=================================================
//
//      initSdcard
//
//=================================================
void initSdcard(void){
    Serial.println("+----------------------------------------+");
    Serial.println("|                                        |");
    Serial.println("|           Init SD Card                 |");
    Serial.println("|                                        |");
    Serial.println("+----------------------------------------+");
    if(!SD.begin(SD_CS)){
        Serial.println("Card Mount Failed");
    } else {
        Serial.println("SD Card mounted with success");
    }
    strcpy(sdcardDataFilename, "/data_regulation.txt");
    //sdcardListFiles();
    //getSdcardInfo("consigne");
    //getSdcardInfo("ville");
    //getSdcardInfo("test");
}


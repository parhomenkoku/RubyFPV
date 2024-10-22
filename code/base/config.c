/*
    Ruby Licence
    Copyright (c) 2024 Petru Soroaga
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Copyright info and developer info must be preserved as is in the user
        interface, additions could be made to that info.
       * Neither the name of the organization nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.
        * Military use is not permited.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Julien Verneuil BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "base.h"
#include "config.h"
#include "hardware.h"
#include "hw_procs.h"
#include "../radio/radiopackets2.h"
#include <ctype.h>


u32 channels433[] = {425000, 426000, 427000, 428000, 429000, 430000, 431000, 432000, 433000, 434000, 435000, 435000, 437000, 438000, 439000, 440000, 441000, 442000, 443000, 444000, 445000};
u32 channels868[] = {858000, 859000, 860000, 861000, 862000, 863000, 864000, 865000, 866000, 867000, 868000, 869000, 870000, 871000, 872000, 873000, 874000, 875000};
u32 channels915[] = {910000,911000,912000,913000,914000,915000,916000,917000,918000,919000,920000};

u32 channels23[] = { 2312000, 2317000, 2322000, 2327000, 2332000, 2337000, 2342000, 2347000, 2352000, 2357000, 2362000, 2367000, 2372000, 2377000, 2382000, 2387000, 2392000, 2397000, 2402000, 2407000 };
u32 channels24[] = { 2412000, 2417000, 2422000, 2427000, 2432000, 2437000, 2442000, 2447000, 2452000, 2457000, 2462000, 2467000, 2472000, 2484000 };
u32 channels25[] = { 2487000, 2489000, 2492000, 2494000, 2497000, 2499000, 2512000, 2532000, 2572000, 2592000, 2612000, 2632000, 2652000, 2672000, 2692000, 2712000 };
u32 channels58[] = { 5180000, 5200000, 5220000, 5240000, 5260000, 5280000, 5300000, 5320000, 5500000, 5520000, 5540000, 5560000, 5580000, 5600000, 5620000, 5640000, 5660000, 5680000, 5700000, 5745000, 5765000, 5785000, 5805000, 5825000, 5845000, 5865000, 5885000 };

// in 1 Mb increments, in bps
int s_WiFidataRates[] = {2000000, 6000000, 9000000, 12000000, 18000000, 24000000, 36000000, 48000000, 54000000};
int s_SiKAirDataRates[] = {2000, 4000, 8000, 16000, 19000, 24000, 32000, 48000, 64000, 96000, 128000, 192000, 250000};

u32* getChannels433() { return channels433; }
int getChannels433Count() { return sizeof(channels433)/sizeof(channels433[0]); }
u32* getChannels868() { return channels868; }
int getChannels868Count() { return sizeof(channels868)/sizeof(channels868[0]); }
u32* getChannels915() { return channels915; }
int getChannels915Count() { return sizeof(channels915)/sizeof(channels915[0]); }

u32* getChannels24() { return channels24; }
int getChannels24Count() { return sizeof(channels24)/sizeof(channels24[0]); }
u32* getChannels23() { return channels23; }
int getChannels23Count() { return sizeof(channels23)/sizeof(channels23[0]); }
u32* getChannels25() { return channels25; }
int getChannels25Count() { return sizeof(channels25)/sizeof(channels25[0]); }
u32* getChannels58() { return channels58; }
int getChannels58Count() { return sizeof(channels58)/sizeof(channels58[0]); }

int _getChannelsAndCount(u32 nBand, u32** channels)
{
   if( channels == NULL )
      return -1;

   switch ( nBand ) {
   case RADIO_HW_SUPPORTED_BAND_433:
      *channels = getChannels433();
      return getChannels433Count();

   case RADIO_HW_SUPPORTED_BAND_868:
      *channels = getChannels868();
      return getChannels868Count();

   case RADIO_HW_SUPPORTED_BAND_915:
      *channels = getChannels915();
      return getChannels915Count();

   case RADIO_HW_SUPPORTED_BAND_23:
      *channels = getChannels23();
      return getChannels23Count();

   case RADIO_HW_SUPPORTED_BAND_24:
      *channels = getChannels24();
      return getChannels24Count();

   case RADIO_HW_SUPPORTED_BAND_25:
      *channels = getChannels25();
      return getChannels25Count();

   case RADIO_HW_SUPPORTED_BAND_58:
      *channels = getChannels58();
      return getChannels58Count();

   default:
      break;
  }

  *channels = NULL;
  return -1;
}


int* getSiKAirDataRates()
{
   return s_SiKAirDataRates;
}

int getSiKAirDataRatesCount()
{
   return sizeof(s_SiKAirDataRates)/sizeof(s_SiKAirDataRates[0]);
}


int getBand(u32 freqKhz)
{
   if ( freqKhz < 10000 ) // Old format
   {
      if ( freqKhz < 2700 )
         return RADIO_HW_SUPPORTED_BAND_24;
      else
         return RADIO_HW_SUPPORTED_BAND_58;
   }
   if ( freqKhz < 500000 )
      return RADIO_HW_SUPPORTED_BAND_433;
   if ( freqKhz < 900000 )
      return RADIO_HW_SUPPORTED_BAND_868;
   if ( freqKhz < 950000 )
      return RADIO_HW_SUPPORTED_BAND_915;
   if ( freqKhz < 2412000 )
      return RADIO_HW_SUPPORTED_BAND_23;
   if ( freqKhz < 2487000 )
      return RADIO_HW_SUPPORTED_BAND_24;
   if ( freqKhz < 5000000 )
      return RADIO_HW_SUPPORTED_BAND_25;
   if ( freqKhz > 5000000 )
      return RADIO_HW_SUPPORTED_BAND_58;

   return 0;
}

int getChannelIndexForFrequency(u32 nBand, u32 freqKhz)
{
   u32* channels = NULL;
   int channelcount = _getChannelsAndCount(nBand, &channels);
   if( channels != NULL ) {
      for( int i=0; i<channelcount; i++ )
         if ( freqKhz == channels[i] )
            return i;
   }
   return -1;
}

int isFrequencyInBands(u32 freqKhz, u8 bands)
{
   if ( freqKhz < 500000 )
   {
      if ( bands & RADIO_HW_SUPPORTED_BAND_433 )
         return 1;
      return 0;
   }
   if ( freqKhz < 890000 )
   {
      if ( bands & RADIO_HW_SUPPORTED_BAND_868 )
         return 1;
      return 0;
   }
   if ( freqKhz < 950000 )
   {
      if ( bands & RADIO_HW_SUPPORTED_BAND_915 )
         return 1;
      return 0;
   }
   if ( freqKhz < 2412000 )
   {
      if ( bands & RADIO_HW_SUPPORTED_BAND_23 )
         return 1;
      return 0;
   }
   if ( freqKhz < 2487000 )
   {
      if ( bands & RADIO_HW_SUPPORTED_BAND_24 )
         return 1;
      return 0;
   }
   if ( freqKhz < 5000000 )
   {
      if ( bands & RADIO_HW_SUPPORTED_BAND_25 )
         return 1;
      return 0;
   }
   if ( freqKhz > 5000000 )
   {
      if ( bands & RADIO_HW_SUPPORTED_BAND_58 )
         return 1;
      return 0;
   }
   return 0;
}

int getSupportedChannels(u32 supportedBands, int includeSeparator, u32* pOutChannels, int maxChannels)
{
   if ( NULL == pOutChannels || 0 == maxChannels )
      return 0;

   int radio_hw_supported_bands[] = {
      RADIO_HW_SUPPORTED_BAND_433,
      RADIO_HW_SUPPORTED_BAND_868,
      RADIO_HW_SUPPORTED_BAND_915,
      RADIO_HW_SUPPORTED_BAND_23,
      RADIO_HW_SUPPORTED_BAND_24,
      RADIO_HW_SUPPORTED_BAND_25,
      RADIO_HW_SUPPORTED_BAND_58
   };

   int countSupported = 0;
   for( int r=0; r < sizeof(radio_hw_supported_bands)/sizeof(radio_hw_supported_bands[0]); r++ )
   {
      u32* channels;
      int channelcount = _getChannelsAndCount(supportedBands & radio_hw_supported_bands[r], &channels);
      if( channels != NULL ) {
         for( int i=0; i<channelcount; i++ )
         {
            *pOutChannels = channels[i];
            pOutChannels++;
            countSupported++;
            if ( countSupported >= maxChannels )
               return countSupported;
         }
         if ( includeSeparator )
         {
            *pOutChannels = 0;
            pOutChannels++;
            countSupported++;
            if ( countSupported >= maxChannels )
               return countSupported;
         }
      }
   }

   return countSupported;
}

int *getDataRatesBPS() { return s_WiFidataRates; }
int getDataRatesCount() { return sizeof(s_WiFidataRates)/sizeof(s_WiFidataRates[0]); }

u32 getRealDataRateFromMCSRate(int mcsIndex, int iHT40)
{
   u32 uMul = 1;
   if ( iHT40 )
      uMul = 2;
   if ( 0 == mcsIndex )
      return 6000000*uMul;
   if ( 1 == mcsIndex )
      return 13000000*uMul;
   if ( 2 == mcsIndex )
      return 19000000*uMul;
   if ( 3 == mcsIndex )
      return 26000000*uMul;
   if ( 4 == mcsIndex )
      return 39000000*uMul;
   if ( 5 == mcsIndex )
      return 52000000*uMul;
   if ( 6 == mcsIndex )
      return 58000000*uMul;
   if ( 7 == mcsIndex )
      return 65000000*uMul;
   if ( 8 == mcsIndex )
      return 78000000*uMul;
   if ( 9 == mcsIndex )
      return 78000000*uMul;
   return 58000000*uMul;
}

u32 getRealDataRateFromRadioDataRate(int dataRateBPS, int iHT40)
{
   if ( dataRateBPS < 0 )
      return getRealDataRateFromMCSRate(-dataRateBPS-1, iHT40);
   else if ( dataRateBPS <= 56 )
      return ((u32)(dataRateBPS))*1000*1000;
   else
      return (u32)dataRateBPS;
}

void getSystemVersionString(char* p, u32 swversion)
{
   if ( NULL == p )
      return;
   u32 val = swversion;
   u32 major = (val >> 8) & 0xFF;
   u32 minor = val & 0xFF;
   sprintf(p, "%u.%u", major, minor);
   int len = strlen(p);

   if ( len > 2 )
   {
      if ( p[len-1] == '0' && p[len-2] != '.' )
         p[len-1] = 0;
   }

   if ( minor > 10 && ((minor % 10) != 0) )
      sprintf(p, "%u.%u.%u", major, minor/10, minor%10);
}

int config_file_get_value(const char* szPropName)
{
   char szComm[MAX_FILE_PATH_SIZE];
   char szOut[1024];
   char* szTmp = NULL;
   int value = 0;

   sprintf(szComm, "grep '#%s!=' /boot/config.txt", szPropName);
   hw_execute_bash_command_silent(szComm, szOut);
   if ( strlen(szOut) > 5 )
   {
      szTmp = strchr(szOut, '=');
      if ( NULL != szTmp )
         sscanf(szTmp+1, "%d", &value);
      if ( value > 0 )
         value = -value;
   }

   sprintf(szComm, "grep '%s=' /boot/config.txt", szPropName);
   hw_execute_bash_command_silent(szComm, szOut);
   if ( strlen(szOut) > 5 )
   {
      szTmp = strchr(szOut, '=');
      if ( NULL != szTmp )
         sscanf(szTmp+1, "%d", &value);
      if ( value < 0 )
         value = -value;
   }
   return value;
}


void config_file_add_value(const char* szFile, const char* szPropName, int value)
{
   char szComm[MAX_FILE_PATH_SIZE];
   char szOutput[1024];
   sprintf(szComm, "cat %s | grep %s", szFile, szPropName);
   hw_execute_bash_command(szComm, szOutput);
   if ( strlen(szOutput) >= strlen(szPropName) )
      return;

   u8 data[4096];
   FILE* fd = fopen(szFile, "r");
   if ( NULL == fd )
      return;
   int nSize = fread(data, 1, 4095, fd);
   fclose(fd);
   if ( nSize <= 0 )
      return;

   fd = fopen(szFile, "w");
   if ( NULL == fd )
      return;

   fprintf(fd, "%s=%d\n", szPropName, value);
   fwrite(data, 1, nSize, fd);
   fclose(fd);
}


void config_file_set_value(const char* szFile, const char* szPropName, int value)
{
   char szComm[MAX_FILE_PATH_SIZE];
   if ( value <= 0 )
   {
      sprintf(szComm, "sed -i 's/#%s!=[-0-9]*/#%s!=%d/g' %s", szPropName, szPropName, value, szFile);
      hw_execute_bash_command(szComm, NULL);

      sprintf(szComm, "sed -i 's/%s=[-0-9]*/#%s!=%d/g' %s", szPropName, szPropName, value, szFile);
      hw_execute_bash_command(szComm, NULL);
   }
   else
   {
      sprintf(szComm, "sed -i 's/#%s!=[-0-9]*/%s=%d/g' %s", szPropName, szPropName, value, szFile);
      hw_execute_bash_command(szComm, NULL);

      sprintf(szComm, "sed -i 's/%s=[-0-9]*/%s=%d/g' %s", szPropName, szPropName, value, szFile);
      hw_execute_bash_command(szComm, NULL);
   }
}

void config_file_force_value(const char* szFile, const char* szPropName, int value)
{
   char szComm[MAX_FILE_PATH_SIZE];
   sprintf(szComm, "sed -i 's/#%s!=[-0-9]*/%s=%d/g' %s", szPropName, szPropName, value, szFile);
   hw_execute_bash_command(szComm, NULL);

   sprintf(szComm, "sed -i 's/#%s=[-0-9]*/%s=%d/g' %s", szPropName, szPropName, value, szFile);
   hw_execute_bash_command(szComm, NULL);

   sprintf(szComm, "sed -i 's/%s!=[-0-9]*/%s=%d/g' %s", szPropName, szPropName, value, szFile);
   hw_execute_bash_command(szComm, NULL);

   sprintf(szComm, "sed -i 's/%s=[-0-9]*/%s=%d/g' %s", szPropName, szPropName, value, szFile);
   hw_execute_bash_command(szComm, NULL);
}


void save_simple_config_fileU(const char* fileName, u32 value)
{
   FILE* fd = fopen(fileName, "w");
   if ( NULL == fd )
   {
      log_softerror_and_alarm("Failed to simple configuration to file: %s",fileName);
      return;
   }
   fprintf(fd, "%u\n", value);
   fclose(fd);
   log_line("Saved value %u to file: %s", value, fileName);
}

u32 load_simple_config_fileU(const char* fileName, u32 defaultValue)
{
   u32 returnValue = defaultValue;

   FILE* fd = fopen(fileName, "r");
   if ( NULL == fd )
   {
      log_softerror_and_alarm("Failed to load simple configuration from file: %s (missing file)",fileName);
      return defaultValue;
   }
   if ( 1 != fscanf(fd, "%u", &returnValue) )
   {
      log_softerror_and_alarm("Failed to load simple configuration from file: %s (invalid config file)",fileName);
      return defaultValue;
   }   
   
   fclose(fd);
   log_line("Loaded value %u from file: %s", returnValue, fileName);
   return returnValue;
}

void save_simple_config_fileI(const char* fileName, int value)
{
   FILE* fd = fopen(fileName, "w");
   if ( NULL == fd )
   {
      log_softerror_and_alarm("Failed to save simple configuration to file: %s",fileName);
      return;
   }
   fprintf(fd, "%d\n", value);
   fclose(fd);
}

int load_simple_config_fileI(const char* fileName, int defaultValue)
{
   int returnValue = defaultValue;

   FILE* fd = fopen(fileName, "r");
   if ( NULL == fd )
   {
      log_softerror_and_alarm("Failed to load simple configuration from file: %s (missing file)",fileName);
      return defaultValue;
   }
   if ( 1 != fscanf(fd, "%d", &returnValue) )
   {
      log_softerror_and_alarm("Failed to load simple configuration from file: %s (invalid config file)",fileName);
      log_line("Saving a default value (%d) and file.", defaultValue);
      save_simple_config_fileI(fileName, defaultValue);
      return defaultValue;
   }   
   
   fclose(fd);
   return returnValue;
}

FILE* try_open_base_version_file(char* szOutputFile)
{
   char szFile[MAX_FILE_PATH_SIZE];

   if ( NULL != szOutputFile )
      szOutputFile[0] = 0;

   strcpy(szFile, FOLDER_BINARIES);
   strcat(szFile, FILE_INFO_VERSION);
   FILE* fd = fopen(szFile, "r");
   if ( NULL != fd )
   {
      if ( NULL != szOutputFile )
         strcpy(szOutputFile, szFile);
      return fd;
   }

   strcpy(szFile, FOLDER_CONFIG);
   strcat(szFile, FILE_INFO_VERSION);
   fd = fopen(szFile, "r");
   if ( NULL != fd )
   {
      if ( NULL != szOutputFile )
         strcpy(szOutputFile, szFile);
      return fd;
   }

   strcpy(szFile, "/root/");
   strcat(szFile, FILE_INFO_VERSION);
   fd = fopen(szFile, "r");
   if ( NULL != fd )
   {
      if ( NULL != szOutputFile )
         strcpy(szOutputFile, szFile);
      return fd;
   }

   fd = fopen("ruby_ver.txt", "r");
   if ( NULL != fd )
   {
      if ( NULL != szOutputFile )
         strcpy(szOutputFile, szFile);
      return fd;
   }

   return fd;
}

void get_Ruby_BaseVersion(int* pMajor, int* pMinor)
{
   int iMajor = 0;
   int iMinor = 0;

   char szVersion[32];
   szVersion[0] = 0;

   FILE* fd = try_open_base_version_file(NULL);
   if ( NULL != fd )
   {
      if ( 1 != fscanf(fd, "%s", szVersion) )
         szVersion[0] = 0;
      fclose(fd);
   }
   else
   {
      if ( NULL != pMajor )
         *pMajor = SYSTEM_SW_VERSION_MAJOR;
      if ( NULL != pMinor )
         *pMinor = 0;
      return;
   }

   if ( 0 != szVersion[0] )
   {
      char* p = &szVersion[0];
      while ( *p )
      {
         if ( isdigit(*p) )
           iMajor = iMajor * 10 + ((*p)-'0');
         if ( (*p) == '.' )
           break;
         p++;
      }
      if ( 0 != *p )
      {
         p++;
         while ( *p )
         {
            if ( isdigit(*p) )
               iMinor = iMinor * 10 + ((*p)-'0');
            if ( (*p) == '.' )
              break;
            p++;
         }
      }
      if ( iMinor > 9 )
         iMinor = iMinor/10;
   }
   if ( NULL != pMajor )
      *pMajor = iMajor;
   if ( NULL != pMinor )
      *pMinor = iMinor;
}

void get_Ruby_UpdatedVersion(int* pMajor, int* pMinor)
{
   int iMajor = 0;
   int iMinor = 0;

   char szVersion[32];
   szVersion[0] = 0;

   char szFile[MAX_FILE_PATH_SIZE];
   strcpy(szFile, FOLDER_CONFIG);
   strcat(szFile, FILE_INFO_LAST_UPDATE);
   FILE* fd = fopen(szFile, "r");
   if ( NULL != fd )
   {
      if ( 1 != fscanf(fd, "%s", szVersion) )
         szVersion[0] = 0;
      fclose(fd);
   }

   if ( 0 != szVersion[0] )
   {
      char* p = &szVersion[0];
      while ( *p )
      {
         if ( isdigit(*p) )
           iMajor = iMajor * 10 + ((*p)-'0');
         if ( (*p) == '.' )
           break;
         p++;
      }
      if ( 0 != *p )
      {
         p++;
         while ( *p )
         {
            if ( isdigit(*p) )
               iMinor = iMinor * 10 + ((*p)-'0');
            if ( (*p) == '.' )
              break;
            p++;
         }
      }
      if ( iMinor > 9 )
         iMinor = iMinor/10;
   }
   if ( NULL != pMajor )
      *pMajor = iMajor;
   if ( NULL != pMinor )
      *pMinor = iMinor;
}

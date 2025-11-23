/*
 * sd_interface.h
 *
 *  Created on: Nov 7, 2025
 *      Author: hcwon
 */

#ifndef INC_SD_INTERFACE_H_
#define INC_SD_INTERFACE_H_

#include "ff.h"
#include "lcd.h"
#include "stm32f4xx_hal.h"

#define MAX_MUSIC_FILES 50
#define MAX_FILENAME_LEN 32
#define SD_MOUNT_PATH "0:"

typedef struct {
    char chunkID[4];        // "RIFF"
    uint32_t chunkSize;     // File size - 8
    char format[4];         // "WAVE"
    char subchunk1ID[4];    // "fmt "
    uint32_t subchunk1Size; // 16 for PCM
    uint16_t audioFormat;   // 1 for PCM
    uint16_t numChannels;   // 1 mono, 2 stereo
    uint32_t sampleRate;    // e.g., 44100
    uint32_t byteRate;      // sampleRate * numChannels * bitsPerSample/8
    uint16_t blockAlign;    // numChannels * bitsPerSample/8
    uint16_t bitsPerSample; // 8, 16, etc.
    char subchunk2ID[4];    // "data"
    uint32_t subchunk2Size; // Data size
} WAV_Header_t;

extern char music_files[MAX_MUSIC_FILES][MAX_FILENAME_LEN];
extern uint8_t num_music_files;

FRESULT SD_Mount(void);
FRESULT SD_Unmount(void);
FRESULT SD_ScanMusicFiles(void);  // Scans root for .wav and .mp3, stores in music_files array, displays on LCD
FRESULT SD_OpenFile(const char* filename, FIL* fp);  // Opens file from root
FRESULT SD_GetWAVHeader(FIL* fp, WAV_Header_t* header);  // Extracts WAV header
void SD_DisplayStatus(const char* message, uint16_t color);  // Helper to display debug on LCD
#endif /* INC_SD_INTERFACE_H_ */

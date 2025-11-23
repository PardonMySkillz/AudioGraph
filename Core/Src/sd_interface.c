/*
 * sd_interface.c
 *
 *  Created on: Nov 7, 2025
 *      Author: hcwon
 */

#include "sd_interface.h"
#include <string.h>  // For strstr, strcpy, etc.

char music_files[MAX_MUSIC_FILES][MAX_FILENAME_LEN];
uint8_t num_music_files = 0;

extern FATFS SDFatFS;

// Helper: Display on LCD
void SD_DisplayStatus(const char* message, uint16_t color) {
    static uint16_t y_pos = 20;
    LCD_DrawString(10, y_pos, message, color, LCD_COLOR_BLACK);
    y_pos += CHAR_HEIGHT + 2;
    if (y_pos >= LCD_HEIGHT - CHAR_HEIGHT) {
        y_pos = 20;
        LCD_Fill(LCD_COLOR_BLACK);
    }
}

FRESULT SD_Mount(void) {
    FRESULT res;
//    SD_DisplayStatus("Mounting SD...", LCD_COLOR_YELLOW);
    res = f_mount(&SDFatFS, SD_MOUNT_PATH, 1);
    if (res == FR_OK) {
//        SD_DisplayStatus("SD Mounted OK", LCD_COLOR_GREEN);
    } else {
        char buf[32];
        sprintf(buf, "Mount fail: %d", res);
        SD_DisplayStatus(buf, LCD_COLOR_RED);
    }
    return res;
}

FRESULT SD_Unmount(void) {
    FRESULT res;
//    SD_DisplayStatus("Unmounting SD...", LCD_COLOR_YELLOW);
    res = f_mount(NULL, SD_MOUNT_PATH, 0);
    if (res == FR_OK) {
//        SD_DisplayStatus("SD Unmounted OK", LCD_COLOR_GREEN);
    } else {
//        SD_DisplayStatus("Unmount failed", LCD_COLOR_RED);
    }
    return res;
}

FRESULT SD_ScanMusicFiles(void) {
    FRESULT res;
    DIR dir;
    FILINFO fno;
    num_music_files = 0;
    memset(music_files, 0, sizeof(music_files));

//    SD_DisplayStatus("Scanning root...", LCD_COLOR_YELLOW);

    res = f_opendir(&dir, "/");
    if (res != FR_OK) {
//        SD_DisplayStatus("Open root failed", LCD_COLOR_RED);
        return res;
    }

    while (num_music_files < MAX_MUSIC_FILES) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) break;

        if (fno.fattrib & AM_DIR) continue;

        char* ext = strrchr(fno.fname, '.');
        if (ext && (strcasecmp(ext, ".wav") == 0 || strcasecmp(ext, ".mp3") == 0)) {
            strncpy(music_files[num_music_files], fno.fname, MAX_FILENAME_LEN - 1);
            num_music_files++;
        }
    }
    f_closedir(&dir);

    if (num_music_files > 0) {
        char buf[32];
        sprintf(buf, "Found %d files", num_music_files);
//        SD_DisplayStatus(buf, LCD_COLOR_GREEN);
    } else {
//        SD_DisplayStatus("No .wav/.mp3 found", LCD_COLOR_RED);
    }

    return FR_OK;
}

FRESULT SD_OpenFile(const char* filename, FIL* fp) {
    FRESULT res;
    char path[64];
    sprintf(path, "/%s", filename);

//    SD_DisplayStatus("Opening:", LCD_COLOR_YELLOW);
//    SD_DisplayStatus(filename, LCD_COLOR_YELLOW);

    res = f_open(fp, path, FA_READ);
    if (res == FR_OK) {
//        SD_DisplayStatus("File opened", LCD_COLOR_GREEN);
    } else {
        char buf[32];
//        sprintf(buf, "Open err: %d", res);
//        SD_DisplayStatus(buf, LCD_COLOR_RED);
    }
    return res;
}

FRESULT SD_GetWAVHeader(FIL* fp, WAV_Header_t* header) {
    FRESULT res;
    UINT br;

//    SD_DisplayStatus("Reading WAV header...", LCD_COLOR_YELLOW);

    res = f_read(fp, header, sizeof(WAV_Header_t), &br);
    if (res != FR_OK || br != sizeof(WAV_Header_t)) {
//        SD_DisplayStatus("Header read fail", LCD_COLOR_RED);
        return res;
    }

    if (strncmp(header->chunkID, "RIFF", 4) == 0 &&
        strncmp(header->format, "WAVE", 4) == 0 &&
        strncmp(header->subchunk1ID, "fmt ", 4) == 0) {

        char buf[40];
        sprintf(buf, "SR: %lu Hz", header->sampleRate);
//        SD_DisplayStatus(buf, LCD_COLOR_WHITE);
        sprintf(buf, "Ch: %d, Bits: %d", header->numChannels, header->bitsPerSample);
//        SD_DisplayStatus(buf, LCD_COLOR_WHITE);
//        SD_DisplayStatus("WAV OK", LCD_COLOR_GREEN);
    } else {
//        SD_DisplayStatus("Invalid WAV", LCD_COLOR_RED);
        res = FR_INVALID_OBJECT;
    }

    return res;
}

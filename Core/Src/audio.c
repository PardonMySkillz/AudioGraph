/*
 * audio.c
 *
 *  Created on: Nov 17, 2025
 *      Author: hcwon
 */


#include "audio.h"
#include "main.h"  // For hi2s3 (I2S handle)
#include <string.h>
#include <stdio.h>
#include <stdbool.h>



extern ADC_HandleTypeDef hadc3;


extern osMessageQueueId_t guiUpdateQueueHandle;

static int16_t i2s_dma_buffer[I2S_DMA_BUFFER_SIZE];
static int16_t *do_buffer = NULL;
static FIL *current_file = NULL;
static WAV_Format_t current_wav;
static uint8_t end_of_file = 0;
static uint8_t paused = 0;
static uint8_t is_drawing = 0;

static volatile bool song_finished = false;      // Set once at EOF
static volatile bool dma_still_running = false;     // Track if I2S DMA is active

static uint32_t data_start_offset = 0;
static uint32_t total_duration_sec = 0;
static uint32_t bytes_per_second = 0;        // Will be calculated from WAV header
static uint32_t total_data_bytes = 0;        // Size of actual audio data
static uint32_t bytes_played = 0;            // How many bytes we've streamed so far


volatile q15_t volume_scale_fract = 32767;  // Full volume (approx 1.0 in Q15)
volatile int8_t volume_shift = 0;           // No shift by default

static void audio_SetI2SFreq(uint32_t freq) {
    // Stop the DMA transfers
    HAL_I2S_DMAStop(&hi2s3);
    // Deinit
    HAL_I2S_DeInit(&hi2s3);
    if (freq > 0) {
        hi2s3.Init.AudioFreq = freq * 2;
        HAL_I2S_Init(&hi2s3);
        HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*)i2s_dma_buffer, I2S_DMA_BUFFER_SIZE);
    }
}

static void audio_ProcessBuffer(int16_t *target_buffer) {
    int16_t buf[2 * I2S_DMA_BUFFER_SAMPLES] = { 0 };
    unsigned int bytes_read = 0;

    if (current_file != NULL && f_read(current_file, buf, sizeof(buf), &bytes_read) == FR_OK) {
    	uint32_t num_samples = bytes_read / sizeof(int16_t);
    	arm_scale_q15((q15_t *)buf, volume_scale_fract, volume_shift, (q15_t *)buf, num_samples);
        memcpy(target_buffer, buf, sizeof(buf));
        if (bytes_read < sizeof(buf)) {
            printf("File done!\n");
            memset((uint8_t*)target_buffer + bytes_read, 0, sizeof(buf) - bytes_read);
            HAL_I2S_DeInit(&hi2s3);
            end_of_file = 1;
        }
        // === Update playback position ===
        bytes_played += bytes_read;
    } else {
        // Fill with silence on error
        memset(target_buffer, 0, 2 * I2S_DMA_BUFFER_SAMPLES * sizeof(int16_t));
        end_of_file = 1;
    }
}

void audio_init(void) {
	memset(i2s_dma_buffer, 0, sizeof(i2s_dma_buffer));
	HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*)i2s_dma_buffer, I2S_DMA_BUFFER_SIZE);
}

FRESULT audio_ParseWAVHeader(FIL *f, WAV_Format_t *format) {
    char buf[512];
    unsigned int n;
    if (f_read(f, buf, sizeof(buf), &n) != FR_OK || n < 44) {
        printf("Read error or too small\n");
        return FR_INT_ERR;
    }

    if (memcmp(buf, "RIFF", 4) != 0 || memcmp(buf+8, "WAVE", 4) != 0) {
        printf("Not a valid WAV file\n");
        return FR_INT_ERR;
    }

    // Copy format chunk (starts at byte 20)
    memcpy(format, &buf[20], sizeof(WAV_Format_t));

    // Find "data" chunk
    uint32_t data_chunk_size = 0;
    data_start_offset = 0;

    for (int i = 12; i < n - 8; i++) {
        if (memcmp(buf + i, "data", 4) == 0) {
            data_chunk_size = *(uint32_t*)(buf + i + 4);
            data_start_offset = i + 8;
            break;
        }
    }

    if (data_start_offset == 0) {
        printf("No data chunk found\n");
        return FR_INT_ERR;
    }

    total_data_bytes = data_chunk_size;  // Now exact!

    if (f_lseek(f, data_start_offset) != FR_OK) {
        return FR_INT_ERR;
    }

    printf("WAV Header parsed: %lu Hz, %d ch, %d bps, data @ %lu, size %lu bytes\n",
           format->frequency, format->channels, format->bits_per_sample,
           data_start_offset, total_data_bytes);

    return FR_OK;
}

void audio_play(FIL *file, WAV_Format_t *format) {

	// --- Safety reset ---
	if (current_file != NULL) {
		HAL_I2S_DMAStop(&hi2s3);
		f_close(current_file);
	}

	song_finished = false;
	dma_still_running = false;
	end_of_file = 0;
	bytes_played = 0;
	do_buffer = NULL;
	if (file == NULL || format == NULL) {
		return;
	}
	current_file = file;
	end_of_file = 0;

	bytes_per_second = format->frequency * format->channels * (format->bits_per_sample / 8);
	total_data_bytes = f_size(file) - data_start_offset;

	total_duration_sec = total_data_bytes / bytes_per_second;
	bytes_played = 0;
	audio_SetI2SFreq(format->frequency);

	GuiUpdate_t init_update = {
		.cmd = GUI_UPDATE_PROGRESS,
		.current_time = 0,
		.total_time = total_duration_sec
	};
	osMessageQueuePut(guiUpdateQueueHandle, &init_update, 0, 10);
}

void audio_stop(void) {
	audio_SetI2SFreq(0);
	if (current_file != NULL) {
		f_close(current_file);
		current_file = NULL;
	}
	paused = 0;
}

void audio_pause(void)
{
    if (!paused) {
    	// Fill current DMA buffer with silence before pausing
    	memset(i2s_dma_buffer, 0, sizeof(i2s_dma_buffer));
        // Wait for current half to finish transmitting
    	osDelay(5);
    	HAL_I2S_DMAPause(&hi2s3);
    	paused = 1;
    }
}

void audio_resume(void) {
	HAL_I2S_DMAResume(&hi2s3);
	paused = 0;
}


uint8_t audio_is_paused(void)
{
    return paused;
}

void audio_update(void)
{
    static uint32_t last_update_ms = 0;
    uint32_t now = HAL_GetTick();

    // === 1. Send final progress update + EOF exactly ONCE ===
    if (end_of_file && current_file != NULL && !song_finished) {
        uint32_t final_sec = bytes_played / bytes_per_second;

        GuiUpdate_t update = {
            .cmd = GUI_UPDATE_PROGRESS,
            .current_time = final_sec,
            .total_time = total_duration_sec
        };
        osMessageQueuePut(guiUpdateQueueHandle, &update, 0, 0);

        GuiUpdate_t eof = { .cmd = GUI_UPDATE_END_OF_FILE };
        osMessageQueuePut(guiUpdateQueueHandle, &eof, 0, 10);

        song_finished = true;                     // ← prevents sending again
    }

    // === 2. Wait until DMA has finished the last buffer ===
    if (end_of_file && dma_still_running) {
        // Do nothing → just wait for the last DMA callback to arrive
        return;
    }

    // === 3. Safe shutdown sequence ===
    if (song_finished && !dma_still_running && current_file != NULL) {
        HAL_I2S_DMAStop(&hi2s3);                 // Stop I2S + DMA
        f_close(current_file);                    // NOW it's safe to close
        current_file = NULL;
        bytes_played = 0;
        song_finished = false;

        printf("Song finished & resources cleaned\r\n");
    }

    // === 4. Normal periodic progress update (only while playing) ===
    if (current_file != NULL && !end_of_file && !paused &&
        (now - last_update_ms >= 500)) {

        uint32_t current_sec = bytes_played / bytes_per_second;

        GuiUpdate_t update = {
            .cmd = GUI_UPDATE_PROGRESS,
            .current_time = current_sec,
            .total_time = total_duration_sec
        };
        osMessageQueuePut(guiUpdateQueueHandle, &update, 0, 0);

        last_update_ms = now;
    }

    // === 5. Normal buffer refill ===
    if (do_buffer != NULL) {
        audio_ProcessBuffer(do_buffer);
        do_buffer = NULL;
    }
}

void audio_I2STxCpltCallback(void) {
    do_buffer = &i2s_dma_buffer[I2S_DMA_BUFFER_SIZE / 2];  // Second half
}

void audio_I2STxHalfCpltCallback(void) {
    do_buffer = &i2s_dma_buffer[0];  // First half
}

uint8_t audio_isEndOfFile(void) {
    return end_of_file;
}

void update_volume_from_adc(void) {
	HAL_ADC_Start(&hadc3);
	if (HAL_ADC_PollForConversion(&hadc3, 10) == HAL_OK) {
		uint16_t adc_val = HAL_ADC_GetValue(&hadc3);
		float volume_float = (float)adc_val / 4095.0f;  // Normalize to 0.0 - 1.0 (for 12-bit ADC)
		volume_scale_fract = (q15_t)(volume_float * 32767.0f);  // Convert to Q15 fractional scale
		volume_shift = 0;  // Default no shift (attenuation only)
	}
}

uint8_t audio_is_drawing(void) {
	return is_drawing;
}

void audio_start_drawing(void) {
	is_drawing = 1;
}
void audio_stop_drawing(void) {
	is_drawing = 0;
}






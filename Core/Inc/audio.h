/*
 * audio.h
 *
 *  Created on: Nov 17, 2025
 *      Author: hcwon
 */

#ifndef INC_AUDIO_H_
#define INC_AUDIO_H_

#include "fatfs.h"
#include <stdint.h>
#include "arm_math.h"

#define SAMPLE_FREQ 48000
#define I2S_DMA_BUFFER_SAMPLES 128
#define I2S_DMA_BUFFER_SIZE (2 * 2 * I2S_DMA_BUFFER_SAMPLES)

// WSOLA parameters
#define WSOLA_WINDOW_SIZE 256              // Window size for WSOLA
#define WSOLA_SEARCH_RANGE 128             // Search range for similarity
#define WSOLA_FRAME_SHIFT_NORMAL 128       // Frame shift for regular playback
#define WSOLA_MAX_BUFFER_SIZE (WSOLA_WINDOW_SIZE * 3 + WSOLA_SEARCH_RANGE)  // Max buffer size needed


extern I2S_HandleTypeDef hi2s3;

typedef enum {
    PLAYBACK_SPEED_HALF = 0,   // 0.5x speed
    PLAYBACK_SPEED_NORMAL,     // 1.0x speed
    PLAYBACK_SPEED_DOUBLE      // 2.0x speed
} PlaybackSpeed_t;


typedef struct {
	uint16_t format;
	uint16_t channels;
	uint32_t frequency;
	uint32_t bytes_per_sec;
	uint16_t bytes_per_block;
	uint16_t bits_per_sample;
} WAV_Format_t;

void audio_init(void);

void audio_play(FIL *file, WAV_Format_t *format);
void audio_stop(void);
void audio_update(void); //updates the audio buffer in main loop
void audio_pause(void);
void audio_resume(void);
uint8_t audio_is_paused(void);

void audio_start_drawing(void);
void audio_stop_drawing(void);
uint8_t audio_is_drawing(void);

void audio_I2STxCpltCallback(void);
void audio_I2STxHalfCpltCallback(void);

uint8_t audio_isEndOfFile(void);

typedef enum {
	GUI_UPDATE_PROGRESS,
	GUI_UPDATE_END_OF_FILE,
	GUI_UPDATE_ERROR,
} GuiUpdateCommmand_t;

typedef struct {
	GuiUpdateCommmand_t cmd;
	uint32_t param;
	uint32_t current_time;
	uint32_t total_time;
} GuiUpdate_t;


typedef struct {
	uint32_t current_seconds;
	uint32_t total_seconds;
} AudioTime_t;

AudioTime_t audio_get_current_time(void);
void        audio_send_progress_update(void);
void        audio_send_end_of_file(void);
void        audio_send_error(uint32_t code);

void 		update_volume_from_adc(void);
extern volatile q15_t volume_scale_fract;  // Default full volume (1.0 in Q15: 32767/32768 ≈1)
extern volatile int8_t volume_shift;

extern volatile float playback_speed;
#endif /* INC_AUDIO_H_ */

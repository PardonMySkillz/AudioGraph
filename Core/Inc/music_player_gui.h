/*
 * music_player_gui.h
 *
 *  Created on: Nov 17, 2025
 *      Author: hcwon
 */

#ifndef INC_MUSIC_PLAYER_GUI_H_
#define INC_MUSIC_PLAYER_GUI_H_

#include "lvgl.h"
#include "sd_interface.h"
#include "audio.h"


typedef enum {
	AUDIO_CMD_PLAY,
	AUDIO_CMD_STOP,
	AUDIO_CMD_PAUSE,
	AUDIO_CMD_RESUME,
	AUDIO_CMD_START_DRAW,
	AUDIO_CMD_STOP_DRAW
} AudioCommandType_t;

typedef struct {
	AudioCommandType_t cmd;  // The command type
	uint32_t param;          // Additional parameter (seek position, etc.)
	char filename[MAX_FILENAME_LEN]; // File to play
} AudioCommand_t;




void MusicPlayerGUI_Init(void);

void update_progress_label(uint32_t current_seconds, uint32_t total_seconds);

#endif /* INC_MUSIC_PLAYER_GUI_H_ */

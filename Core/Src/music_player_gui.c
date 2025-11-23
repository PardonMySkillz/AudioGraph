/*
 * music_player_gui.c
 *
 *  Created on: Nov 17, 2025
 *      Author: hcwon
 */

#include "music_player_gui.h"
#include <string.h>
#include <stdio.h>

static lv_obj_t *browser_screen = NULL;
static lv_obj_t *playback_screen = NULL;
static lv_obj_t *file_label = NULL;
static int current_index = 0;
static char playing_file[MAX_FILENAME_LEN];
static FIL current_fil;
static WAV_Format_t current_wav;

static lv_obj_t *playing_label = NULL;
static lv_obj_t *progress_label = NULL;


extern osMessageQId audioCommandQueueHandle;



// Forward declarations for event callbacks
static void prev_btn_event_cb(lv_event_t *e);
static void next_btn_event_cb(lv_event_t *e);
static void play_btn_event_cb(lv_event_t *e);
static void pause_btn_event_cb(lv_event_t *e);
static void back_btn_event_cb(lv_event_t *e);
static void draw_btn_event_cb(lv_event_t *e);
static void ff_btn_event_cb(lv_event_t *e);
static void slow_btn_event_cb(lv_event_t *e);

static void update_file_label(void) {
    if (file_label && num_music_files > 0) {
        char buf[64];
        snprintf(buf, sizeof(buf), "File %d/%d: %s", current_index + 1, num_music_files, music_files[current_index]);
        lv_label_set_text(file_label, buf);
    }
}

static void create_browser_screen(void) {
    browser_screen = lv_obj_create(NULL);  // Create a new screen object
    lv_obj_set_size(browser_screen, LCD_WIDTH, LCD_HEIGHT);
    lv_obj_set_style_bg_color(browser_screen, lv_color_hex(0x000000), LV_PART_MAIN);  // Black background

    // Title label
    lv_obj_t *title = lv_label_create(browser_screen);
    lv_label_set_text(title, "Music Browser");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);  // White text

    // File display label
    file_label = lv_label_create(browser_screen);
    update_file_label();  // Set initial text
    lv_obj_align(file_label, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_text_color(file_label, lv_color_hex(0x00FF00), LV_PART_MAIN);  // Green text

    // Previous button
    lv_obj_t *prev_btn = lv_btn_create(browser_screen);
    lv_obj_set_size(prev_btn, 80, 40);
    lv_obj_align(prev_btn, LV_ALIGN_BOTTOM_LEFT, 20, -50);
    lv_obj_add_event_cb(prev_btn, prev_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *prev_label = lv_label_create(prev_btn);
    lv_label_set_text(prev_label, "Prev");

    // Next button
    lv_obj_t *next_btn = lv_btn_create(browser_screen);
    lv_obj_set_size(next_btn, 80, 40);
    lv_obj_align(next_btn, LV_ALIGN_BOTTOM_RIGHT, -20, -50);
    lv_obj_add_event_cb(next_btn, next_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *next_label = lv_label_create(next_btn);
    lv_label_set_text(next_label, "Next");

    // Play button
    lv_obj_t *play_btn = lv_btn_create(browser_screen);
    lv_obj_set_size(play_btn, 100, 50);
    lv_obj_align(play_btn, LV_ALIGN_BOTTOM_MID, 0, -100);
    lv_obj_add_event_cb(play_btn, play_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *play_label = lv_label_create(play_btn);
    lv_label_set_text(play_label, "Play");
}


static void create_playback_screen(void) {
    playback_screen = lv_obj_create(NULL);
    lv_obj_set_size(playback_screen, LCD_WIDTH, LCD_HEIGHT);
    lv_obj_set_style_bg_color(playback_screen, lv_color_hex(0x000000), LV_PART_MAIN);

    // Title label
    lv_obj_t *title = lv_label_create(playback_screen);
    lv_label_set_text(title, "Now Playing");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    // Playing file label (made static for dynamic updates)
    playing_label = lv_label_create(playback_screen);
    lv_label_set_text(playing_label, "");  // Set dynamically later
    lv_obj_align(playing_label, LV_ALIGN_CENTER, 0, -40);
    lv_obj_set_style_text_color(playing_label, lv_color_hex(0x00FF00), LV_PART_MAIN);

    // Progress label (made static for dynamic updates)
    progress_label = lv_label_create(playback_screen);
    lv_label_set_text(progress_label, "00:00 / 00:00");  // Initial text; update dynamically later
    lv_obj_align(progress_label, LV_ALIGN_CENTER, 0, 0);  // Positioned below playing_label
    lv_obj_set_style_text_color(progress_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);  // White text for visibility

    // Upper row: Slow button (left)
    lv_obj_t *slow_btn = lv_btn_create(playback_screen);
    lv_obj_set_size(slow_btn, 60, 40);
    lv_obj_align(slow_btn, LV_ALIGN_BOTTOM_MID, -75, -70);
    lv_obj_add_event_cb(slow_btn, slow_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *slow_label = lv_label_create(slow_btn);
    lv_label_set_text(slow_label, "0.5x");

    // Upper row: Pause button (center)
    lv_obj_t *pause_btn = lv_btn_create(playback_screen);
    lv_obj_set_size(pause_btn, 60, 40);
    lv_obj_align(pause_btn, LV_ALIGN_BOTTOM_MID, 0, -70);
    lv_obj_add_event_cb(pause_btn, pause_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *pause_label = lv_label_create(pause_btn);
    lv_label_set_text(pause_label, "Pause");  // Initial state (assumes playing)

    // Upper row: Fast forward button (right)
    lv_obj_t *ff_btn = lv_btn_create(playback_screen);
    lv_obj_set_size(ff_btn, 60, 40);
    lv_obj_align(ff_btn, LV_ALIGN_BOTTOM_MID, 75, -70);
    lv_obj_add_event_cb(ff_btn, ff_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *ff_label = lv_label_create(ff_btn);
    lv_label_set_text(ff_label, "2x");

    // Lower row: Draw button (left)
    lv_obj_t *draw_btn = lv_btn_create(playback_screen);
    lv_obj_set_size(draw_btn, 60, 40);
    lv_obj_align(draw_btn, LV_ALIGN_BOTTOM_MID, -40, -20);
    lv_obj_add_event_cb(draw_btn, draw_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *draw_label = lv_label_create(draw_btn);
    lv_label_set_text(draw_label, "Draw");

    // Lower row: Back button (right)
    lv_obj_t *back_btn = lv_btn_create(playback_screen);
    lv_obj_set_size(back_btn, 60, 40);  // Standardized size
    lv_obj_align(back_btn, LV_ALIGN_BOTTOM_MID, 40, -20);
    lv_obj_add_event_cb(back_btn, back_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, "Back");
}

// Event callback for Previous button
static void prev_btn_event_cb(lv_event_t *e) {
    if (num_music_files > 0) {
        current_index = (current_index - 1 + num_music_files) % num_music_files;
        update_file_label();
    }
}

// Event callback for Next button
static void next_btn_event_cb(lv_event_t *e) {
    if (num_music_files > 0) {
        current_index = (current_index + 1) % num_music_files;
        update_file_label();
    }
}


static void play_btn_event_cb(lv_event_t *e) {
	if (num_music_files == 0) return;

	lv_obj_t *playing_label = lv_obj_get_child(playback_screen, 1);

	AudioCommand_t play_cmd = {
			.cmd = AUDIO_CMD_PLAY,
			.param = current_index,
			.filename = ""
	};
	//cpy the filename safety
	if (current_index >= 0 && current_index < num_music_files) {
		strncpy(play_cmd.filename, music_files[current_index], MAX_FILENAME_LEN - 1);
		play_cmd.filename[MAX_FILENAME_LEN - 1] = '\0';

		lv_label_set_text_fmt(playing_label, "%s", play_cmd.filename);
	}
	else {
		lv_label_set_text(playing_label, "Invalid File");
		return;
	}
	osStatus_t status = osMessageQueuePut(audioCommandQueueHandle, &play_cmd, 0, osWaitForever);
	if (status != osOK) {
		lv_label_set_text(playing_label, "Error: Command failed");
	} else {
	}
	lv_scr_load(playback_screen);
}

void update_progress_label(uint32_t current_seconds, uint32_t total_seconds) {
	if (progress_label == NULL) return;
	uint32_t curr_min = current_seconds / 60;
	uint32_t curr_sec = current_seconds % 60;
	uint32_t total_min = total_seconds / 60;
	uint32_t total_sec = total_seconds % 60;

	lv_label_set_text_fmt(progress_label, "%02lu:%02lu / %02lu:%02lu",
			curr_min, curr_sec, total_min, total_sec);
}


static void pause_btn_event_cb(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *label = lv_obj_get_child(btn, 0);  // Get the button's label child

    AudioCommand_t audio_cmd = { .cmd = 0 };  // No filename/param needed for pause/resume

    if (audio_is_paused()) {
        // Currently paused → send RESUME
        audio_cmd.cmd = AUDIO_CMD_RESUME;
        osMessageQueuePut(audioCommandQueueHandle, &audio_cmd, 0, osWaitForever);
        lv_label_set_text(label, "Pause");  // Update label for next click
    } else {
        // Currently playing → send PAUSE
        audio_cmd.cmd = AUDIO_CMD_PAUSE;
        osMessageQueuePut(audioCommandQueueHandle, &audio_cmd, 0, osWaitForever);
        lv_label_set_text(label, "Resume");  // Update label for next click
    }
}

static void back_btn_event_cb(lv_event_t *e) {
    AudioCommand_t stop_cmd = { .cmd = AUDIO_CMD_STOP };
    audio_resume();
    osMessageQueuePut(audioCommandQueueHandle, &stop_cmd, 0, osWaitForever);

    // Switch back to browser screen
    lv_scr_load(browser_screen);
}

static void draw_btn_event_cb(lv_event_t *e) {
    // TODO: Stop music playback and trigger CNC waveform plotting
	lv_obj_t *btn = lv_event_get_target(e);
	lv_obj_t *label = lv_obj_get_child(btn, 0);
	AudioCommand_t stop_cmd;
	if (!audio_is_drawing()) {
		stop_cmd.cmd = AUDIO_CMD_STOP;
		osMessageQueuePut(audioCommandQueueHandle, &stop_cmd, 0, osWaitForever);

		AudioCommand_t start_draw_cmd;
		start_draw_cmd.cmd = AUDIO_CMD_START_DRAW;
		strcpy(start_draw_cmd.filename, music_files[current_index]);
		osMessageQueuePut(audioCommandQueueHandle, &start_draw_cmd,0, osWaitForever);

		lv_label_set_text(label, "Stop");
	}
	else {
		AudioCommand_t stop_draw_cmd;
		stop_draw_cmd.cmd = AUDIO_CMD_STOP_DRAW;
		osMessageQueuePut(audioCommandQueueHandle, &stop_draw_cmd, 0, osWaitForever);
		AudioCommand_t play_cmd = {
				.cmd = AUDIO_CMD_PLAY,
				.param = current_index,
				.filename = ""
		};

		if (current_index >= 0 && current_index < num_music_files) {
			strncpy(play_cmd.filename, music_files[current_index], MAX_FILENAME_LEN - 1);
			play_cmd.filename[MAX_FILENAME_LEN - 1] = '\0';

			lv_label_set_text_fmt(playing_label, "%s", play_cmd.filename);
			osStatus_t status = osMessageQueuePut(audioCommandQueueHandle, &play_cmd, 0, osWaitForever);
			if (status == osOK) {
				lv_label_set_text(label, "Draw");
			}
		}
	}
}

static void ff_btn_event_cb(lv_event_t *e) {
    // TODO: Fast forward playback to 2x speed (e.g., adjust I2S freq or skip samples)
//	if (playback_speed != 2.0f) {
//		playback_speed = 2.0f;
//	}
//	else {
//		playback_speed = 1.0f;
//	}
}

static void slow_btn_event_cb(lv_event_t *e) {
    // TODO: Slow down playback to 0.5x speed (e.g., adjust I2S freq or interpolate samples)
//	if (playback_speed != 0.5f) {
//		playback_speed = 0.5f;
//	}
//	else {
//		playback_speed = 1.0f;
//	}
}




// Initialize the GUI
void MusicPlayerGUI_Init(void) {
    if (num_music_files == 0) return;
    create_browser_screen();
    create_playback_screen();
    // Load the browser screen initially
    lv_scr_load(browser_screen);
}




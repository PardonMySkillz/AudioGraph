# STM32 Music Player with GUI and Pen Plotter
Embedded systems course project implementing a music player with a graphical user interface (GUI) and a physical pen plotter on an STM32F407VET6 development board.
## ✨ Features

- **Audio Playback:** Decodes and plays WAV from a microSD card using the .
- **Graphical User Interface (GUI):**
  - Navigate through list of songs.
  - Play, Pause, Stop track controls.
  - external Variable resistor for Volume control.
  - Keep track of audio time in GUI.
- **Pen Plotter Integration:** Physically plots the audio waveform or song title on paper using stepper motor based XY plotter.
- **Touchscreen Control**.
- 
## 🛠️ Hardware Components
- **Microcontroller:** STM32F407VET6 Development Board
- **Display:** ILI9341 240*320 TFT LCD Screen, interfaced via FSMC
- **Audio Output:** I2S audio streaming PCM data to PCM5102a DAC chip; amplified wth PAM8406 Audio Amplifier
- **Storage:** microSD card, interfaced via SDIO
- **Pen Plotter:**
  - 4x 48 stepper motor
- **Power:** 12V Power Supply for motors, 3.3V for logic

## 📦 Software & Libraries
- **IDE:** [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)
- **Firmware:** STM32CubeF4 HAL & Low-Layer Drivers
- **Graphics Library:** [LVGL](https://lvgl.io/)
- **File System:** [FatFs](http://elm-chan.org/fsw/ff/00index_e.html) (FAT32 support for SD card)
- **Audio Decoding:** Custom WAV parser, or Helix MP3 decoder (if MP3 support is added)
-  **RTOS** Concurrent opiration for GUI rendering and Audio Playback control

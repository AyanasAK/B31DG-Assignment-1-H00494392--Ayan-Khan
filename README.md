ESP32 Waveform Generator
This repository contains the code for an ESP32-based waveform generator. The application generates two digital signals, DATA (Signal A) and SYNC (Signal B), controlled by two push buttons. The project includes implementations in both the Arduino IDE and the Espressif IoT Development Framework (ESP-IDF).


Project Overview
The application generates periodic digital signals using an ESP32 microcontroller. The signals are controlled by two push buttons:

OUTPUT_ENABLE: Enables or disables the DATA signal.

OUTPUT_SELECT: Toggles between the Normal and Alternative waveform behavior.

The timing parameters for the signals are calculated based on the first four letters of the developer's surname ("KHAN" in this case).

Code Files
1. Arduino IDE (namewave.ino)
Description: This file contains the Arduino code for generating the waveforms.

Functions:

  setup(): Initializes GPIO pins and serial communication.
  
  loop(): Continuously checks button states and generates waveforms.
  
  normWave(): Generates the Normal waveform.
  
  altWave(): Generates the Alternative waveform.
  
  syncTrig(): Generates the SYNC pulse.

2. ESP-IDF (nameWave_idf.c)
  Description: This file contains the ESP-IDF code for generating the waveforms.
  
  Functions:
  
  app_main(): Initializes GPIO pins and creates FreeRTOS tasks.
  
  buttonTask(): Handles button inputs.
  
  waveformTask(): Generates waveforms based on button states.
  
  normWave(): Generates the Normal waveform.
  
  altWave(): Generates the Alternative waveform.
  
  syncTrig(): Generates the SYNC pulse.




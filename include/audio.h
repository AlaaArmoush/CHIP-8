#pragma once

bool audio_init(void);
void audio_cleanup(void);
void audio_start_beep(void);
void audio_stop_beep(void);
bool audio_is_beeping(void);

#ifndef CSV_H
#define CSV_H

#include "song.h"

int csvEnsureFile(const char *filename);
int csvAppendSong(const char *filename, const Song *song);
int csvLoadSongs(const char *filename, Song songs[], int max_songs);
int csvSaveSongs(const char *filename, const Song songs[], int count);

#endif

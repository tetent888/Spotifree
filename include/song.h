#ifndef SONG_H
#define SONG_H

typedef struct Song {
    char id[20];
    char title[100];
    char artist[100];
    char genre[50];
    int duration_seconds;
    struct Song *next;
} Song;

void songInit(Song *song, const char *id, const char *title,
              const char *artist, const char *genre, int duration_seconds);
Song *createSong(const Song *song);

#endif

#ifndef SONG_H
#define SONG_H

typedef struct Song {
    int id;
    char title[100];
    char artist[100];
    int duration;
    struct Song *next;
} Song;

Song *createSong(int id, const char *title, const char *artist, int duration);

#endif

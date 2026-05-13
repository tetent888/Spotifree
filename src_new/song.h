#ifndef SONG_H
#define SONG_H

#define SONG_ID_SIZE 32
#define SONG_TITLE_SIZE 120
#define SONG_ARTIST_SIZE 120
#define SONG_GENRE_SIZE 80

typedef struct Song {
    char id[SONG_ID_SIZE];
    char title[SONG_TITLE_SIZE];
    char artist[SONG_ARTIST_SIZE];
    char genre[SONG_GENRE_SIZE];
    int duration_seconds;
} Song;

void songInit(Song *song, const char *id, const char *title, const char *artist,
              const char *genre, int duration_seconds);
void songPrint(const Song *song, int index);

#endif

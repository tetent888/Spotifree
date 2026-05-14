/*
 * song.c - Basic Song helper functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/song.h"

void songInit(Song *song, const char *id, const char *title,
              const char *artist, const char *genre, int duration_seconds) {
    if (song == NULL) {
        return;
    }

    strcpy(song->id, id);
    strcpy(song->title, title);
    strcpy(song->artist, artist);
    strcpy(song->genre, genre);
    song->duration_seconds = duration_seconds;
    song->next = NULL;
}

Song *createSong(const Song *song) {
    Song *newSong;

    if (song == NULL) {
        return NULL;
    }

    newSong = (Song *)malloc(sizeof(Song));
    if (newSong == NULL) {
        printf("Error: cannot allocate memory.\n");
        return NULL;
    }

    songInit(newSong, song->id, song->title, song->artist,
             song->genre, song->duration_seconds);

    return newSong;
}

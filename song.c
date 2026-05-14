/*
 * song.c - Basic Song node function
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/song.h"

Song *createSong(int id, const char *title, const char *artist, int duration) {
    Song *newSong;

    newSong = (Song *)malloc(sizeof(Song));
    if (newSong == NULL) {
        printf("Error: cannot allocate memory.\n");
        return NULL;
    }

    newSong->id = id;
    newSong->duration = duration;

    strcpy(newSong->title, title);
    strcpy(newSong->artist, artist);

    newSong->next = NULL;

    return newSong;
}

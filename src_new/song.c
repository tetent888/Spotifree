#include "song.h"

#include <stdio.h>
#include <string.h>

static void copyText(char *dest, size_t size, const char *src) {
    if (size == 0) {
        return;
    }
    snprintf(dest, size, "%s", src ? src : "");
}

void songInit(Song *song, const char *id, const char *title, const char *artist,
              const char *genre, int duration_seconds) {
    if (song == NULL) {
        return;
    }

    copyText(song->id, sizeof(song->id), id);
    copyText(song->title, sizeof(song->title), title);
    copyText(song->artist, sizeof(song->artist), artist);
    copyText(song->genre, sizeof(song->genre), genre);
    song->duration_seconds = duration_seconds;
}

void songPrint(const Song *song, int index) {
    int minutes;
    int seconds;

    if (song == NULL) {
        return;
    }

    minutes = song->duration_seconds / 60;
    seconds = song->duration_seconds % 60;

    printf("  %d. [%s] %s - %s\n", index, song->id, song->title, song->artist);
    printf("     Genre: %s | Duration: %d:%02d\n", song->genre, minutes, seconds);
}

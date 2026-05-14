#include "csv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void trimNewline(char *text) {
    text[strcspn(text, "\r\n")] = '\0';
}

static int parseSongLine(char *line, Song *song) {
    char *id;
    char *title;
    char *artist;
    char *genre;
    char *duration;

    trimNewline(line);

    id = strtok(line, ",");
    title = strtok(NULL, ",");
    artist = strtok(NULL, ",");
    genre = strtok(NULL, ",");
    duration = strtok(NULL, ",");

    if (id == NULL || title == NULL || artist == NULL || genre == NULL || duration == NULL) {
        return 0;
    }

    songInit(song, id, title, artist, genre, atoi(duration));
    return 1;
}

int csvEnsureFile(const char *filename) {
    FILE *file = fopen(filename, "r");

    if (file != NULL) {
        fclose(file);
        return 1;
    }

    file = fopen(filename, "w");
    if (file == NULL) {
        return 0;
    }

    fprintf(file, "id,title,artist,genre,duration_seconds\n");
    fclose(file);
    return 1;
}

int csvAppendSong(const char *filename, const Song *song) {
    FILE *file;

    if (song == NULL || !csvEnsureFile(filename)) {
        return 0;
    }

    file = fopen(filename, "a");
    if (file == NULL) {
        return 0;
    }

    fprintf(file, "%s,%s,%s,%s,%d\n",
            song->id, song->title, song->artist, song->genre, song->duration_seconds);
    fclose(file);
    return 1;
}

int csvLoadSongs(const char *filename, Song songs[], int max_songs) {
    FILE *file;
    char line[512];
    int count = 0;

    if (!csvEnsureFile(filename)) {
        return 0;
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    fgets(line, sizeof(line), file);
    while (count < max_songs && fgets(line, sizeof(line), file) != NULL) {
        if (parseSongLine(line, &songs[count])) {
            count++;
        }
    }

    fclose(file);
    return count;
}

int csvSaveSongs(const char *filename, const Song songs[], int count) {
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        return 0;
    }

    fprintf(file, "id,title,artist,genre,duration_seconds\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s,%s,%d\n",
                songs[i].id, songs[i].title, songs[i].artist,
                songs[i].genre, songs[i].duration_seconds);
    }

    fclose(file);
    return 1;
}

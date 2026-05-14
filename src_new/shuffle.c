#include "shuffle.h"

#include "csv.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SONGS_FILE "data/songs.csv"
#define MAX_SHUFFLE_SONGS 500

static void seedRandomOnce(void) {
    static int seeded = 0;

    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }
}

static void printSongLine(const Song *song, int number) {
    if (song == NULL) {
        return;
    }

    printf("  %d. [%s] %s - %s (%s, %d:%02d)\n",
           number, song->id, song->title, song->artist, song->genre,
           song->duration_seconds / 60,
           song->duration_seconds % 60);
}

int shufflePickSong(Song *out_song) {
    Song songs[MAX_SHUFFLE_SONGS];
    int count;
    int index;

    if (out_song == NULL) {
        return 0;
    }

    count = csvLoadSongs(SONGS_FILE, songs, MAX_SHUFFLE_SONGS);
    if (count <= 0) {
        return 0;
    }

    seedRandomOnce();
    index = rand() % count;
    *out_song = songs[index];
    return 1;
}

void shuffleShowSongs(int limit) {
    Song songs[MAX_SHUFFLE_SONGS];
    Song picked;
    int count;

    count = csvLoadSongs(SONGS_FILE, songs, MAX_SHUFFLE_SONGS);
    if (count <= 0) {
        printf("No songs found in %s\n", SONGS_FILE);
        return;
    }

    if (limit <= 0 || limit > count) {
        limit = count;
    }

    printf("\n========== SHUFFLE PREVIEW ==========\n");
    for (int i = 0; i < limit; i++) {
        if (shufflePickSong(&picked)) {
            printSongLine(&picked, i + 1);
        }
    }
    printf("=====================================\n\n");
}

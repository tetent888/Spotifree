#include "recommend.h"

#include "csv.h"
#include "song.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SONGS_FILE "data/songs.csv"
#define PLAYLIST_FILE "data/playlist.csv"
#define HISTORY_FILE "data/history.csv"
#define MAX_RECOMMEND_SONGS 200

static int sameText(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

static void seedRandomOnce(void) {
    static int seeded = 0;

    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }
}

static int hasSongId(const Song songs[], int count, const char *id) {
    for (int i = 0; i < count; i++) {
        if (sameText(songs[i].id, id)) {
            return 1;
        }
    }

    return 0;
}

static int chooseSeedSong(Song *seed) {
    Song history[MAX_RECOMMEND_SONGS];
    Song playlist[MAX_RECOMMEND_SONGS];
    int history_count;
    int playlist_count;

    history_count = csvLoadSongs(HISTORY_FILE, history, MAX_RECOMMEND_SONGS);
    if (history_count > 0) {
        *seed = history[history_count - 1];
        return 1;
    }

    playlist_count = csvLoadSongs(PLAYLIST_FILE, playlist, MAX_RECOMMEND_SONGS);
    if (playlist_count > 0) {
        *seed = playlist[playlist_count - 1];
        return 1;
    }

    return 0;
}

static int scoreSong(const Song *candidate, const Song *seed) {
    int score = 0;

    if (candidate == NULL || seed == NULL) {
        return 0;
    }

    if (sameText(candidate->artist, seed->artist)) {
        score += 3;
    }

    if (sameText(candidate->genre, seed->genre)) {
        score += 2;
    }

    return score;
}

int recommendPickRandomSong(Song *out_song) {
    Song all_songs[MAX_RECOMMEND_SONGS];
    int all_count;
    int random_index;

    if (out_song == NULL) {
        return 0;
    }

    all_count = csvLoadSongs(SONGS_FILE, all_songs, MAX_RECOMMEND_SONGS);
    if (all_count == 0) {
        return 0;
    }

    seedRandomOnce();
    random_index = rand() % all_count;
    *out_song = all_songs[random_index];
    return 1;
}

int recommendPickSong(Song *out_song) {
    Song seed;
    Song all_songs[MAX_RECOMMEND_SONGS];
    Song playlist[MAX_RECOMMEND_SONGS];
    int all_count;
    int playlist_count;
    int best_index = -1;
    int best_score = 0;

    if (out_song == NULL) {
        return 0;
    }

    all_count = csvLoadSongs(SONGS_FILE, all_songs, MAX_RECOMMEND_SONGS);
    if (all_count == 0) {
        return 0;
    }

    if (!chooseSeedSong(&seed)) {
        return recommendPickRandomSong(out_song);
    }

    playlist_count = csvLoadSongs(PLAYLIST_FILE, playlist, MAX_RECOMMEND_SONGS);
    for (int i = 0; i < all_count; i++) {
        int score;

        if (hasSongId(playlist, playlist_count, all_songs[i].id)) {
            continue;
        }

        score = scoreSong(&all_songs[i], &seed);
        if (score > best_score) {
            best_score = score;
            best_index = i;
        }
    }

    if (best_index >= 0) {
        *out_song = all_songs[best_index];
        return 1;
    }

    return recommendPickRandomSong(out_song);
}

void showRecommendations(void) {
    Song seed;
    Song all_songs[MAX_RECOMMEND_SONGS];
    Song playlist[MAX_RECOMMEND_SONGS];
    int all_count;
    int playlist_count;
    int shown = 0;

    all_count = csvLoadSongs(SONGS_FILE, all_songs, MAX_RECOMMEND_SONGS);
    if (all_count == 0) {
        printf("  No song database found. Add songs to data/songs.csv first.\n");
        return;
    }

    if (!chooseSeedSong(&seed)) {
        printf("  No playlist/history yet. Try these songs from the database:\n");
        for (int i = 0; i < all_count && shown < 5; i++) {
            songPrint(&all_songs[i], shown + 1);
            shown++;
        }
        return;
    }

    playlist_count = csvLoadSongs(PLAYLIST_FILE, playlist, MAX_RECOMMEND_SONGS);

    printf("  Based on: %s - %s (%s)\n", seed.title, seed.artist, seed.genre);
    printf("  Recommended songs:\n");

    for (int target_score = 5; target_score >= 2; target_score--) {
        for (int i = 0; i < all_count && shown < 5; i++) {
            if (hasSongId(playlist, playlist_count, all_songs[i].id)) {
                continue;
            }

            if (scoreSong(&all_songs[i], &seed) == target_score) {
                songPrint(&all_songs[i], shown + 1);
                shown++;
            }
        }
    }

    if (shown == 0) {
        printf("  No close match yet. Add more songs with the same artist or genre.\n");
    }
}

/*
 * main.c - Demo program for CSV + playlist + queue + stack
 */

#include <stdio.h>
#include <stdlib.h>

#include "csv.h"
#include "playlist.h"
#include "queue.h"
#include "stack.h"

#define CSV_FILE "data/songs.csv"
#define MAX_SONGS 500

static void freeSongCopy(Song *song) {
    free(song);
}

static void savePlaylistToCsv(Song *playlist) {
    Song songs[MAX_SONGS];
    int count;

    count = playlistToArray(playlist, songs, MAX_SONGS);
    if (csvSaveSongs(CSV_FILE, songs, count)) {
        printf("Saved songs to %s\n", CSV_FILE);
    } else {
        printf("Cannot save songs to %s\n", CSV_FILE);
    }
}

static void printPlayer(Song *nowPlaying, Queue *playQueue) {
    Song *nextSong = queuePeek(playQueue);

    printf("\n");
    if (nowPlaying == NULL) {
        printf("  Now Playing : -\n");
        printf("  Duration    : -                 Shuffle: OFF\n");
    } else {
        printf("  Now Playing : %s - %s\n", nowPlaying->title, nowPlaying->artist);
        printf("  Duration    : %d:%02d             Shuffle: OFF\n",
               nowPlaying->duration_seconds / 60,
               nowPlaying->duration_seconds % 60);
    }

    printf("\n");
    if (nextSong == NULL) {
        printf("  Next Song   : -\n");
    } else {
        printf("  Next Song   : %s\n", nextSong->title);
    }

    printf("  -----------------------------------------\n");
    printf("      [|<<]      [ || ]      [<>]    [>>|]\n");
    printf("     Previous    Pause    Shuffle    Next\n");
}

int main(void) {
    Song songs[MAX_SONGS];
    Song newSong;
    Song *playlist = NULL;
    Queue playQueue;
    Stack history;
    Song *nowPlaying = NULL;
    Song *found;
    int songCount;
    int choice;
    char id[20];
    char title[100];
    char artist[100];
    char genre[50];
    int duration_seconds;

    queueInit(&playQueue);
    stackInit(&history);

    songCount = csvLoadSongs(CSV_FILE, songs, MAX_SONGS);
    playlist = playlistFromArray(songs, songCount);

    printf("\n=== Playlist Manager ===\n");
    printf("Loaded %d songs from %s\n", songCount, CSV_FILE);

    while (1) {
        printPlayer(nowPlaying, &playQueue);

        printf("\n1) Add song          2) Delete song       3) Search song\n");
        printf("4) Show playlist     5) Add to queue      6) Play next\n");
        printf("7) Previous song     8) Show queue        9) Show history\n");
        printf("0) Save and exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                printf("ID: ");
                scanf(" %19[^\n]", id);
                printf("Title: ");
                scanf(" %99[^\n]", title);
                printf("Artist: ");
                scanf(" %99[^\n]", artist);
                printf("Genre: ");
                scanf(" %49[^\n]", genre);
                printf("Duration (seconds): ");
                scanf("%d", &duration_seconds);

                songInit(&newSong, id, title, artist, genre, duration_seconds);
                playlist = playlistAddEnd(playlist, &newSong);
                printf("Added: %s - %s\n", title, artist);
                break;

            case 2:
                playlistShow(playlist);
                printf("Song ID to delete: ");
                scanf(" %19[^\n]", id);
                playlist = playlistDeleteById(playlist, id);
                break;

            case 3:
                printf("Search title: ");
                scanf(" %99[^\n]", title);
                found = playlistFindByTitle(playlist, title);
                if (found == NULL) {
                    printf("Song not found.\n");
                } else {
                    printf("Found: [%s] %s - %s (%s, %d:%02d)\n",
                           found->id, found->title, found->artist, found->genre,
                           found->duration_seconds / 60,
                           found->duration_seconds % 60);
                }
                break;

            case 4:
                playlistShow(playlist);
                break;

            case 5:
                playlistShow(playlist);
                printf("Song ID to add to queue: ");
                scanf(" %19[^\n]", id);

                found = playlistFindById(playlist, id);
                if (found == NULL) {
                    printf("Song ID %s not found.\n", id);
                } else {
                    queueEnqueue(&playQueue, found);
                    printf("Added to queue: %s\n", found->title);
                }
                break;

            case 6: {
                Song *next = queueDequeue(&playQueue);
                if (next == NULL) {
                    printf("Queue is empty.\n");
                } else {
                    if (nowPlaying != NULL) {
                        stackPush(&history, nowPlaying);
                        freeSongCopy(nowPlaying);
                    }
                    nowPlaying = next;
                    printf("Now playing: %s - %s\n",
                           nowPlaying->title, nowPlaying->artist);
                }
                break;
            }

            case 7: {
                Song *previous = stackPop(&history);
                if (previous == NULL) {
                    printf("History is empty.\n");
                } else {
                    if (nowPlaying != NULL) {
                        queueEnqueue(&playQueue, nowPlaying);
                        freeSongCopy(nowPlaying);
                    }
                    nowPlaying = previous;
                    printf("Back to: %s - %s\n",
                           nowPlaying->title, nowPlaying->artist);
                }
                break;
            }

            case 8:
                queueShow(&playQueue);
                break;

            case 9:
                stackShow(&history);
                break;

            case 0:
                savePlaylistToCsv(playlist);
                freeSongCopy(nowPlaying);
                queueClear(&playQueue);
                stackClear(&history);
                playlistFree(playlist);
                printf("Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }

    savePlaylistToCsv(playlist);
    freeSongCopy(nowPlaying);
    queueClear(&playQueue);
    stackClear(&history);
    playlistFree(playlist);
    return 0;
}

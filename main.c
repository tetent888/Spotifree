/*
 * main.c - Interactive menu that uses playlist, queue, and stack modules
 */

#include <stdio.h>
#include <stdlib.h>

#include "include/playlist.h"
#include "include/queue.h"
#include "include/stack.h"

static void freeSongCopy(Song *song) {
    free(song);
}

static void printPlayer(Song *nowPlaying, Queue *playQueue) {
    Song *nextSong = peekQueue(playQueue);

    printf("\n");
    if (nowPlaying == NULL) {
        printf("  Now Playing : -\n");
        printf("  Duration    : -                 Shuffle: OFF\n");
    } else {
        printf("  Now Playing : %s - %s\n", nowPlaying->title, nowPlaying->artist);
        printf("  Duration    : %d:%02d             Shuffle: OFF\n",
               nowPlaying->duration / 60, nowPlaying->duration % 60);
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
    Song *playlist = NULL;
    Queue playQueue;
    Stack history;
    Song *nowPlaying = NULL;
    int choice, id, dur, dir;
    char title[100], artist[100];
    static int nextId = 1;

    initQueue(&playQueue);
    initStack(&history);

    printf("\n=== Playlist Manager ===\n");

    while (1) {
        printPlayer(nowPlaying, &playQueue);

        printf("\n1) Add to end        2) Add to front      3) Delete\n");
        printf("4) Move up/down      5) Show playlist     6) Add to queue\n");
        printf("7) Play next         8) Previous song     9) Show queue\n");
        printf("10) Show history\n");
        printf("0) Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                printf("Title: "); scanf(" %99[^\n]", title);
                printf("Artist: "); scanf(" %99[^\n]", artist);
                printf("Duration (seconds): "); scanf("%d", &dur);
                playlist = addToEnd(playlist, nextId++, title, artist, dur);
                break;

            case 2:
                printf("Title: "); scanf(" %99[^\n]", title);
                printf("Artist: "); scanf(" %99[^\n]", artist);
                printf("Duration (seconds): "); scanf("%d", &dur);
                playlist = addToFront(playlist, nextId++, title, artist, dur);
                break;

            case 3:
                printf("Song ID to delete: "); scanf("%d", &id);
                playlist = deleteSong(playlist, id);
                break;

            case 4:
                printf("Song ID: "); scanf("%d", &id);
                printf("Direction (-1=up, +1=down): "); scanf("%d", &dir);
                playlist = moveSong(playlist, id, dir);
                break;

            case 5:
                showPlaylist(playlist);
                break;

            case 6: {
                Song *found;

                showPlaylist(playlist);
                printf("Song ID to add to queue: ");
                scanf("%d", &id);

                found = findSongById(playlist, id);
                if (found == NULL) {
                    printf("[queue] Song ID %d not found.\n", id);
                } else {
                    enqueueSong(&playQueue, found);
                }
                break;
            }

            case 7: {
                Song *next = dequeueSong(&playQueue);
                if (next != NULL) {
                    if (nowPlaying != NULL) {
                        pushSong(&history, nowPlaying);
                        freeSongCopy(nowPlaying);
                    }
                    nowPlaying = next;
                    printf("[play] Now playing \"%s\" - %s\n",
                           nowPlaying->title, nowPlaying->artist);
                }
                break;
            }

            case 8: {
                Song *previous = popSong(&history);
                if (previous != NULL) {
                    if (nowPlaying != NULL) {
                        enqueueSong(&playQueue, nowPlaying);
                        freeSongCopy(nowPlaying);
                    }
                    nowPlaying = previous;
                    printf("[play] Back to \"%s\" - %s\n",
                           nowPlaying->title, nowPlaying->artist);
                }
                break;
            }

            case 9:
                showQueue(&playQueue);
                break;

            case 10:
                showStack(&history);
                break;

            case 0:
                freeSongCopy(nowPlaying);
                clearQueue(&playQueue);
                clearStack(&history);
                freePlaylist(playlist);
                printf("Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }

    freeSongCopy(nowPlaying);
    clearQueue(&playQueue);
    clearStack(&history);
    freePlaylist(playlist);
    return 0;
}

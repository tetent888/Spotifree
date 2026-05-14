/*
 * playlist.c - Linked list playlist
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "playlist.h"

Song *playlistAddEnd(Song *head, const Song *song) {
    Song *newSong = createSong(song);
    Song *curr;

    if (newSong == NULL) {
        return head;
    }

    if (head == NULL) {
        return newSong;
    }

    curr = head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = newSong;

    return head;
}

Song *playlistDeleteById(Song *head, const char *id) {
    Song *prev;
    Song *curr;

    if (head == NULL) {
        printf("Playlist is empty.\n");
        return NULL;
    }

    if (strcmp(head->id, id) == 0) {
        Song *temp = head;
        head = head->next;
        printf("Deleted: %s - %s\n", temp->title, temp->artist);
        free(temp);
        return head;
    }

    prev = head;
    curr = head->next;
    while (curr != NULL && strcmp(curr->id, id) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("Song ID %s not found.\n", id);
        return head;
    }

    prev->next = curr->next;
    printf("Deleted: %s - %s\n", curr->title, curr->artist);
    free(curr);

    return head;
}

Song *playlistFindById(Song *head, const char *id) {
    Song *curr = head;

    while (curr != NULL) {
        if (strcmp(curr->id, id) == 0) {
            return curr;
        }
        curr = curr->next;
    }

    return NULL;
}

Song *playlistFindByTitle(Song *head, const char *title) {
    Song *curr = head;

    while (curr != NULL) {
        if (strstr(curr->title, title) != NULL) {
            return curr;
        }
        curr = curr->next;
    }

    return NULL;
}

void playlistShow(Song *head) {
    Song *curr = head;
    int number = 1;
    int total = 0;

    if (head == NULL) {
        printf("(Playlist is empty)\n");
        return;
    }

    printf("\n==================== PLAYLIST ====================\n");
    printf("%-4s %-8s %-22s %-18s %-12s %-8s\n",
           "No.", "ID", "Title", "Artist", "Genre", "Time");
    printf("--------------------------------------------------\n");

    while (curr != NULL) {
        printf("%-4d %-8s %-22.22s %-18.18s %-12.12s %d:%02d\n",
               number, curr->id, curr->title, curr->artist, curr->genre,
               curr->duration_seconds / 60, curr->duration_seconds % 60);

        total += curr->duration_seconds;
        curr = curr->next;
        number++;
    }

    printf("--------------------------------------------------\n");
    printf("Total: %d songs | %d:%02d\n\n",
           number - 1, total / 60, total % 60);
}

void playlistFree(Song *head) {
    Song *curr = head;

    while (curr != NULL) {
        Song *temp = curr;
        curr = curr->next;
        free(temp);
    }
}

int playlistCount(Song *head) {
    int count = 0;

    while (head != NULL) {
        count++;
        head = head->next;
    }

    return count;
}

int playlistToArray(Song *head, Song songs[], int max_songs) {
    int count = 0;

    while (head != NULL && count < max_songs) {
        songInit(&songs[count], head->id, head->title, head->artist,
                 head->genre, head->duration_seconds);
        head = head->next;
        count++;
    }

    return count;
}

Song *playlistFromArray(Song songs[], int count) {
    Song *head = NULL;
    int i;

    for (i = 0; i < count; i++) {
        head = playlistAddEnd(head, &songs[i]);
    }

    return head;
}

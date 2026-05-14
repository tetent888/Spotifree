/*
 * playlist.c - Linked List playlist functions
 */

#include <stdio.h>
#include <stdlib.h>

#include "include/playlist.h"

Song *addToEnd(Song *head, int id, const char *title,
               const char *artist, int duration) {
    Song *newSong = createSong(id, title, artist, duration);
    if (newSong == NULL) return head;

    if (head == NULL) {
        printf("[+] Added \"%s\" to end of playlist.\n", title);
        return newSong;
    }

    Song *curr = head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = newSong;

    printf("[+] Added \"%s\" to end of playlist.\n", title);
    return head;
}

Song *addToFront(Song *head, int id, const char *title,
                 const char *artist, int duration) {
    Song *newSong = createSong(id, title, artist, duration);
    if (newSong == NULL) return head;

    newSong->next = head;
    printf("[+] Added \"%s\" to front of playlist.\n", title);
    return newSong;
}

Song *deleteSong(Song *head, int id) {
    if (head == NULL) {
        printf("[-] Playlist is empty.\n");
        return NULL;
    }

    if (head->id == id) {
        Song *temp = head;
        head = head->next;
        printf("[-] Deleted \"%s\" from playlist.\n", temp->title);
        free(temp);
        return head;
    }

    Song *prev = head;
    Song *curr = head->next;
    while (curr != NULL && curr->id != id) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("[-] Song with ID %d not found.\n", id);
        return head;
    }

    prev->next = curr->next;
    printf("[-] Deleted \"%s\" from playlist.\n", curr->title);
    free(curr);
    return head;
}

void showPlaylist(Song *head) {
    if (head == NULL) {
        printf("  (Playlist is empty)\n");
        return;
    }

    int total = 0;
    int pos = 1;
    Song *curr = head;

    printf("\n==================== MY PLAYLIST ====================\n");
    printf("#  %-32s  Duration\n", "Title - Artist");
    printf("-----------------------------------------------------\n");

    while (curr != NULL) {
        int min = curr->duration / 60;
        int sec = curr->duration % 60;
        char display[210];

        snprintf(display, sizeof(display), "%s - %s", curr->title, curr->artist);
        printf("%-2d %-32.32s  %3d:%02d  (id=%d)\n",
               pos, display, min, sec, curr->id);

        total += curr->duration;
        curr = curr->next;
        pos++;
    }

    printf("-----------------------------------------------------\n");
    printf("Total: %d songs | %d:%02d total duration\n\n",
           pos - 1, total / 60, total % 60);
}

Song *moveSong(Song *head, int id, int direction) {
    if (head == NULL || head->next == NULL) return head;

    Song *prev2 = NULL;
    Song *prev = NULL;
    Song *curr = head;

    while (curr != NULL && curr->id != id) {
        prev2 = prev;
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("[!] Song ID %d not found.\n", id);
        return head;
    }

    if (direction == -1) {
        if (prev == NULL) {
            printf("[!] Song is already at the top.\n");
            return head;
        }

        if (prev2 != NULL) prev2->next = curr;
        else head = curr;

        prev->next = curr->next;
        curr->next = prev;
        printf("[up] Moved \"%s\" up.\n", curr->title);
    } else {
        Song *next = curr->next;
        if (next == NULL) {
            printf("[!] Song is already at the bottom.\n");
            return head;
        }

        if (prev != NULL) prev->next = next;
        else head = next;

        curr->next = next->next;
        next->next = curr;
        printf("[down] Moved \"%s\" down.\n", curr->title);
    }

    return head;
}

void freePlaylist(Song *head) {
    Song *curr = head;
    while (curr != NULL) {
        Song *temp = curr;
        curr = curr->next;
        free(temp);
    }
    printf("[*] Playlist cleared from memory.\n");
}

int countSongs(Song *head) {
    int count = 0;
    while (head != NULL) {
        count++;
        head = head->next;
    }
    return count;
}

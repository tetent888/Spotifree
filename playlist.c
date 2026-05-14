/*
 * playlist.c — Music Playlist using Linked List
 * Features: add (front/back/position), delete, show, search, move
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─────────────────────────────────────────
   Node Structure
   ───────────────────────────────────────── */
typedef struct Song {
    int    id;
    char   title[100];
    char   artist[100];
    int    duration;     /* seconds */
    struct Song *next;
} Song;

/* ─────────────────────────────────────────
   Helper: create a new Song node
   ───────────────────────────────────────── */
Song *createSong(int id, const char *title, const char *artist, int duration) {
    Song *newSong = (Song *)malloc(sizeof(Song));
    if (!newSong) {
        printf("Error: Memory allocation failed!\n");
        return NULL;
    }
    newSong->id       = id;
    newSong->duration = duration;
    strncpy(newSong->title,  title,  sizeof(newSong->title)  - 1);
    strncpy(newSong->artist, artist, sizeof(newSong->artist) - 1);
    newSong->next = NULL;
    return newSong;
}

/* ─────────────────────────────────────────
   ADD — at the END of playlist
   ───────────────────────────────────────── */
Song *addToEnd(Song *head, int id, const char *title,
               const char *artist, int duration) {
    Song *newSong = createSong(id, title, artist, duration);
    if (!newSong) return head;

    if (head == NULL) return newSong;   /* empty list */

    Song *curr = head;
    while (curr->next != NULL)
        curr = curr->next;
    curr->next = newSong;

    printf("[+] Added \"%s\" to end of playlist.\n", title);
    return head;
}

/* ─────────────────────────────────────────
   ADD — at the FRONT of playlist
   ───────────────────────────────────────── */
Song *addToFront(Song *head, int id, const char *title,
                 const char *artist, int duration) {
    Song *newSong = createSong(id, title, artist, duration);
    if (!newSong) return head;

    newSong->next = head;
    printf("[+] Added \"%s\" to front of playlist.\n", title);
    return newSong;
}

/* ─────────────────────────────────────────
   ADD — at a specific POSITION (1-based)
   ───────────────────────────────────────── */
Song *addAtPosition(Song *head, int pos, int id, const char *title,
                    const char *artist, int duration) {
    if (pos <= 1)
        return addToFront(head, id, title, artist, duration);

    Song *newSong = createSong(id, title, artist, duration);
    if (!newSong) return head;

    Song *curr = head;
    for (int i = 1; i < pos - 1 && curr->next != NULL; i++)
        curr = curr->next;

    newSong->next = curr->next;
    curr->next    = newSong;

    printf("[+] Added \"%s\" at position %d.\n", title, pos);
    return head;
}

/* ─────────────────────────────────────────
   DELETE — by Song ID
   ───────────────────────────────────────── */
Song *deleteSong(Song *head, int id) {
    if (head == NULL) {
        printf("[-] Playlist is empty.\n");
        return NULL;
    }

    /* Case: head node matches */
    if (head->id == id) {
        Song *temp = head;
        head = head->next;
        printf("[-] Deleted \"%s\" from playlist.\n", temp->title);
        free(temp);
        return head;
    }

    /* Search for node to delete */
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

/* ─────────────────────────────────────────
   SEARCH — by title (case-sensitive)
   ───────────────────────────────────────── */
Song *searchByTitle(Song *head, const char *title) {
    Song *curr = head;
    int   pos  = 1;
    while (curr != NULL) {
        if (strstr(curr->title, title) != NULL) {
            printf("[?] Found: [%d] \"%s\" — %s (%d:%02d)\n",
                   pos, curr->title, curr->artist,
                   curr->duration / 60, curr->duration % 60);
            return curr;
        }
        curr = curr->next;
        pos++;
    }
    printf("[?] No song matching \"%s\" found.\n", title);
    return NULL;
}

/* ─────────────────────────────────────────
   SHOW — print all songs
   ───────────────────────────────────────── */
void showPlaylist(Song *head) {
    if (head == NULL) {
        printf("  (Playlist is empty)\n");
        return;
    }

    int    total    = 0;
    int    pos      = 1;
    Song  *curr     = head;

    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║                    MY PLAYLIST                     ║\n");
    printf("╠══╦══════════════════════════════════╦════════════╣\n");
    printf("║# ║ Title — Artist                   ║ Duration   ║\n");
    printf("╠══╬══════════════════════════════════╬════════════╣\n");

    while (curr != NULL) {
        int min = curr->duration / 60;
        int sec = curr->duration % 60;
        char display[100];
        snprintf(display, sizeof(display), "%s - %s", curr->title, curr->artist);
            printf("║%-2d║ %-32.32s ║ %3d:%02d  (id=%d)  ║\n",
               pos, display, min, sec, curr->id);
        total += curr->duration;
        curr   = curr->next;
        pos++;
    }

    printf("╠══╩══════════════════════════════════╩════════════╣\n");
    printf("║  Total: %d songs   |   %d:%02d total duration      ║\n",
           pos - 1, total / 60, total % 60);
    printf("╚══════════════════════════════════════════════════╝\n\n");
}

/* ─────────────────────────────────────────
   MOVE — song up or down by one position
   ───────────────────────────────────────── */
Song *moveSong(Song *head, int id, int direction) {
    /* direction: -1 = move up, +1 = move down */
    if (head == NULL || head->next == NULL) return head;

    /* Find the target song and its previous node */
    Song *prev2 = NULL;   /* node before prev */
    Song *prev  = NULL;
    Song *curr  = head;

    while (curr != NULL && curr->id != id) {
        prev2 = prev;
        prev  = curr;
        curr  = curr->next;
    }

    if (curr == NULL) {
        printf("[!] Song ID %d not found.\n", id);
        return head;
    }

    if (direction == -1) {
        /* Move UP: swap curr with prev */
        if (prev == NULL) {
            printf("[!] Song is already at the top.\n");
            return head;
        }
        /* Rewire */
        if (prev2) prev2->next = curr;
        else       head        = curr;
        prev->next = curr->next;
        curr->next = prev;
        printf("[↑] Moved \"%s\" up.\n", curr->title);

    } else {
        /* Move DOWN: swap curr with curr->next */
        Song *next = curr->next;
        if (next == NULL) {
            printf("[!] Song is already at the bottom.\n");
            return head;
        }
        if (prev) prev->next = next;
        else      head       = next;
        curr->next = next->next;
        next->next = curr;
        printf("[↓] Moved \"%s\" down.\n", curr->title);
    }
    return head;
}

/* ─────────────────────────────────────────
   FREE — release all memory
   ───────────────────────────────────────── */
void freePlaylist(Song *head) {
    Song *curr = head;
    while (curr != NULL) {
        Song *temp = curr;
        curr = curr->next;
        free(temp);
    }
    printf("[*] Playlist cleared from memory.\n");
}

/* ─────────────────────────────────────────
   COUNT — number of songs
   ───────────────────────────────────────── */
int countSongs(Song *head) {
    int count = 0;
    while (head != NULL) { count++; head = head->next; }
    return count;
}

/* ═══════════════════════════════════════════
   MAIN — Demo / Interactive Menu
   ═══════════════════════════════════════════ */
int main(void) {
    Song *playlist = NULL;
    int   choice, id, pos, dur, dir;
    char  title[100], artist[100];
    static int nextId = 1;

    printf("\n=== Playlist Manager (Linked List) ===\n");
    showPlaylist(playlist);

    /* Interactive menu */
    while (1) {
        printf("1) Add to end   2) Add to front   3) Add at position\n");
        printf("4) Delete       5) Search          6) Move up/down\n");
        printf("7) Show all     0) Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                printf("Title: ");  scanf(" %99[^\n]", title);
                printf("Artist: "); scanf(" %99[^\n]", artist);
                printf("Duration (seconds): "); scanf("%d", &dur);
                playlist = addToEnd(playlist, nextId++, title, artist, dur);
                break;

            case 2:
                printf("Title: ");  scanf(" %99[^\n]", title);
                printf("Artist: "); scanf(" %99[^\n]", artist);
                printf("Duration (seconds): "); scanf("%d", &dur);
                playlist = addToFront(playlist, nextId++, title, artist, dur);
                break;

            case 3:
                printf("Position: "); scanf("%d", &pos);
                printf("Title: ");    scanf(" %99[^\n]", title);
                printf("Artist: ");   scanf(" %99[^\n]", artist);
                printf("Duration (seconds): "); scanf("%d", &dur);
                playlist = addAtPosition(playlist, pos, nextId++, title, artist, dur);
                break;

            case 4:
                printf("Song ID to delete: "); scanf("%d", &id);
                playlist = deleteSong(playlist, id);
                break;

            case 5:
                printf("Search title: "); scanf(" %99[^\n]", title);
                searchByTitle(playlist, title);
                break;

            case 6:
                printf("Song ID: ");          scanf("%d", &id);
                printf("Direction (-1=up, +1=down): "); scanf("%d", &dir);
                playlist = moveSong(playlist, id, dir);
                break;

            case 7:
                showPlaylist(playlist);
                break;

            case 0:
                freePlaylist(playlist);
                printf("Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }

    freePlaylist(playlist);
    return 0;
}
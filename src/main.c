#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ENABLE_CORE_MODULE_MENU
#include "../include/playlist.h"
#include "../include/queue.h"
#include "../include/stack.h"
#endif

/* ─── Headers จาก module อื่น (uncomment เมื่อพร้อม) ─── */
// #include "../include/song.h"        // [คน 1] struct Song
// #include "../include/playlist.h"    // [คน 1] linked list playlist
// #include "../include/hash.h"        // [คน 1] hash table สำหรับ search
// #include "../include/api.h"         // [คน 2] iTunes API / CSV loader
// queue.c / stack.c / recommend.c ยังไม่มี header แยก ให้ declare ไว้ที่นี่ก่อน

/* ─── ANSI Color (ทำให้ terminal UI สวยขึ้น) ─── */
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[1;36m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"

/* ══════════════════════════════════════════════
 * STUB DECLARATIONS
 * ลบออกเมื่อ include header จริงแล้ว
 * ══════════════════════════════════════════════ */

/* [STUB — คน 1] playlist.c */
void uiAddSong(const char *title, const char *artist, const char *genre);
void uiDeleteSong(const char *title);
void uiShowPlaylist();

/* [STUB — คน 1] queue.c */
void uiPlayNext();

/* [STUB — คน 1] stack.c */
void uiPlayPrevious();

/* [STUB — คน 2] api.c หรือ CSV loader */
void uiSearchSong(const char *keyword);

/* [STUB — คน 2] recommend.c */
void uiShowRecommendations();

/* ══════════════════════════════════════════════
 * STUB IMPLEMENTATIONS (ชั่วคราว)
 * ลบทิ้งเมื่อ link กับ .c จริงแล้ว
 * ══════════════════════════════════════════════ */

void uiAddSong(const char *title, const char *artist, const char *genre) {
    printf(YELLOW "[STUB] addSong: title='%s', artist='%s', genre='%s'\n" RESET,
           title, artist, genre);
}

void uiDeleteSong(const char *title) {
    printf(YELLOW "[STUB] deleteSong: title='%s'\n" RESET, title);
}

void uiShowPlaylist() {
    printf(YELLOW "[STUB] showPlaylist: (ยังไม่มีข้อมูล)\n" RESET);
}

void uiPlayNext() {
    printf(YELLOW "[STUB] playNext: เล่นเพลงถัดไป\n" RESET);
}

void uiPlayPrevious() {
    printf(YELLOW "[STUB] playPrevious: ย้อนกลับเพลงก่อนหน้า\n" RESET);
}

void uiSearchSong(const char *keyword) {
    printf(YELLOW "[STUB] searchSong: keyword='%s'\n" RESET, keyword);
}

void uiShowRecommendations() {
    printf(YELLOW "[STUB] showRecommendations: (ยังไม่มี algorithm)\n" RESET);
}

#ifdef ENABLE_CORE_MODULE_MENU
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

static void runCoreModuleMenu(void) {
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
        printf("0) Back to main menu\n");
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
                return;

            default:
                printf("Invalid choice.\n");
        }
    }

    freeSongCopy(nowPlaying);
    clearQueue(&playQueue);
    clearStack(&history);
    freePlaylist(playlist);
}
#endif

/* ══════════════════════════════════════════════
 * UI HELPER FUNCTIONS
 * ══════════════════════════════════════════════ */

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printDivider() {
    printf(CYAN "═══════════════════════════════════════\n" RESET);
}

void printHeader() {
    printDivider();
    printf(CYAN BOLD "        🎵 Smart Music Playlist Manager\n" RESET);
    printDivider();
}

void printMenu() {
    printf("\n");
    printf(GREEN "  1." RESET " Add Song\n");
    printf(GREEN "  2." RESET " Delete Song\n");
    printf(GREEN "  3." RESET " Search Song\n");
    printf(GREEN "  4." RESET " Play Next Song\n");
    printf(GREEN "  5." RESET " Previous Song\n");
    printf(GREEN "  6." RESET " Show Playlist\n");
    printf(GREEN "  7." RESET " Recommendations\n");
    printf(GREEN "  8." RESET " Toggle Shuffle [OFF]\n");
    printf(GREEN "  9." RESET " Admin mode\n");
    printf("\n");
    printf(RED   "  0." RESET " Exit\n");
    printf("\n");
    printDivider();
    printf("  Enter your choice: ");
}

/* อ่าน string จาก stdin แบบปลอดภัย (ป้องกัน buffer overflow) */
void readLine(char *buffer, int maxLen) {
    if (fgets(buffer, maxLen, stdin) != NULL) {
        /* ตัด newline ออก */
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

/* ══════════════════════════════════════════════
 * MENU HANDLERS
 * แต่ละ case เรียก function จาก module ที่เกี่ยวข้อง
 * ══════════════════════════════════════════════ */

void handleAddSong() {
    char title[100], artist[100], genre[50];

    printf("\n── Add Song ──\n");
    printf("  Title  : "); readLine(title,  sizeof(title));
    printf("  Artist : "); readLine(artist, sizeof(artist));
    printf("  Genre  : "); readLine(genre,  sizeof(genre));

    uiAddSong(title, artist, genre);  /* → playlist.c [คน 1] */

    printf(GREEN "\n  ✓ Added: %s — %s\n" RESET, title, artist);
}

void handleDeleteSong() {
    char title[100];

    printf("\n── Delete Song ──\n");
    printf("  Enter title to delete: "); readLine(title, sizeof(title));

    uiDeleteSong(title);  /* → playlist.c [คน 1] */
}

void handleSearchSong() {
    char keyword[100];

    printf("\n── Search Song ──\n");
    printf("  Keyword: "); readLine(keyword, sizeof(keyword));

    uiSearchSong(keyword);  /* → hash.c [คน 1] / api.c [คน 2] */
}

void handlePlayNext() {
    printf("\n── Play Next ──\n");
    uiPlayNext();  /* → queue.c [คน 1] */
}

void handlePlayPrevious() {
    printf("\n── Previous Song ──\n");
    uiPlayPrevious();  /* → stack.c [คน 1] */
}

void handleShowPlaylist() {
    printf("\n── Playlist ──\n");
    uiShowPlaylist();  /* → playlist.c [คน 1] */
}

void handleRecommendations() {
    printf("\n── Recommendations ──\n");
    uiShowRecommendations();  /* → recommend.c [คน 2] */
}

void handleToggleShuffle() {
}

void handleAdminMode() {
#ifdef ENABLE_CORE_MODULE_MENU
    runCoreModuleMenu();
#else
    printf(YELLOW "[STUB] Admin mode: core playlist/queue/stack modules are not merged yet.\n" RESET);
#endif
}

/* ══════════════════════════════════════════════
 * MAIN — Program Entry Point
 * ══════════════════════════════════════════════ */

int main() {
    int choice;
    char input[10];

    /*
     * TODO: init modules ที่นี่เมื่อพร้อม
     * initHashTable();   // hash.c  [คน 1]
     * loadFromCSV();     // api.c   [คน 2]
     */

    while (1) {
        clearScreen();
        printHeader();
        printMenu();

        readLine(input, sizeof(input));

        /* แปลง string → int อย่างปลอดภัย */
        if (sscanf(input, "%d", &choice) != 1) {
            choice = -1;  /* input ไม่ใช่ตัวเลข */
        }

        printf("\n");

        switch (choice) {
            case 1: handleAddSong();         break;
            case 2: handleDeleteSong();      break;
            case 3: handleSearchSong();      break;
            case 4: handlePlayNext();        break;
            case 5: handlePlayPrevious();    break;
            case 6: handleShowPlaylist();    break;
            case 7: handleRecommendations(); break;
            case 8: handleToggleShuffle();   break;
            case 9: handleAdminMode();       break;
            case 0:
                printf(CYAN "  Goodbye! 🎵\n" RESET);
                /*
                 * TODO: cleanup ที่นี่เมื่อพร้อม
                 * freePlaylist();
                 * freeHashTable();
                 */
                return 0;
            default:
                printf(RED "  ✗ Invalid choice. Please try again.\n" RESET);
                break;
        }

        /* รอ user กด Enter ก่อนกลับ menu */
        printf("\n  Press Enter to continue...");
        getchar();
    }

    return 0;
}

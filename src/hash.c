/*
 * hash.c
 * Smart Music Playlist Manager
 *
 * Hash Table สำหรับ search เพลงจาก title + artist
 * ใช้ Separate Chaining แก้ collision
 *
 * เหตุผลที่ใช้ Hash Table:
 *   - search แบบปกติ (linear) = O(n) ยิ่งเพลงเยอะยิ่งช้า
 *   - hash table = O(1) เฉลี่ย เร็วกว่ามากเมื่อเพลงเยอะ
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Windows ไม่มี strcasecmp — ใช้ _stricmp แทน */
#ifdef _WIN32
    #define strcasecmp  _stricmp
    #define strncasecmp _strnicmp
#endif

#include "../include/hash.h"
#include "song.h"

/* ══════════════════════════════════════════════
 * HASH FUNCTION
 * ใช้ djb2 algorithm — กระจายข้อมูลได้ดี collision น้อย
 * ══════════════════════════════════════════════ */
static unsigned int hashFunction(const char *key) {
    unsigned int hash = 5381;
    int c;
    while ((c = *key++)) {
        /* แปลงเป็น lowercase ก่อน hash ทำให้ search case-insensitive */
        hash = ((hash << 5) + hash) + tolower(c);
    }
    return hash % HASH_SIZE;
}

/* ══════════════════════════════════════════════
 * INIT
 * ══════════════════════════════════════════════ */
void hashInit(HashTable *ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        ht->buckets[i] = NULL;
    }
    ht->count = 0;
}

/* ══════════════════════════════════════════════
 * INSERT
 * เพิ่มเพลงลง hash table โดย hash จาก title
 * ══════════════════════════════════════════════ */
void hashInsert(HashTable *ht, const Song *song) {
    unsigned int index = hashFunction(song->title);

    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    if (!newNode) {
        fprintf(stderr, "[hash] Error: malloc failed\n");
        return;
    }

    newNode->song = *song;  /* copy struct ทั้งหมด */
    newNode->next = ht->buckets[index];
    ht->buckets[index] = newNode;
    ht->count++;
}

/* ══════════════════════════════════════════════
 * SEARCH BY TITLE
 * return pointer ถ้าเจอ, NULL ถ้าไม่เจอ
 * ══════════════════════════════════════════════ */
Song *hashSearchByTitle(HashTable *ht, const char *title) {
    unsigned int index = hashFunction(title);
    HashNode *current = ht->buckets[index];

    while (current) {
        if (strcasecmp(current->song.title, title) == 0) {
            return &current->song;
        }
        current = current->next;
    }
    return NULL;
}

/* ══════════════════════════════════════════════
 * SEARCH BY ARTIST
 * ต้อง scan ทั้ง table เพราะ hash จาก title
 * return pointer อันแรกที่เจอ
 * ══════════════════════════════════════════════ */
Song *hashSearchByArtist(HashTable *ht, const char *artist) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *current = ht->buckets[i];
        while (current) {
            if (strcasecmp(current->song.artist, artist) == 0) {
                return &current->song;
            }
            current = current->next;
        }
    }
    return NULL;
}

/* ══════════════════════════════════════════════
 * HELPER: containsIgnoreCase
 * strcasestr ไม่มีใน Windows — เขียนเองแทน
 * ══════════════════════════════════════════════ */
static int containsIgnoreCase(const char *haystack, const char *needle) {
    if (!haystack || !needle) return 0;
    char h[256], n[256];
    size_t i;
    for (i = 0; haystack[i] && i < 255; i++) h[i] = (char)tolower((unsigned char)haystack[i]);
    h[i] = '\0';
    for (i = 0; needle[i] && i < 255; i++) n[i] = (char)tolower((unsigned char)needle[i]);
    n[i] = '\0';
    return strstr(h, n) != NULL;
}

/* ══════════════════════════════════════════════
 * SEARCH ALL (title + artist)
 * แสดงผลทุกเพลงที่ keyword ตรงกับ title หรือ artist
 * ══════════════════════════════════════════════ */
void hashSearchAll(HashTable *ht, const char *keyword) {
    int found = 0;

    printf("\n  Search results for \"%s\":\n", keyword);
    printf("  ─────────────────────────────────────\n");

    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *current = ht->buckets[i];
        while (current) {
            /* ตรวจทั้ง title และ artist (case-insensitive) */
            if (containsIgnoreCase(current->song.title,  keyword) ||
                containsIgnoreCase(current->song.artist, keyword)) {
                songPrint(&current->song, found + 1);
                found++;
            }
            current = current->next;
        }
    }

    if (found == 0) {
        printf("  No songs found.\n");
    } else {
        printf("  ─────────────────────────────────────\n");
        printf("  Found %d song(s)\n", found);
    }
}

/* ══════════════════════════════════════════════
 * DELETE
 * ลบเพลงออกจาก hash table โดย title
 * ══════════════════════════════════════════════ */
void hashDelete(HashTable *ht, const char *title) {
    unsigned int index = hashFunction(title);
    HashNode *current  = ht->buckets[index];
    HashNode *prev     = NULL;

    while (current) {
        if (strcasecmp(current->song.title, title) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                ht->buckets[index] = current->next;
            }
            free(current);
            ht->count--;
            printf("  ✓ Deleted \"%s\" from hash table\n", title);
            return;
        }
        prev    = current;
        current = current->next;
    }
    printf("  Song \"%s\" not found in hash table\n", title);
}

/* ══════════════════════════════════════════════
 * FREE
 * คืน memory ทั้งหมดตอน exit
 * ══════════════════════════════════════════════ */
void hashFree(HashTable *ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *current = ht->buckets[i];
        while (current) {
            HashNode *next = current->next;
            free(current);
            current = next;
        }
        ht->buckets[i] = NULL;
    }
    ht->count = 0;
}

/* ══════════════════════════════════════════════
 * DEBUG: PRINT STATS
 * ดูว่า table กระจายข้อมูลดีแค่ไหน
 * ══════════════════════════════════════════════ */
void hashPrintStats(HashTable *ht) {
    int used    = 0;
    int maxChain = 0;

    for (int i = 0; i < HASH_SIZE; i++) {
        if (ht->buckets[i]) {
            used++;
            int chainLen = 0;
            HashNode *current = ht->buckets[i];
            while (current) {
                chainLen++;
                current = current->next;
            }
            if (chainLen > maxChain) maxChain = chainLen;
        }
    }

    printf("\n  [Hash Table Stats]\n");
    printf("  Total songs  : %d\n", ht->count);
    printf("  Buckets used : %d / %d\n", used, HASH_SIZE);
    printf("  Max chain    : %d\n", maxChain);
    printf("  Load factor  : %.2f\n", (float)ht->count / HASH_SIZE);
}

#ifndef HASH_H
#define HASH_H

#include "song.h"

/* ─── Hash Table Config ─── */
#define HASH_SIZE 101  /* ขนาด table ควรเป็นเลขเฉพาะ ลด collision */

/* ─── Node ใน Hash Table (Chaining) ─── */
typedef struct HashNode {
    Song song;
    struct HashNode *next;  /* linked list สำหรับ collision */
} HashNode;

/* ─── Hash Table ─── */
typedef struct {
    HashNode *buckets[HASH_SIZE];
    int count;
} HashTable;

/* ─── Public Functions ─── */
void     hashInit(HashTable *ht);
void     hashInsert(HashTable *ht, const Song *song);
Song    *hashSearchByTitle(HashTable *ht, const char *title);
Song    *hashSearchByArtist(HashTable *ht, const char *artist);
void     hashSearchAll(HashTable *ht, const char *keyword); /* search ทั้ง title + artist */
void     hashDelete(HashTable *ht, const char *title);
void     hashFree(HashTable *ht);
void     hashPrintStats(HashTable *ht); /* debug: ดู load ของ table */

#endif

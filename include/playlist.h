#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "song.h"

Song *playlistAddEnd(Song *head, const Song *song);
Song *playlistDeleteById(Song *head, const char *id);
Song *playlistFindById(Song *head, const char *id);
Song *playlistFindByTitle(Song *head, const char *title);
void playlistShow(Song *head);
void playlistFree(Song *head);
int playlistCount(Song *head);
int playlistToArray(Song *head, Song songs[], int max_songs);
Song *playlistFromArray(Song songs[], int count);

#endif

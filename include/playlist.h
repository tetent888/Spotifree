#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "song.h"

Song *addToEnd(Song *head, int id, const char *title,
               const char *artist, int duration);
Song *addToFront(Song *head, int id, const char *title,
                 const char *artist, int duration);
Song *deleteSong(Song *head, int id);
Song *moveSong(Song *head, int id, int direction);
void showPlaylist(Song *head);
void freePlaylist(Song *head);
int countSongs(Song *head);

#endif

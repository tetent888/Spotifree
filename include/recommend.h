#ifndef RECOMMEND_H
#define RECOMMEND_H

#include "song.h"

int recommendPickRandomSong(Song *out_song);
int recommendPickSong(Song *out_song);
void showRecommendations(void);

#endif

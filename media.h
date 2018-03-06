/*
 * This file is part of TONG.              http://www.nongnu.org/tong
 * Copyright 2004, 2007 Owen Swerkstrom
 *
 * TONG is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tong is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MEDIA_H__
#define __MEDIA_H__

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "option.h"

#ifdef GP2X
#define SIZERATIO 2
#else
#define SIZERATIO 1
#endif

#ifdef WII
#define SFXEXT "wav"
#else
#define SFXEXT "ogg"
#endif

extern void draw_splash(SDL_Surface*);
extern void draw_bg(bool, SDL_Surface*, Option*);
extern void draw_title(SDL_Surface*);
extern void draw_gameover(SDL_Surface*);
extern void draw_gameover(SDL_Surface*, Uint32);
extern void draw_fade(Uint8, SDL_Surface*);
extern void init_audio(Option*, bool, Uint32&);
extern SDL_Surface* init_video(int, int);
extern void load_graphics();
extern void load_graphics(const char*);
SDL_Surface* read_image(const char*, bool);
extern void theme(const char*);
extern void safe_blit(SDL_Surface*, SDL_Surface*, SDL_Rect);

#endif

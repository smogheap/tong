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

#include <stdio.h>

#include "media.h"
#include "tetris.h"

extern SDL_Surface *black, *back, *zone, *temp, *title, *scorebg, *timebg;
extern SDL_Surface *timeup, *nextbg, *rulebg, *ballbounce, *ballignore;
extern SDL_Surface *balljoin, *balldie, *tetradmove, *tetradignore;
extern SDL_Surface *tetradjoin, *tetradbreak, *tetraddie, *tetradrotate;
extern SDL_Surface *tetraddrop, *stackbreak, *stackignore, *stackjoin;
extern SDL_Surface *stackdie, *stacklinebreak, *gameover, *youlose;
extern SDL_Surface *icon, *splash, *splashbehind;
extern Mix_Music *music;
extern Mix_Chunk *toing, *newrule, *twobits;
extern Mix_Chunk *rotate1, *rotate2, *plop, *bust, *tick;
extern Mix_Chunk *bounce1, *bounce2;

void draw_splash(SDL_Surface* surf) {
	SDL_Rect dest = {0, 0, 0, 0};
	dest.x = 192 / SIZERATIO;
	dest.y = (224 / SIZERATIO) - ((SDL_GetTicks() / 10) % (256 / SIZERATIO));
	safe_blit(splashbehind, surf, dest);
	dest.y = (224 / SIZERATIO) - ((SDL_GetTicks() / 10) % (256 / SIZERATIO)) +
		(256 / SIZERATIO);
	safe_blit(splashbehind, surf, dest);
	dest.x = 0;
	dest.y = 0;
	safe_blit(splash, surf, dest);
}

void draw_bg(bool drawgameboard, SDL_Surface* surf, Option* ops) {
	SDL_Rect dest = {0, 0, 0, 0};
	dest.x = 0;
	if(ops->GetBgScroll()) {
		dest.y = ((SDL_GetTicks() / 80) % 480) / SIZERATIO;
		safe_blit(back, surf, dest);
		dest.x = 0;
		dest.y = (((SDL_GetTicks() / 80) % 480) - 480) / SIZERATIO;
		safe_blit(back, surf, dest);
	} else {
		dest.y = 0;
		safe_blit(back, surf, dest);
	}
	if(drawgameboard) {
		dest.x = 224 / SIZERATIO;
		dest.y = 64 / SIZERATIO;
		safe_blit(zone, surf, dest);
		dest.x = 448 / SIZERATIO;
		dest.y = 64 / SIZERATIO;
		safe_blit(scorebg, surf, dest);
		dest.x = 64 / SIZERATIO;
		safe_blit(timebg, surf, dest);

		//ball+tetrad rule
		dest.x = 64 / SIZERATIO;
		dest.y = 144 / SIZERATIO;
		safe_blit(rulebg, surf, dest);
		if(ops->GetRuleBallTetrad() == BALL_JOINS_TETRAD) {
			safe_blit(balljoin, surf, dest);
		} else {
			switch(ops->GetRuleTetradBall()) {
			case NO_REACTION:
				safe_blit(ballignore, surf, dest);
				break;
			case TETRAD_KILLS_BALL:
				safe_blit(balldie, surf, dest);
				break;
			case TETRAD_BOUNCES_BALL:
			default:
				safe_blit(ballbounce, surf, dest);
				break;
			}
		}
		dest.y = 208 / SIZERATIO;
		switch(ops->GetRuleBallTetrad()) {
		case NO_REACTION:
			safe_blit(tetradignore, surf, dest);
			break;
		case BALL_ROTATES_TETRAD:
			safe_blit(tetradrotate, surf, dest);
			break;
		case BALL_BREAKS_TETRAD:
			safe_blit(tetradbreak, surf, dest);
			break;
		case BALL_DROPS_TETRAD:
			safe_blit(tetraddrop, surf, dest);
			break;
		case BALL_KILLS_TETRAD:
			safe_blit(tetraddie, surf, dest);
			break;
		case BALL_JOINS_TETRAD:
			safe_blit(tetradjoin, surf, dest);
			break;
		case BALL_MOVES_TETRAD:
		default:
			safe_blit(tetradmove, surf, dest);
			break;
		}

		//ball+stack rule
		dest.y = 288 / SIZERATIO;
		safe_blit(rulebg, surf, dest);
		if(ops->GetRuleBallStack() == BALL_JOINS_STACK) {
			safe_blit(balljoin, surf, dest);
		} else {
			switch(ops->GetRuleStackBall()) {
			case NO_REACTION:
				safe_blit(ballignore, surf, dest);
				break;
			case STACK_KILLS_BALL:
				safe_blit(balldie, surf, dest);
				break;
			case STACK_BOUNCES_BALL:
			default:
				safe_blit(ballbounce, surf, dest);
				break;
			}
		}
		dest.y = 352 / SIZERATIO;
		switch(ops->GetRuleBallStack()) {
		case NO_REACTION:
			safe_blit(stackignore, surf, dest);
			break;
		case BALL_LINEBREAK_STACK:
			safe_blit(stacklinebreak, surf, dest);
			break;
		case BALL_KILLS_STACK:
			safe_blit(stackdie, surf, dest);
			break;
		case BALL_JOINS_STACK:
			safe_blit(stackjoin, surf, dest);
			break;
		case BALL_BREAKS_STACK:
		default:
			safe_blit(stackbreak, surf, dest);
			break;
		}

		//next tetrad
		dest.x = PREVIEW_OFFSET_X - (16 / SIZERATIO);
		dest.y = PREVIEW_OFFSET_Y - (16 / SIZERATIO);
		safe_blit(nextbg, surf, dest);
	}
}

void draw_title(SDL_Surface* surf) {
	SDL_Rect dest = {0, 0, 0, 0};
	dest.x = 166 / SIZERATIO;
	dest.y = 64 / SIZERATIO;
	safe_blit(title, surf, dest);
}

void draw_gameover(SDL_Surface* surf) {
	SDL_Rect dest = {0, 0, 0, 0};
	dest.x = 160 / SIZERATIO;
	dest.y = 96 / SIZERATIO;
	safe_blit(timeup, surf, dest);
}

void draw_gameover(SDL_Surface* surf, Uint32 timespent) {
	SDL_Rect dest = {0, 0, 0, 0};
	dest.x = 160 / SIZERATIO;
	dest.y = 96 / SIZERATIO;
	if(timespent < 15000) {
		safe_blit(youlose, surf, dest);
	} else {
		safe_blit(gameover, surf, dest);
	}
}

SDL_Surface* init_video(int bpp, int videoflags) {
	if(!SDL_WasInit(SDL_INIT_VIDEO)) {
		SDL_Init(SDL_INIT_VIDEO);
	}
	SDL_Surface* screen;
#ifdef GP2X
	screen = SDL_SetVideoMode(320, 240, 16, videoflags & ~SDL_FULLSCREEN);
#else
	screen = SDL_SetVideoMode(640, 480, bpp, videoflags);
#endif
	if(!screen) {
		printf("Unable to set video: %s\n", SDL_GetError());
		exit(1);
	}

	return screen;
}

void init_audio(Option* ops, bool startsong, Uint32& musicstarted) {
	if(music) {
		Mix_FreeMusic(music);
	}
	if(rotate1) {
		Mix_FreeChunk(rotate1);
	}
	if(rotate2) {
		Mix_FreeChunk(rotate2);
	}
	if(plop) {
		Mix_FreeChunk(plop);
	}
	if(bust) {
		Mix_FreeChunk(bust);
	}
	if(tick) {
		Mix_FreeChunk(tick);
	}
	if(bounce1) {
		Mix_FreeChunk(bounce1);
	}
	if(bounce2) {
		Mix_FreeChunk(bounce2);
	}
	if(toing) {
		Mix_FreeChunk(toing);
	}
	if(newrule) {
		Mix_FreeChunk(newrule);
	}
	if(twobits) {
		Mix_FreeChunk(twobits);
	}
	Mix_CloseAudio();
	if(!Mix_OpenAudio(ops->GetAudioFreq(), MIX_DEFAULT_FORMAT, 2,
					  ops->GetAudioBuffers())) {
		Mix_AllocateChannels(3); //one sfx channels for tetris, one for pong,
		//one for "new rule" and other such noises
		rotate1=Mix_LoadWAV("media/swish1." SFXEXT);
		if(!rotate1) {
			printf("couldn't load sfx swish1.\n");
		}
		rotate2=Mix_LoadWAV("media/swish2." SFXEXT);
		if(!rotate2) {
			printf("couldn't load sfx swish2.\n");
		}
		plop=Mix_LoadWAV("media/plop1." SFXEXT);
		if(!plop) {
			printf("couldn't load sfx plop.\n");
		}
		bust=Mix_LoadWAV("media/wee1." SFXEXT);
		if(!bust) {
			printf("couldn't load sfx bust.\n");
		}
		tick=Mix_LoadWAV("media/tick." SFXEXT);
		if(!tick) {
			printf("couldn't load sfx tick.\n");
		}
		bounce1=Mix_LoadWAV("media/bounce1." SFXEXT);
		if(!bounce1) {
			printf("couldn't load sfx bounce1.\n");
		}
		bounce2=Mix_LoadWAV("media/bounce2." SFXEXT);
		if(!bounce2) {
			printf("couldn't load sfx bounce2.\n");
		}
		toing=Mix_LoadWAV("media/toing." SFXEXT);
		if(!toing) {
			printf("couldn't load sfx toing.\n");
		}
		newrule=Mix_LoadWAV("media/newrule." SFXEXT);
		if(!newrule) {
			printf("couldn't load sfx newrule.\n");
		}
		twobits=Mix_LoadWAV("media/twobits." SFXEXT);
		if(!twobits) {
			printf("couldn't load sfx twobits.\n");
		}

		if(ops->GetMusic() != MUSIC_NONE) {
			if(ops->GetCurrentMusicFile()) {
				if((music=Mix_LoadMUS(ops->GetCurrentMusicFile()))) {
					if(startsong) {
						if(ops->GetMusic()==MUSIC_SHUFFLE) {
							Mix_PlayMusic(music, 1);
							musicstarted=SDL_GetTicks();
						} else {
							Mix_PlayMusic(music, -1);
							musicstarted=SDL_GetTicks();
						}
					} else {
						if(ops->GetMusic()==MUSIC_SHUFFLE) {
							Mix_FadeInMusicPos(music, -1, 500,
											   (SDL_GetTicks() -
												musicstarted)/1000);
						} else {
							if(music) {
								Mix_FadeInMusicPos(music, -1, 500,
												   (SDL_GetTicks() -
													musicstarted)/1000);
							}
						}
					}
				} else {
					printf("couldn't load music: %s\n", Mix_GetError());
				}
			}
		} else {
			printf("config said no music\n");
		}
	} else {
		printf("Couldn't init sound: %s\n", Mix_GetError());
	}
	return;
}

void load_graphics() {
	load_graphics("");
}

void load_graphics(const char* theme) {
	SDL_Surface* pieces = NULL;
	char filename[80];
	Uint32 rmask, gmask, bmask, amask;
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	} else {
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	}
	SDL_Rect src = {0, 0, 16 / SIZERATIO, 16 / SIZERATIO};
	SDL_Rect dest = {0, 0, 0, 0};
#ifndef GP2X
	int i = 0;
	int j = 0;
	int blocks[3][5] = {{RED, RED, BLUE, BLUE, GREEN},
						{YELLOW, YELLOW, BLUE, BLUE, GREEN},
						{CYAN, YELLOW, YELLOW, GREEN, GREEN}};
#endif
	sprintf(filename, "media/pieces%s.png", theme);
	pieces = read_image(filename, true);

	if(temp) {
		SDL_FreeSurface(temp);
	}
	if(black) {
		SDL_FreeSurface(black);
	}
	if(back) {
		SDL_FreeSurface(back);
	}
	if(zone) {
		SDL_FreeSurface(zone);
	}
	if(title) {
		SDL_FreeSurface(title);
	}
	if(scorebg) {
		SDL_FreeSurface(scorebg);
	}
	if(timebg) {
		SDL_FreeSurface(timebg);
	}
	if(rulebg) {
		SDL_FreeSurface(rulebg);
	}
	if(ballbounce) {
		SDL_FreeSurface(ballbounce);
	}
	if(ballignore) {
		SDL_FreeSurface(ballignore);
	}
	if(balljoin) {
		SDL_FreeSurface(balljoin);
	}
	if(balldie) {
		SDL_FreeSurface(balldie);
	}
	if(tetradmove) {
		SDL_FreeSurface(tetradmove);
	}
	if(tetradignore) {
		SDL_FreeSurface(tetradignore);
	}
	if(tetradjoin) {
		SDL_FreeSurface(tetradjoin);
	}
	if(tetradbreak) {
		SDL_FreeSurface(tetradbreak);
	}
	if(tetraddie) {
		SDL_FreeSurface(tetraddie);
	}
	if(tetradrotate) {
		SDL_FreeSurface(tetradrotate);
	}
	if(tetraddrop) {
		SDL_FreeSurface(tetraddrop);
	}
	if(stackbreak) {
		SDL_FreeSurface(stackbreak);
	}
	if(stackignore) {
		SDL_FreeSurface(stackignore);
	}
	if(stackjoin) {
		SDL_FreeSurface(stackjoin);
	}
	if(stackdie) {
		SDL_FreeSurface(stackdie);
	}
	if(stacklinebreak) {
		SDL_FreeSurface(stacklinebreak);
	}
	if(nextbg) {
		SDL_FreeSurface(nextbg);
	}
	if(gameover) {
		SDL_FreeSurface(gameover);
	}
	if(youlose) {
		SDL_FreeSurface(youlose);
	}
	if(icon) {
		SDL_FreeSurface(icon);
	}
	if(splash) {
		SDL_FreeSurface(splash);
	}
	if(splashbehind) {
		SDL_FreeSurface(splashbehind);
	}
	black = read_image("media/black.png", false);
	back = read_image("media/background.png", true);
	zone = read_image("media/playzone.png", true);
	title = read_image("media/tong.png", true);
	scorebg = read_image("media/score.png", true);
	timebg = read_image("media/time.png", true);
	timeup = read_image("media/timeup.png", true);
	rulebg = read_image("media/rulespark.png", true);

	src.x = 0;
	src.y = 48 / SIZERATIO;
	ballbounce = read_image("media/ball-bounce.png", true);
	dest.x = 83 / SIZERATIO;
	dest.y = 26 / SIZERATIO;
	SDL_BlitSurface(pieces, &src, ballbounce, &dest);
	ballignore = read_image("media/ball-ignore.png", true);
	dest.x = 56 / SIZERATIO;
	dest.y = 48 / SIZERATIO;
	SDL_BlitSurface(pieces, &src, ballignore, &dest);
	balljoin = read_image("media/ball-join.png", true);
	src.h = 8 / SIZERATIO;
	SDL_BlitSurface(pieces, &src, balljoin, &dest);
	src.x = 112 / SIZERATIO;
	src.y = 8 / SIZERATIO;
	dest.y = 56 / SIZERATIO;
	SDL_BlitSurface(pieces, &src, balljoin, &dest);
	balldie=read_image("media/ball-die.png", true);
	src.h = 16 / SIZERATIO;

	tetradmove = read_image("media/tetrad-move.png", true);
#ifndef GP2X
	src.x = src.y = 0;
	dest.y = 16 / SIZERATIO;
	for(i = 0; i < 3; i++) {
		dest.x = (40 + (i * 16)) / SIZERATIO;
		SDL_BlitSurface(pieces, &src, tetradmove, &dest);
	}
	dest.x = 40 / SIZERATIO;
	dest.y = 32 / SIZERATIO;
	SDL_BlitSurface(pieces, &src, tetradmove, &dest);
#endif

	tetradignore = read_image("media/tetrad-ignore.png", true);
#ifndef GP2X
	dest.y = 0;
	for(i = 0; i < 3; i++) {
		dest.x = (40 + (i * 16)) / SIZERATIO;
		SDL_BlitSurface(pieces, &src, tetradignore, &dest);
	}
	dest.x = 40 / SIZERATIO;
	dest.y = 16 / SIZERATIO;
	SDL_BlitSurface(pieces, &src, tetradignore, &dest);
#endif

	tetradjoin = read_image("media/tetrad-join.png", true);
#ifndef GP2X
	dest.y = 0;
	for(i = 0; i < 3; i++) {
		dest.x = (40 + (i * 16)) / SIZERATIO;
		SDL_BlitSurface(pieces, &src, tetradjoin, &dest);
	}
	dest.x = 40 / SIZERATIO;
	dest.y = 16 / SIZERATIO;
	SDL_BlitSurface(pieces, &src, tetradjoin, &dest);
#endif

	tetradbreak = read_image("media/tetrad-break.png", true);
#ifndef GP2X
	dest.x = 40 / SIZERATIO;
	dest.y = 16 / SIZERATIO;
	SDL_BlitSurface(pieces, &src, tetradbreak, &dest);
	dest.y = 0;
	SDL_BlitSurface(pieces, &src, tetradbreak, &dest);
	dest.x = 72 / SIZERATIO;
	SDL_BlitSurface(pieces, &src, tetradbreak, &dest);
#endif

	tetraddie = read_image("media/tetrad-die.png", true);
	tetradrotate = read_image("media/tetrad-rotate.png", true);
#ifndef GP2X
	dest.x = 56 / SIZERATIO;
	for(i = 0; i < 3; i++) {
		dest.y = (i * 16) / SIZERATIO;
		SDL_BlitSurface(pieces, &src, tetradrotate, &dest);
	}
	dest.x = 72 / SIZERATIO;
	dest.y = 0;
	SDL_BlitSurface(pieces, &src, tetradrotate, &dest);
#endif

	tetraddrop = read_image("media/tetrad-drop.png", true);
#ifndef GP2X
	dest.y = 28 / SIZERATIO;
	for(i = 0; i < 3; i++) {
		dest.x = (40 + (i * 16)) / SIZERATIO;
		SDL_BlitSurface(pieces, &src, tetraddrop, &dest);
	}
	dest.x = 40 / SIZERATIO;
	dest.y = 44 / SIZERATIO;
	SDL_BlitSurface(pieces, &src, tetraddrop, &dest);
#endif

	stackbreak = read_image("media/stack-break.png", true);
#ifndef GP2X
	src.y = 16 / SIZERATIO;
	for(i = 0; i < 5; i++){
		for(j = 0; j < 3; j++) {
			if(i != 2 || j != 0) {
				src.x = (blocks[j][i] * 16) / SIZERATIO;
				dest.x = (24 + (i * 16)) / SIZERATIO;
				dest.y = (j * 16) / SIZERATIO;
				SDL_BlitSurface(pieces, &src, stackbreak, &dest);
			}
		}
	}
#endif
	stackignore = read_image("media/stack-ignore.png", true);
#ifndef GP2X
	src.y = 16 / SIZERATIO;
	for(i = 0; i < 5; i++){
		for(j = 0; j < 3; j++) {
			src.x = (blocks[j][i] * 16) / SIZERATIO;
			dest.x = (24 + (i * 16)) / SIZERATIO;
			dest.y = (j * 16) / SIZERATIO;
			SDL_BlitSurface(pieces, &src, stackignore, &dest);
		}
	}
#endif
	stackjoin = read_image("media/stack-join.png", true);
#ifndef GP2X
	src.y = 16;
	for(i = 0; i < 5; i++){
		for(j = 0; j < 3; j++) {
			src.x = (blocks[j][i] * 16) / SIZERATIO;
			dest.x = (24 + (i * 16)) / SIZERATIO;
			dest.y = (j * 16) / SIZERATIO;
			SDL_BlitSurface(pieces, &src, stackjoin, &dest);
		}
	}
#endif
	stackdie = read_image("media/stack-die.png", true);
	stacklinebreak = read_image("media/stack-linebreak.png", true);
#ifndef GP2X
	src.y = 16 / SIZERATIO;
	for(i = 0; i < 5; i++){
		for(j = 1; j < 3; j++) {
			src.x = (blocks[j][i] * 16) / SIZERATIO;
			dest.x = (24 + (i * 16)) / SIZERATIO;
			dest.y = (j * 16) / SIZERATIO;
			SDL_BlitSurface(pieces, &src, stacklinebreak, &dest);
		}
	}
#endif
	nextbg = read_image("media/next.png", true);
	icon = read_image("media/icon.png", true);
	gameover = read_image("media/gameover.png", true);
	youlose = read_image("media/youlose.png", true);
	splash = read_image("media/penduincode.png", true);
	splashbehind = read_image("media/behind.png", true);

	SDL_FreeSurface(pieces);
	return;
}

SDL_Surface* read_image(const char* filename, bool alpha) {
	SDL_Surface *temp, *result;
	temp = IMG_Load(filename);
	if(temp) {
		if(alpha) {
			result = SDL_DisplayFormatAlpha(temp);
		} else {
			result = SDL_DisplayFormat(temp);
		}
		SDL_FreeSurface(temp);
	} else {
		printf("Couldn't load image %s.\n", filename);
		result = NULL;
	}
	return result;
}

void theme(const char* theme) {
	char filename[80];
	load_graphics(theme);
	if(back) {
		SDL_FreeSurface(back);
	}
	if(zone) {
		SDL_FreeSurface(zone);
	}
	sprintf(filename, "media/background%s.png", theme);
	back = read_image(filename, true);
	sprintf(filename, "media/playzone%s.png", theme);
	zone = read_image(filename, true);

	return;
}

void safe_blit(SDL_Surface *src, SDL_Surface *dest, SDL_Rect coords) {
	if(SDL_BlitSurface(src, NULL, dest, &coords)) {
		load_graphics();
		SDL_BlitSurface(src, NULL, dest, &coords);
		printf("encountered weirdness drawing to %d,%d\n", coords.x, coords.y);
	}
	return;
}

void draw_fade(Uint8 alpha, SDL_Surface *surf) {
	SDL_Rect dest={0, 0, 0, 0};
	SDL_SetAlpha(black, SDL_SRCALPHA|SDL_RLEACCEL, alpha);
	safe_blit(black, surf, dest);
	return;
}

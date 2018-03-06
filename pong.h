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

#ifndef __PONG_H__
#define __PONG_H__

class Ball;
class Paddle;

#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_image.h"

#include "tetris.h"
#include "option.h"
#include "media.h"

#define PI 3.14159265

#define BOUND_TOP    ( 5 * TILESIZE)
#define BOUND_LEFT   (15 * TILESIZE)
#define BOUND_RIGHT  (25 * TILESIZE)
#define BOUND_BOTTOM (25 * TILESIZE)

#define NEWBALL_NOTICE 500  //half a second for a new ball to "appear"

#define OUT_OF_BOUNDS 0
#define MOVED         1
#define BOUNCED       2


class Ball {
 private:
	int ballstatus;
	int trappedbounces;
	float x, y;
	float angle; //0~360, but sin() takes radians so sin(angle*PI/180)
	float speed;    //pixels per second
	float startspeed;
	Uint32 lastmove, startmoving;
	bool visible;
	bool tetrisreacted;
	SDL_Surface *pongbits;
	SDL_Rect ballrect, destrect;
	void ReactWithStack(int, int, int, Bucket*, Option*);
	void ReactWithTetrad(int, int, int, Tetrad*, Tetrad*, Bucket*, bool*,
						 Option*);
	void LoadGraphics();
 public:
	Ball();
	~Ball();
	void Init(float);
	void Theme(const char*);
	void UpdateTime();
	int Move(Paddle*, Bucket*, Tetrad*, Tetrad*, bool*, Option*);
	void Bounce(int direction);
	void Draw(SDL_Surface*);
};

class Paddle {
 private:
	int x, y;
	int xsize, ysize;
	float xspeed, yspeed;
	SDL_Surface *pongbits;
	SDL_Rect paddlerect, destrect;
	Uint32 lastdraw;
	void LoadGraphics();
 public:
	Paddle();
	~Paddle();
	void Init(int, int);
	void Theme(const char*);
	bool MouseMoved(int, int);
	bool MoveX(int);
	bool MoveY(int);
	float GetXSpeed();
	float GetYSpeed();
	bool IsAtX(int);
	bool IsAtY(int);
	void Draw(SDL_Surface*);
};

int pong2tetris(int, int);
int tetris2pongx(int);
int tetris2pongy(int);

#endif

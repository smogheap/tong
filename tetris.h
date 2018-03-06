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

#ifndef __TETRIS_H__
#define __TETRIS_H__

class Tetrad;  //the tetris pieces you rotate around
class Bucket;  //the tetris playing area

#include <stdlib.h>
#include <stdio.h>

#include "SDL.h"
#include "SDL_image.h"

#define SHAPE_L 0
#define SHAPE_Z 1
#define SHAPE_S 2
#define SHAPE_J 3
#define SHAPE_O 4
#define SHAPE_I 5
#define SHAPE_T 6

#define COLORMASK 7 //== WHITE|RED|GREEN|BLUE|YELLOW|CYAN|MAGENTA
                    //what?!  no it doesn't!  those aren't bits...

#define ORANGE  0
#define RED     1
#define GREEN   2
#define BLUE    3
#define YELLOW  4
#define CYAN    5
#define MAGENTA 6
#define WHITE   7

#define EMPTY 0
#define STACK 16
#define BREAK 32

#define BREAK_TIME       300  //time it takes a line to break in milliseconds

#ifdef GP2X
#define TILESIZE 8
#else
#define TILESIZE 16
#endif

#define OFFSET_X         (15 * TILESIZE)
#define OFFSET_Y         ( 5 * TILESIZE)  //offsets for the tetris game board
#define PREVIEW_OFFSET_X (30 * TILESIZE)
#define PREVIEW_OFFSET_Y (11 * TILESIZE)  //offsets for "next tetrad" preview


class Tetrad {
 private:
	int type;
	int piece[8];
	int pieces;
	SDL_Surface *tetrisbits;
	SDL_Rect tetradrect, shadowrect, destrect;
	int color;
	int moves;
	bool haslanded;
	void LoadGraphics();
 public:
	Tetrad();
	~Tetrad();
	void Init(int);
	void Theme(const char*);
	int GetType();
	bool MoveLeft(Bucket*);
	bool MoveRight(Bucket*);
	bool MoveDown(Bucket*, bool);
	bool MoveUp(Bucket*);
	bool Rotate(Bucket*, bool);
	bool IsAt(int);
	bool AddBlock(int);
	void DelBlock(int);
	bool HasLanded();
	void Draw(Bucket*, SDL_Surface*);
	void Preview(SDL_Surface*);
	int GetMoves();
};

class Bucket {
 private:
	int area[200];
	int breakrowmin, breakrowmax;  //TODO:  necessary? darn breakout rules...
	Uint32 breakstart;
	SDL_Surface *tetrisbits;
	SDL_Rect stackrect, destrect;
	int Drop();
	void LoadGraphics();
 public:
	Bucket();
	~Bucket();
	void Init(int);
	void Theme(const char*);
	void GreyOut();
	void AddJunk();
	void StartOver();
	bool IsBreaking();
	bool IsBreaking(int);
	bool IsEmpty(int, bool);
	void AddBlock(int, int=0);
	void DelBlock(int);
	int Draw(SDL_Surface*);
};


#endif

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

#ifndef __TEXT_H__
#define __TEXT_H__

class Text;
class Menu;

#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "SDL_image.h"
#include "option.h"

#ifdef GP2X
#define TEXTWIDTH  8
#define TEXTHEIGHT 16
#define MENUTILE   16
#else
#define TEXTWIDTH  16
#define TEXTHEIGHT 32
#define MENUTILE   32
#endif

#define NORMAL      0
#define HIGHLIGHTED (3 * TEXTHEIGHT)
#define DISABLED    (6 * TEXTHEIGHT)


class Text {
 private:
	int x, y;
	char* text;
	SDL_Rect destrect;
	SDL_Surface* textbits;
	void LoadGraphics();
 public:
	Text();
	~Text();
	void SetText(const char*);
	void SetX(int);
	void SetY(int);
	int GetLines();
	void Init(int, int, const char*);
	void Draw(SDL_Surface*);
};

class Menu {
 private:
	char* content;
	int* states;
	int x, y;
	int boxesw, boxesh;
	unsigned int choices, current;
	bool active;
	SDL_Rect destrect;
	SDL_Surface* textbits;
	SDL_Surface* menubits;
	void CalcArea();
	void LoadGraphics();
 public:
	Menu();
	~Menu();
	bool IsActive();
	void SetActive(bool);
	void AddChoice(const char*, int);
	bool ModChoice(const char*, unsigned int);
	bool MoveCursor(int);
	int GetState(int);
	bool SetState(int, int);
	int GetChoice();
	void SetChoice(int);
	int NumChoices();
	void SetPos(int, int);
	void Draw(SDL_Surface*);
};


#endif

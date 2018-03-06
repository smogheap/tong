/*
 * This file is part of TONG.              http://www.nongnu.org/tong
 * Copyright 2004, 2006 Owen Swerkstrom
 *
 * TONG! is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * TONG! is distributed in the hope that it will be useful and fun,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TONG!; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "text.h"


Text::Text() {
	LoadGraphics();
	x=0;
	y=0;
	text=NULL;
}

Text::~Text() {
	delete[] text;
	SDL_FreeSurface(textbits);
}

void Text::LoadGraphics() {
	SDL_Surface* ttemp;
	ttemp=IMG_Load("media/text.png");
	textbits=SDL_DisplayFormatAlpha(ttemp);
	SDL_FreeSurface(ttemp);
}

void Text::Init(int xpos, int ypos, const char* msg) {
	x=xpos;
	y=ypos;
	text=new char[(int)strlen(msg)+1];
	strcpy(text, msg);
	return;
}

void Text::SetText(const char* msg) {
	delete[] text;
	text=new char[(int)strlen(msg)+1];
	strcpy(text, msg);
	return;
}

void Text::SetX(int newx) {
	x=newx;
	return;
}

void Text::SetY(int newy) {
	y=newy;
	return;
}

int Text::GetLines() {
	int i, lines=1;
	for(i=0; i<(int)strlen(text); i++) {
		if(text[i]=='\n') {
			lines++;
		}
	}
	return lines;
}

void Text::Draw(SDL_Surface* surf) {
	int i;
	int letterx = x;
	int lettery = y;
	SDL_Rect srcrect;
	srcrect.w = TEXTWIDTH;
	srcrect.h = TEXTHEIGHT;
	//draw each letter
	for(i = 0; text[i]; i++) {
		if(text[i] == '\n') {
			letterx = x;
			lettery += TEXTHEIGHT;
		} else {
			srcrect.x = ((int)text[i] % 32) * TEXTWIDTH;
			if(text[i] < '@') {
				srcrect.y = 0;
			} else if(text[i] < '`') {
				srcrect.y = TEXTHEIGHT;
			} else {
				srcrect.y = TEXTHEIGHT * 2;
			}
			destrect.x = letterx;
			destrect.y = lettery;
			if(SDL_BlitSurface(textbits, &srcrect, surf, &destrect)) {
				LoadGraphics();
				SDL_BlitSurface(textbits, &srcrect, surf, &destrect);
			}
			letterx += TEXTWIDTH;
		}
	}
	return;
}


Menu::Menu() {
	LoadGraphics();
	content=new char[0];
	x=y=0;
	boxesw=boxesh=0;
	choices=0;
	current=0;
	active=false;
	states=new int;
}

Menu::~Menu() {
	delete[] content;
	SDL_FreeSurface(textbits);
	delete[] states;
}

void Menu::LoadGraphics() {
	SDL_Surface* ttemp;
	ttemp=IMG_Load("media/text.png");
	textbits=SDL_DisplayFormatAlpha(ttemp);
	SDL_FreeSurface(ttemp);
	ttemp=IMG_Load("media/textbox.png");
	menubits=SDL_DisplayFormatAlpha(ttemp);
	SDL_FreeSurface(ttemp);
}

//set whether the menu is active (cursor'd) or not
void Menu::SetActive(bool activate) {
	active=activate;
	return;
}

bool Menu::IsActive() {
	return active;
}

//add a choice labeled text to the menu
void Menu::AddChoice(const char* text, int state) {
	char* temp;
	int i;
	int* newstates=new int[choices+1];
 
	for(i=0; i<(int)choices; i++) {
		newstates[i]=states[i];
	}
	newstates[choices]=state;
	delete[] states;
	states=newstates;
	choices++;

	if(choices==1) {
		//easy to add the first one
		current=1;
		temp=new char[(int)strlen(text)+1];
		strcpy(temp, text);
	} else {
		//put newlines between choices, end with null
		temp=new char[(int)strlen(text)+(int)strlen(content)+2];
		strncpy(temp, content, strlen(content));
		temp[strlen(content)]='\n';
		temp[strlen(content)+1]='\0';
		strcat(temp, text);
	}
	//swap the completed temp in for the menu's content
	delete[] content;
	content=temp;
	CalcArea();
	return;
}

//replace the menu's which'th choice with text
bool Menu::ModChoice(const char* text, unsigned int which) {
	char* temp;
	unsigned int newlines=0, keepchars=0;
	int i, startpos=0, endpos=0;
	bool success=false;

	if(which>choices || which<1) {
		//    printf("out-of-range ModChoice\n");
	} else {
		//easy to replace just one
		if(choices==1) {
			current=1;
			temp=new char[(int)strlen(text)+1];
			if(temp==NULL) {
				printf("couldn't allocate %d!\n", (int)strlen(text)+1);
				return false;
			}
			strcpy(temp, text);
		} else {
			for(i=0; i<(int)strlen(content)-1; i++) {
				if(content[i]=='\n') {
					newlines++;
					//watch for position of newlines around which'th choice
					if(newlines+1==which) {
						startpos=i;
					} else if(newlines==which) {
						endpos=i;
					}
				}
			}
			keepchars=strlen(content)-(endpos-startpos);
			temp=new char[(int)strlen(text)+keepchars+2];
			if(temp==NULL) {
				printf("couldn't allocate %d!\n", (int)(keepchars+2+strlen(text)));
				return false;
			}
			if(endpos==0) {
				//last option changes
				strncpy(temp, content, startpos);
				temp[startpos]='\n';
				temp[startpos+1]='\0';
				strcat(temp, text);
			} else {
				if(startpos>0) {
					//some middle option changes
					strncpy(temp, content, startpos);
					temp[startpos]='\n';
					strncpy(temp+startpos+1, text, strlen(text)+1);
					startpos++; //?! not sure why this and the +1^ are needed
				} else {
					//first option changes
					strncpy(temp, text, strlen(text));
				}
				temp[startpos+strlen(text)]='\n';
				strcpy(temp+startpos+strlen(text)+1, content+endpos+1);
			} 
			/*this better not be necessary... :^S
			  if(temp[strlen(temp)]!='\0') {
			  temp[strlen(temp)]='\0';
			  }
			*/
			//swap the completed temp in for the menu's content
			delete[] content;
			content=temp;
			CalcArea();
			success=true;
		}
	}
	return success;
}

bool Menu::SetState(int which, int state) {
	bool success = false;
	int i = 0;
	if(which > (int)choices || which < 1) {
	} else {
		if(state == HIGHLIGHTED) {
			for(i = 0; i < (int)choices; i++) {
				if(states[i] == HIGHLIGHTED) {
					states[i] = NORMAL;
				}
			}
		}
		states[which - 1] = state;
		success = true;
	}
	return success;
}

int Menu::GetState(int which) {
	if(which > (int)choices || which < 1) {
		//printf("invalid choice GetState\n");
		return DISABLED;
	} else {
		return states[which - 1];
	}
}

bool Menu::MoveCursor(int direction) {
	int i;
	unsigned int oldcurrent = current;
	switch(direction) {
	case NORTH:
		for(i = current; i > 0; i--) {
			if(GetState(i - 1) != DISABLED) {
				current = i - 1;
				break;
			}
		}
		if(oldcurrent == current) {
			for(i = choices + 1; i > (int)oldcurrent; i--) {
				if(GetState(i - 1) != DISABLED) {
					current = i - 1;
					break;
				}
			}
		}
		break;
	case SOUTH:
		for(i = current; i <= (int)choices; i++) {
			if(GetState(i+1) != DISABLED) {
				current = i + 1;
				break;
			}
		}
		if(oldcurrent == current) {
			for(i = 0; i < (int)oldcurrent; i++) {
				if(GetState(i + 1) != DISABLED) {
					current = i + 1;
					break;
				}
			}
		}
		break;
	default:
		break;
	}
	return current != oldcurrent;
}

int Menu::GetChoice() {
	return current;
}

void Menu::SetChoice(int newchoice) {
	current = newchoice;
	return;
}

int Menu::NumChoices() {
	return choices;
}

void Menu::SetPos(int newx, int newy) {
	x = newx;
	y = newy;
	return;
}

void Menu::CalcArea() {
	int halves=0;
	int i;

	boxesw=boxesh=0;
	for(i=0; i<(int)strlen(content); i++) {
		if(content[i]=='\n') {
			boxesh++;
			if(boxesw<(halves/2)) {
				boxesw=halves/2;
				if(halves%2==0) {
					boxesw--;
				}
			}
			halves=0;
		} else {
			halves++;
		}
	}
	if(boxesw<(halves/2)) {
		boxesw=halves/2;
		if(halves%2==0) {
			boxesw--;
		}
	}
	return;
}

void Menu::Draw(SDL_Surface* surf) {
	SDL_Rect srcrect;
	int i;
	int j;
	int mode;
	int line = 0;
	int letterx = x;
	int lettery = y;
	if(active) {
		mode = NORMAL;
	} else {
		mode = DISABLED;
	}
	if(strlen(content) > 0) {
		//draw the background
		srcrect.w = srcrect.h = MENUTILE;
		for(i = 0; i <= boxesw + 1; i++) {
			for(j = 0; j <= boxesh + 1; j++) {
				if(i == 0) {
					srcrect.x = 0;
				} else if(i == boxesw + 1) {
					srcrect.x = MENUTILE * 2;
				} else {
					srcrect.x = MENUTILE;
				}
				if(j == 0) {
					srcrect.y = 0;
				} else if(j == boxesh + 1) {
					srcrect.y = MENUTILE * 2;
				} else {
					srcrect.y = MENUTILE;
				}
				destrect.x = x - (MENUTILE / 2) + (i * MENUTILE);
				destrect.y = y - (MENUTILE / 2) + (j * MENUTILE);
				if(SDL_BlitSurface(menubits, &srcrect, surf, &destrect)) {
					LoadGraphics();
					SDL_BlitSurface(menubits, &srcrect, surf, &destrect);
				}
			}
		}
		//draw each letter
		mode = active ? states[0] : DISABLED;
		srcrect.w = TEXTWIDTH;
		srcrect.h = TEXTHEIGHT;
		for(i = 0; i < (int)strlen(content); i++) {
			if(content[i] == '\n') {
				letterx = x;
				lettery += TEXTHEIGHT;
				line++;
				if((int)choices > line) {
					mode = active ? states[line] : DISABLED;
				}
			} else {
				srcrect.x = ((int)content[i] % 32) * TEXTWIDTH;
				if(content[i] < '@') {
					srcrect.y = 0 + mode;
				} else if(content[i] < '`') {
					srcrect.y = TEXTHEIGHT + mode;
				} else {
					srcrect.y = (TEXTHEIGHT * 2) + mode;
				}
				destrect.x = letterx;
				destrect.y = lettery;
				if(SDL_BlitSurface(textbits, &srcrect, surf, &destrect)) {
					LoadGraphics();
					SDL_BlitSurface(textbits, &srcrect, surf, &destrect);
				}
				letterx += TEXTWIDTH;
			}
		}
		if(active) {
			//draw the pointer finger thingy
			srcrect.x = 0;
			srcrect.y = MENUTILE * 3;
			srcrect.w = MENUTILE;
			srcrect.h = MENUTILE / 2;
			destrect.x = x - MENUTILE;
			destrect.y = y + (MENUTILE * 2 / 5) + (TEXTHEIGHT * (current-1));
			if(SDL_BlitSurface(menubits, &srcrect, surf, &destrect)) {
				LoadGraphics();
				SDL_BlitSurface(menubits, &srcrect, surf, &destrect);
			}
		}
	} else {
		//draw a blank menu if the menu is, erm, blank.
		srcrect.x = srcrect.y = 0;
		srcrect.w = srcrect.h = MENUTILE * 3;
		destrect.x = x - (MENUTILE / 2);
		destrect.y = y - (MENUTILE / 2);
		if(SDL_BlitSurface(menubits, &srcrect, surf, &destrect)) {
			LoadGraphics();
			SDL_BlitSurface(menubits, &srcrect, surf, &destrect);
		}
	}
	return;
}

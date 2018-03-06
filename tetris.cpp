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

#include "tetris.h"


Tetrad::Tetrad() {
	LoadGraphics();
}

Tetrad::~Tetrad() {
	SDL_FreeSurface(tetrisbits);
}

void Tetrad::Init(int newtype) {
	//piece[0] is the "pivot" block during rotations
	type=newtype;
	switch(type) {
	case SHAPE_Z:
		piece[0]=5;
		piece[1]=4;
		piece[2]=15;
		piece[3]=16;
		break;
	case SHAPE_S:
		piece[0]=15;
		piece[1]=6;
		piece[2]=14;
		piece[3]=5;
		break;
	case SHAPE_L:
		piece[0]=5;
		piece[1]=4;
		piece[2]=6;
		piece[3]=14;
		break;
	case SHAPE_J:
		piece[0]=5;
		piece[1]=4;
		piece[2]=6;
		piece[3]=16;
		break;
	case SHAPE_O:
		piece[0]=4;
		piece[1]=5;
		piece[2]=14;
		piece[3]=15;
		break;
	case SHAPE_T:
		piece[0]=5;
		piece[1]=4;
		piece[2]=6;
		piece[3]=15;
		break;
	case SHAPE_I:
	default:
		piece[0]=4;
		piece[1]=3;
		piece[2]=5;
		piece[3]=6;
		break;
	}
	pieces=4;
	//color=rand()%6+1;
	color = type;
	moves=0;
	haslanded=false;
	tetradrect.x = color * TILESIZE;
	tetradrect.y = 0;
	tetradrect.w = TILESIZE;
	tetradrect.h = TILESIZE;
	shadowrect.x = color * TILESIZE;
	shadowrect.y = TILESIZE * 2;
	shadowrect.w = TILESIZE;
	shadowrect.h = TILESIZE;
	return;
}

void Tetrad::Theme(const char* theme) {
	SDL_Surface *temp;
	char* filename = new char[80];
	sprintf(filename, "media/pieces%s.png", theme);
	temp=IMG_Load(filename);
	if(!temp) {
		printf("Failed to load tetrad graphics %s\n", filename);
	} else {
		SDL_FreeSurface(tetrisbits);
		tetrisbits=SDL_DisplayFormatAlpha(temp);
		SDL_FreeSurface(temp);
	}
	delete[] filename;
	return;
}

int Tetrad::GetType() {
	return type;
}

void Tetrad::LoadGraphics() {
	SDL_Surface* temp;
	temp=IMG_Load("media/pieces.png");
	if(!temp) {
		printf("Failed to load tetrad graphics media/pieces.png\n");
	}
	tetrisbits=SDL_DisplayFormatAlpha(temp);
	SDL_FreeSurface(temp);
}

bool Tetrad::MoveLeft(Bucket* bucket) {
	int i;
	bool move=!haslanded;
	for(i=0; i<pieces; i++) {
		if(!bucket->IsEmpty(piece[i]-1, true) ||
		   piece[i]%10==0) {
			move=false;
		}
	}
	if(move) {
		for(i=0; i<pieces; i++) {
			piece[i]--;
		}
		moves++;
	}
	return move;
}

bool Tetrad::MoveRight(Bucket* bucket) {
	int i;
	bool move=!haslanded;
	for(i=0; i<pieces; i++) {
		if(!bucket->IsEmpty(piece[i]+1, true) ||
		   piece[i]%10==9) {
			move=false;
		}
	}
	if(move) {
		for(i=0; i<pieces; i++) {
			piece[i]++;
		}
		moves++;
	}
	return move;
}

bool Tetrad::MoveDown(Bucket* bucket, bool land) {
	int i;
	bool move=pieces?true:false;  //ain't moving if there's nothing there!
	for(i=0; i<pieces; i++) {
		if(!bucket->IsEmpty(piece[i]+10, true) ||
		   piece[i]/10==19) {
			move=false;
		}
	}
	if(move) {
		for(i=0; i<pieces; i++) {
			piece[i]+=10;
		}
		moves++;
	} else {
		if(land) {
			for(i=0; i<pieces; i++) {
				bucket->AddBlock(piece[i], color);
			}
			haslanded=true;
		}
	}
	return move;
}

bool Tetrad::MoveUp(Bucket* bucket) {
	int i;
	bool move=true;
	for(i=0; i<pieces; i++) {
		if(!bucket->IsEmpty(piece[i]+10, true) ||
		   piece[i]/10==0) {
			move=false;
		}
	}
	if(move) {
		for(i=0; i<pieces; i++) {
			piece[i]-=10;
		}
		moves++;
	}
	return move;
}

bool Tetrad::Rotate(Bucket* bucket, bool clockwise) {
	int i, leniancy, leniancytype, min, max;
	bool leftedge=false, rightedge=false;
	bool preleft=false, preright=false;
	int temp[8]; //acts as a temporary last[], in case rotation is illegal
	bool rotate=!haslanded;
	for(i=0; i<pieces; i++) {
		temp[i]=piece[i];
		if(temp[i]%10==0) {
			preleft=true;
		} else if(temp[i]%10==9) {
			preright=true;
		}
	}
	for(leniancy=1; leniancy<pieces/2+1; leniancy++) {
		for(leniancytype=0; leniancytype<3; leniancytype++) {
			if(piece[0]%10==0) {
				leftedge=true;
			} else {
				leftedge=false;
			}
			if(piece[0]%10==9) {
				rightedge=true;
			} else {
				rightedge=false;
			}
			rotate=true;
			//try the rotate
			for(i=1; i<pieces; i++) {
				//x dist from [0] is piece[0]%10-piece[i]%10
				//y dist from [0] is piece[0]/10-piece[i]/10
				if(clockwise) {
					piece[i]=-10*(piece[0]%10-piece[i]%10)
						+piece[0]+(piece[0]/10-piece[i]/10);
				} else {
					piece[i]=10*(piece[0]%10-piece[i]%10)
						+piece[0]-(piece[0]/10-piece[i]/10);
				}
				if(piece[i]%10==0) {
					leftedge=true;
				} else if(piece[i]%10==9) {
					rightedge=true;
				}
			}
			//check valid
			min=9;
			max=0;
			for(i=0; i<pieces; i++) {
				if(piece[i]<0 || piece[i]>199 || !bucket->IsEmpty(piece[i], true) ||
				   (leftedge && rightedge) ) {
					rotate=false;
				}
				if(piece[i]%10<min) {
					min=piece[i]%10;
				}
				if(piece[i]%10>max) {
					max=piece[i]%10;
				}
			}
			if(max-min>pieces) {
				rotate=false;
			}
			if(rotate) {
				break;
			} else {
				for(i=0; i<pieces; i++) {
					switch(leniancytype) {
					case 0:        //try left
						if(!preleft) {
							piece[i]=temp[i]-leniancy;
						} else {
							piece[i]=temp[i];
						}
						break;
					case 1:        //try right
						if(!preright) {
							piece[i]=temp[i]+leniancy;
						} else {
							piece[i]=temp[i];
						}
						break;
					case 2:        //try down
					default:
						piece[i]=temp[i]+(10*leniancy);
						break;
					}
				}
			}
		}
		if(rotate) {
			break;
		}
	}
	if(!rotate) {
		for(i=0; i<pieces; i++) {
			//reset piece[] if we rotated illegally
			piece[i]=temp[i];
		}
	} else {
		moves++;
	}
	return rotate;
}

bool Tetrad::IsAt(int loc) {
	int i;
	for(i=0; i<pieces; i++) {
		if(piece[i]==loc) {
			return true;
		}
	}
	return false;
}

bool Tetrad::AddBlock(int loc) {
	int i;
	if(0>loc||loc>199) {
		return false;
	}
	for(i=0; i<pieces; i++) {
		if(IsAt(loc)) {
			return false;
		}
	}
	if(pieces<8) {
		piece[pieces]=loc;
		pieces++;
		moves++;
		return true;
	} else {
		return false;
	}
}

void Tetrad::DelBlock(int loc) {
	moves++;
	int i;
	bool gotit=false;
	for(i=0; i<pieces; i++) {
		if(piece[i]==loc) {
			gotit=true;
			if(i<pieces+1) {
				piece[i]=piece[i+1];
				loc=piece[i];    //little trick to delete & trickle in one loop
			}
		}
	}
	if(gotit) {
		pieces--;
	}
	if(pieces<1) {
		haslanded=true;
	}
}

bool Tetrad::HasLanded() {
	return haslanded;
}

void Tetrad::Draw(Bucket *bucket, SDL_Surface *surf) {
	int i, j;
	int shadoff=0, shadmin=20;
	for(i=0; i<pieces; i++) {
		destrect.x = ((piece[i] % 10) * TILESIZE) + OFFSET_X;
		destrect.y = ((piece[i] / 10) * TILESIZE) + OFFSET_Y;
		for(j=piece[i]; j<200; j+=10) {
			if(bucket->IsEmpty(j, true)) {
				shadoff = (j / 10) - (piece[i] / 10);
			} else {
				break;
			}
		}
		shadmin = (shadmin>shadoff) ? shadoff : shadmin;
		if(SDL_BlitSurface(tetrisbits, &tetradrect, surf, &destrect)) {
			LoadGraphics();
			SDL_BlitSurface(tetrisbits, &tetradrect, surf, &destrect);
		}
	}
	//draw the shadow
	for(i=0; i<pieces; i++) {
		destrect.x = ((piece[i]%10) * TILESIZE) + OFFSET_X;
		destrect.y= (((piece[i] + (shadmin * 10)) / 10) * TILESIZE) + OFFSET_Y;
		if(SDL_BlitSurface(tetrisbits, &shadowrect, surf, &destrect)) {
			LoadGraphics();
			SDL_BlitSurface(tetrisbits, &shadowrect, surf, &destrect);
		}
	}
	return;
}

void Tetrad::Preview(SDL_Surface *surf) {
	int i;
	SDL_Rect rect;
	rect.x = type * TILESIZE;
	rect.y = TILESIZE;
	rect.w = rect.h = TILESIZE;
	for(i = 0; i < pieces; i++) {
		destrect.x = ((piece[i] % 10) * TILESIZE) +
			PREVIEW_OFFSET_X - (2 * TILESIZE); //to center
		destrect.y = ((piece[i] / 10) * TILESIZE) + PREVIEW_OFFSET_Y;
		if(SDL_BlitSurface(tetrisbits, &rect, surf, &destrect)) {
			LoadGraphics();
			SDL_BlitSurface(tetrisbits, &rect, surf, &destrect);
		}
	}
	return;
}

int Tetrad::GetMoves() {
	return moves;
}


Bucket::Bucket() {
	LoadGraphics();
}

Bucket::~Bucket() {
	SDL_FreeSurface(tetrisbits);
}

void Bucket::Init(int junklevel) {
	int i;
	int inarow=0;
	for(i=0; i<200; i++) {
		area[i]=EMPTY;
	}
	for(i=200-junklevel*10; i<200; i++) {
		if(rand()%2 && inarow<10) {
			area[i]=rand()%6+1;
			inarow++;
		} else {
			inarow=0;
		}
	}
}

void Bucket::LoadGraphics() {
	SDL_Surface *temp;
	temp = IMG_Load("media/pieces.png");
	if(!temp) {
		printf("Failed to load bucket graphics media/pieces.png\n");
	}
	tetrisbits = SDL_DisplayFormatAlpha(temp);
	stackrect.w = TILESIZE;
	stackrect.h = TILESIZE;
	breakstart = 0;
	breakrowmin = 19;
	breakrowmax = 0;
}

void Bucket::Theme(const char* theme) {
	SDL_Surface *temp;
	char* filename = new char[80];
	sprintf(filename, "media/pieces%s.png", theme);
	temp=IMG_Load(filename);
	if(!temp) {
		printf("Failed to load bucket graphics %s\n", filename);
	} else {
		SDL_FreeSurface(tetrisbits);
		tetrisbits=SDL_DisplayFormatAlpha(temp);
		SDL_FreeSurface(temp);
	}
	delete[] filename;
	return;
}

void Bucket::GreyOut() {
	int i;
	for(i=0; i<200; i++) {
		if(area[i]!=EMPTY) {
			area[i]=STACK|WHITE;
		}
	}
	return;
}

void Bucket::AddJunk() {
	int i;
	int hole;
	for(i=0; i<190; i++) {
		area[i]=area[i+10];
	}
	hole=(rand()%10)+190;
	for(i=190; i<200; i++) {
		if(i!=hole) {
			//area[i]=STACK|rand()%7;
			area[i]=STACK|WHITE;
		} else {
			area[i]=EMPTY;
		}
	}
	return;
}

void Bucket::StartOver() {
	int i;
	for(i=0; i<200; i++) {
		if(area[i]!=EMPTY) {
			area[i]|=BREAK;
		}
	}
}

bool Bucket::IsBreaking() {
	int i;
	bool breaking=false;
	for(i=0; i<200; i++) {
		if(area[i]&BREAK) {
			return true;
		}
	}
	return breaking;
}

bool Bucket::IsBreaking(int block) {
	//looks redundant, but vc6 under windows whined every other way i tried it
	return (area[block]&BREAK)?true:false;
}

bool Bucket::IsEmpty(int block, bool break_is_empty) {
	if(block>199) {
		return false;
	} else {
		return (area[block]==EMPTY || (break_is_empty && area[block]&BREAK));
	}
}

void Bucket::AddBlock(int block, int color) {
	int i;
	bool fullrow=true;

	if(0>block||block>199) {
		return;
	}
	area[block]=STACK | color;
	for(i=(block/10)*10; i<(block/10)*10+10; i++) {
		if(IsEmpty(i, false)) {
			fullrow=false;
			break;
		}
	}
	if(fullrow) {
		for(i=(block/10)*10; i<(block/10)*10+10; i++) {
			area[i]|=BREAK;
		}
		if(block/10<breakrowmin) {
			breakrowmin=block/10;
		}
		if(block/10>breakrowmax) {
			breakrowmax=block/10;
		}
	}
	return;
}

void Bucket::DelBlock(int block) {
	if(0>block||block>200) {
		return;
	}
	if(!IsEmpty(block, false)) {
		area[block]|=BREAK;
	}
	return;
}

int Bucket::Drop() {
	int i, j;
	int droppingto=breakrowmax;
	int lines=0;
	if(breakrowmin<=breakrowmax) {
		for(i=breakrowmax; i>=0; i--) { //each row
			if(!(area[i*10]&BREAK)) {     //if it's not breaking
				for(j=0; j<10; j++) {   //each block
					area[droppingto*10+j]=area[i*10+j];
					area[i]=EMPTY;
				}
				droppingto--;
			} else {
				lines++;
			}
		}
	} else {
		for(i=0; i<200; i++) {
			if(area[i]&BREAK) {
				area[i]=EMPTY;
			}
		}
	}
	breakrowmin=19;
	breakrowmax=0;
	return lines;
}

int Bucket::Draw(SDL_Surface* surf) {
	int i;
	int lines=0;
	//TODO:  could be faster if went up from bottom?
	//       if an entire blank row is found, we're done, right?
	if(breakstart && SDL_GetTicks()-breakstart>BREAK_TIME) {
		lines=Drop();
		breakstart=0;
	}
	for(i=0; i<200; i++) {
		destrect.x = ((i % 10) * TILESIZE) + OFFSET_X;
		destrect.y = ((i / 10) * TILESIZE) + OFFSET_Y;
		if(area[i]&BREAK) {
			if(!breakstart) {
				//start breaking!
				stackrect.x=0;
				stackrect.y=0;
				if(SDL_BlitSurface(tetrisbits, &stackrect, surf, &destrect)) {
					LoadGraphics();
					SDL_BlitSurface(tetrisbits, &stackrect, surf, &destrect);
				}
				breakstart=SDL_GetTicks();
			} else {
				//continue breaking!
				//TODO:  funky visual tricks using breakstart and BREAK_TIME
				stackrect.x = (rand() % 7) * TILESIZE;
				stackrect.y = (rand() % 2) * TILESIZE;
				if(((float)(i%10)/10.0)+0.4 <=
				   1.0-(((float)SDL_GetTicks()-(float)breakstart)/
						(float)BREAK_TIME/2) ||
				   1.0-((float)(i%10)/10.0)+0.4 <=
				   1.0-(((float)SDL_GetTicks()-(float)breakstart)/
						(float)BREAK_TIME/2)) {
					if(SDL_BlitSurface(tetrisbits, &stackrect, surf,
									   &destrect)) {
						LoadGraphics();
						SDL_BlitSurface(tetrisbits, &stackrect, surf,
										&destrect);
					}
				}
			}
		} else if(area[i]!=EMPTY) {
			stackrect.x = (area[i] & COLORMASK) * TILESIZE;
			stackrect.y = TILESIZE;
			if(SDL_BlitSurface(tetrisbits, &stackrect, surf, &destrect)) {
				LoadGraphics();
				SDL_BlitSurface(tetrisbits, &stackrect, surf, &destrect);
			}
		}
	}
	return lines;
}

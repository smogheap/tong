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

#include "pong.h"


Ball::Ball() {
	LoadGraphics();
}

Ball::~Ball() {
	SDL_FreeSurface(pongbits);
}

void Ball::ReactWithStack(int balltetrispos, int hittetrispos, int direction,
						  Bucket* bucket, Option* options) {
	int i;
	bool ballstack=true, stackball=true;

	if(!tetrisreacted) {
		switch(options->GetRuleBallStack()) {
		case BALL_BREAKS_STACK:
			bucket->DelBlock(hittetrispos);
			break;
		case BALL_LINEBREAK_STACK:
			for(i=(hittetrispos/10)*10; i<(hittetrispos/10)*10+10; i++) {
				bucket->AddBlock(i);
			}
			break;
		case BALL_KILLS_STACK:
			bucket->StartOver();
			break;
		case BALL_JOINS_STACK:
			while(!bucket->IsEmpty(balltetrispos, false)) {
				switch(direction) {
				case NORTH:
					balltetrispos+=10;
					break;
				case SOUTH:
					balltetrispos-=10;
					break;
				case WEST:
					balltetrispos++;
					break;
				case EAST:
				default:
					balltetrispos--;
					break;
				}
			}
			bucket->AddBlock(balltetrispos);
			Init(startspeed);
			break;
		case NO_REACTION:
		default:
			ballstack=false;
			break;
		}
		switch(options->GetRuleStackBall()) {
		case STACK_BOUNCES_BALL:
			Bounce(direction);
			/*
			  if(direction==NORTH || direction==SOUTH) {
			  y=tetris2pongy(balltetrispos);
			  } else {
			  x=tetris2pongx(balltetrispos);
			  }
			*/
			break;
		case STACK_KILLS_BALL:
			Init(startspeed);  //TODO: add a state so we can play a sound or something
			break;
		case NO_REACTION:
		default:
			stackball=false;
			break;
		}
	}
	tetrisreacted=ballstack||stackball;
}

void Ball::ReactWithTetrad(int balltetrispos, int hittetrispos, int direction,
						   Tetrad* tetrad, Tetrad* nexttetrad, Bucket* bucket,
						   bool *newdown, Option* options) {
	bool tetradball=true, balltetrad=true;

	if(!tetrisreacted) {
		switch(options->GetRuleBallTetrad()) {
		case BALL_BREAKS_TETRAD:
			tetrad->DelBlock(hittetrispos);
			break;
		case BALL_MOVES_TETRAD:
			switch(direction) {
			case NORTH:
				tetrad->MoveUp(bucket);
				break;
			case SOUTH:
				tetrad->MoveDown(bucket, false);
				break;
			case WEST:
				tetrad->MoveLeft(bucket);
				break;
			case EAST:
			default:
				tetrad->MoveRight(bucket);
				break;
			}
			break;
		case BALL_ROTATES_TETRAD:
			if(balltetrispos<=hittetrispos) {
				tetrad->Rotate(bucket, true);
			} else {
				tetrad->Rotate(bucket, false);
			}
			break;
		case BALL_DROPS_TETRAD:
			while(tetrad->MoveDown(bucket, false));
			break;
		case BALL_KILLS_TETRAD:
			//TODO: flag for sound?
			tetrad->Init(nexttetrad->GetType());
			nexttetrad->Init(rand()%7);
			*newdown = true;
			break;
		case BALL_JOINS_TETRAD:
			while(tetrad->IsAt(balltetrispos)) {
				switch(direction) {
				case NORTH:
					balltetrispos+=10;
					break;
				case SOUTH:
					balltetrispos-=10;
					break;
				case WEST:
					balltetrispos++;
					break;
				case EAST:
				default:
					balltetrispos--;
					break;
				}
			}
			tetrad->AddBlock(balltetrispos);
			Init(startspeed);
			break;
		case NO_REACTION:
		default:
			balltetrad=false;
			break;
		}
		switch(options->GetRuleTetradBall()) {
		case TETRAD_BOUNCES_BALL:
			Bounce(direction);
			/*
			  if(direction==NORTH || direction==SOUTH) {
			  y=tetris2pongy(balltetrispos);
			  } else {
			  x=tetris2pongx(balltetrispos);
			  }
			*/
			break;
		case TETRAD_KILLS_BALL:
			tetradball=balltetrad=true;
			Init(startspeed);
		case NO_REACTION:
		default:
			tetradball=false;
			break;
		}
	}
	tetrisreacted=tetradball||balltetrad;
}

void Ball::Init(float spd) {
	int startpos=rand() % 2;
	x = (float)(startpos ? BOUND_LEFT : (BOUND_RIGHT - TILESIZE));
	y = BOUND_TOP;
	angle = (float)(rand() % 45 + (startpos ? 292 : 202));
	//  x=BOUND_LEFT+32+rand()%80;
	speed = spd;
	startspeed = spd;
	//  angle=240+rand()%120;
	ballstatus = MOVED;
	lastmove = SDL_GetTicks();
	startmoving = lastmove + NEWBALL_NOTICE;
	visible = true;
	trappedbounces = 0;
}

void Ball::LoadGraphics() {
	SDL_Surface* temp;
	temp = IMG_Load("media/pieces.png");
	if(!temp) {
		printf("Failed to load ball graphics media/pieces.png\n");
	}
	pongbits = SDL_DisplayFormatAlpha(temp);
	SDL_FreeSurface(temp);
	ballrect.x = 0;
	ballrect.y = TILESIZE * 3;
	ballrect.w = TILESIZE;
	ballrect.h = TILESIZE;
}

void Ball::Theme(const char* theme) {
	SDL_Surface *temp;
	char* filename = new char[80];
	sprintf(filename, "media/pieces%s.png", theme);
	temp=IMG_Load(filename);
	if(!temp) {
		printf("Failed to load ball graphics %s\n", filename);
	} else {
		SDL_FreeSurface(pongbits);
		pongbits=SDL_DisplayFormatAlpha(temp);
		SDL_FreeSurface(temp);
	}
	delete[] filename;
	return;
}

void Ball::UpdateTime() {
	if(lastmove<startmoving) {
		startmoving=SDL_GetTicks()+(startmoving-lastmove);
	}
	lastmove=SDL_GetTicks();
}

int Ball::Move(Paddle* pad, Bucket* bucket, Tetrad* tetrad, Tetrad* nexttetrad,
			   bool *newdown, Option* options) {
	if(trappedbounces>20) {
		Init(startspeed);
	}
	tetrisreacted=false;
	ballstatus=MOVED;
	if(lastmove>startmoving) {
		int tetrispos;
		float newx, newy;
		float movedist=(float)(speed*((SDL_GetTicks()-lastmove)/1000.0));
#ifdef GP2X
		movedist /= 2.0;
#endif
		//angle corrections, in case english gets out of hand etc :^)
		while(angle>360) {
			angle-=360;
		}
		while(angle<0) {
			angle+=360;
		}
		if(angle<90) {
			newx=x+(float)cos(angle*PI/180)*movedist;
			newy=y-(float)sin(angle*PI/180)*movedist;
		} else if(angle<180) {
			newx=x-(float)sin((angle-90)*PI/180)*movedist;
			newy=y-(float)cos((angle-90)*PI/180)*movedist;
		} else if(angle<270) {
			newx=x-(float)cos((angle-180)*PI/180)*movedist;
			newy=y+(float)sin((angle-180)*PI/180)*movedist;
		} else {
			newx=x+(float)sin((angle-270)*PI/180)*movedist;
			newy=y+(float)cos((angle-270)*PI/180)*movedist;
		}
		if(newy < BOUND_TOP) {
			if(pad->IsAtX((int)newx)) { //TODO:  somewhere between x and newx
				Bounce(NORTH);
				newy=BOUND_TOP; //TODO:  close, but not 100% accurate
				angle += 4 * pad->GetXSpeed();
			} else {
				ballstatus=OUT_OF_BOUNDS;
			}
		} else if(newy > BOUND_BOTTOM - TILESIZE) {
			if(pad->IsAtX((int)newx)) { //TODO:  x~newx
				Bounce(SOUTH);
				newy = BOUND_BOTTOM - TILESIZE;  //TODO: ~=
				angle -= 4 * pad->GetXSpeed();
			} else {
				ballstatus = OUT_OF_BOUNDS;
			}
		}
		if(newx < BOUND_LEFT) {
			if(pad->IsAtY((int)newy)) {  //TODO:  y~newy
				Bounce(WEST);
				newx=BOUND_LEFT; //TODO
				angle -= 4 * pad->GetYSpeed();
			} else {
				ballstatus = OUT_OF_BOUNDS;
			}
		} else if(newx > BOUND_RIGHT - TILESIZE) {
			if(pad->IsAtY((int)newy)) { //TODO:  y~newy
				Bounce(EAST);
				newx = BOUND_RIGHT - TILESIZE; //TODO
				angle += 4 * pad->GetYSpeed();
			} else {
				ballstatus = OUT_OF_BOUNDS;
			}
		} else { //no paddle bouncing, no out of bounds, all's well
			//work with tetris stuff
			tetrispos=pong2tetris((int)newx, (int)newy);
			if(tetrispos>0 && tetrispos<200) {
				//bucket
				if(angle<180) {
					if(!bucket->IsEmpty(tetrispos, true)) {
						ReactWithStack(tetrispos, tetrispos, NORTH, bucket, options);
					} else if(tetrispos%10<9 && !bucket->IsEmpty(tetrispos+1, true)) {
						ReactWithStack(tetrispos, tetrispos+1, NORTH, bucket, options);
					}
				} else {
					if(tetrispos/10<19 && !bucket->IsEmpty(tetrispos+10, true)) {
						ReactWithStack(tetrispos, tetrispos+10, SOUTH, bucket, options);
					} else if(tetrispos%10<9 && tetrispos/10<19 &&
							  !bucket->IsEmpty(tetrispos+11, true)) {
						ReactWithStack(tetrispos, tetrispos+11, SOUTH, bucket, options);
					}
				}
				if(angle<90||angle>270) {
					if(tetrispos%10<9 && !bucket->IsEmpty(tetrispos+1, true)) {
						ReactWithStack(tetrispos, tetrispos+1, EAST, bucket, options);
					} else if(tetrispos%10<9 && tetrispos/10<19 &&
							  !bucket->IsEmpty(tetrispos+11, true)) {
						ReactWithStack(tetrispos, tetrispos+11, EAST, bucket, options);
					}
				} else {
					if(!bucket->IsEmpty(tetrispos, true)) {
						ReactWithStack(tetrispos, tetrispos, WEST, bucket, options);
					} else if(tetrispos/10<19 && !bucket->IsEmpty(tetrispos+10, true)) {
						ReactWithStack(tetrispos, tetrispos+10, WEST, bucket, options);
					}
				}

				//tetrad
				if(angle<180) {
					if(tetrad->IsAt(tetrispos)) {
						ReactWithTetrad(tetrispos, tetrispos, NORTH, tetrad,
										nexttetrad, bucket, newdown, options);
					} else if(tetrispos%10<9 && tetrad->IsAt(tetrispos+1)) {
						ReactWithTetrad(tetrispos, tetrispos+1, NORTH, tetrad,
										nexttetrad, bucket, newdown, options);
					}
				} else {
					if(tetrispos/10<19 && tetrad->IsAt(tetrispos+10)) {
						ReactWithTetrad(tetrispos, tetrispos+10, SOUTH, tetrad,
										nexttetrad, bucket, newdown, options);
					} else if(tetrispos%10<9 && tetrispos/10<19 &&
							  tetrad->IsAt(tetrispos+11)) {
						ReactWithTetrad(tetrispos, tetrispos+11, SOUTH, tetrad,
										nexttetrad, bucket, newdown, options);
					}
				}
				if(angle<90||angle>270) {
					if(tetrispos%10<9 && tetrad->IsAt(tetrispos+1)) {
						ReactWithTetrad(tetrispos, tetrispos+1, EAST, tetrad,
										nexttetrad, bucket, newdown, options);
					} else if(tetrispos%10<9 && tetrispos/10<19 &&
							  tetrad->IsAt(tetrispos+11)) {
						ReactWithTetrad(tetrispos, tetrispos+11, EAST, tetrad,
										nexttetrad, bucket, newdown, options);
					}
				} else {
					if(tetrad->IsAt(tetrispos)) {
						ReactWithTetrad(tetrispos, tetrispos, WEST, tetrad,
										nexttetrad, bucket, newdown, options);
					} else if(tetrispos/10<19 && tetrad->IsAt(tetrispos+10)) {
						ReactWithTetrad(tetrispos, tetrispos+10, WEST, tetrad,
										nexttetrad, bucket, newdown, options);
					}
				}
			}
		}
		if(!tetrisreacted) {
			x=newx;
			y=newy;
		}
		if(BOUND_LEFT > x || x > BOUND_RIGHT - TILESIZE ||
		   BOUND_TOP > y || y > BOUND_BOTTOM - TILESIZE) {
			ballstatus = OUT_OF_BOUNDS;
		}
		if(!visible) {
			visible=true;
		}
	} else {
		visible =! visible;
	}
	lastmove = SDL_GetTicks();
	if(ballstatus != BOUNCED && trappedbounces > 0) {
		trappedbounces--;
	} else {
		if(ballstatus == BOUNCED && options->GetBallSpeedsUp()) {
			if(speed < 256) {
				speed +=2;
			} else {
				//max speed ball
			}
		}
	}
	return ballstatus;
}

//bounce off of a surface that's to the north, west, east, or south
void Ball::Bounce(int direction) {
	trappedbounces++;
	switch(direction) {
	case NORTH:
		if(angle>90) {
			angle=180+(180-angle);
		} else {
			angle=360-angle;
		}
		break;
	case SOUTH:
		if(angle<270) {
			angle=90+(270-angle);
		} else {
			angle=360-angle;
		}
		break;
	case WEST:
		if(angle>180) {
			angle=360-(angle-180);
		} else {
			angle=90-(angle-90);
		}
		break;
	case EAST:
		if(angle>180) {
			angle=180+(360-angle);
		} else {
			angle=180-angle;
		}
		break;
	default:
		printf("Call to Ball::Bounce() made without proper direction\n");
		break;
	}
	ballstatus=BOUNCED;
	return;
}

void Ball::Draw(SDL_Surface* surf) {
	if(visible) {
		destrect.x=(int)x;
		destrect.y=(int)y;
		if(SDL_BlitSurface(pongbits, &ballrect, surf, &destrect)) {
			LoadGraphics();
			SDL_BlitSurface(pongbits, &ballrect, surf, &destrect);
		}
	}
	return;
}


Paddle::Paddle() {
	LoadGraphics();
	Init(4, 4);
}

Paddle::~Paddle() {
	SDL_FreeSurface(pongbits);
}

void Paddle::LoadGraphics() {
	SDL_Surface* temp;
	temp=IMG_Load("media/pieces.png");
	if(!temp) {
		printf("Failed to load paddle graphics media/pieces.png\n");
	}
	pongbits=SDL_DisplayFormatAlpha(temp);
	x=BOUND_LEFT+8;
	y=BOUND_TOP+19;
	xspeed=0;
	yspeed=0;
	return;
}

void Paddle::Theme(const char* theme) {
	SDL_Surface *temp;
	char* filename = new char[80];
	sprintf(filename, "media/pieces%s.png", theme);
	temp=IMG_Load(filename);
	if(!temp) {
		printf("Failed to load paddle graphics %s\n", filename);
	} else {
		SDL_FreeSurface(pongbits);
		pongbits=SDL_DisplayFormatAlpha(temp);
		SDL_FreeSurface(temp);
	}
	delete[] filename;
	return;
}

void Paddle::Init(int sizex, int sizey) {
	if(sizex<2) {
		xsize=2;
	} else if(sizex>10) {
		xsize=10;
	} else {
		xsize=sizex;
	}
	if(sizey<2) {
		ysize=2;
	} else if(sizey>20) {
		ysize=20;
	} else {
		ysize=sizey;
	}
	MouseMoved(0, 0);
	return;
}

bool Paddle::MouseMoved(int xrel, int yrel) {
	int oldx = x;
	int oldy = y;
	x += xrel;
	y += yrel;
	if(x < BOUND_LEFT) {
		x = BOUND_LEFT;
	} else if(x > BOUND_RIGHT - (TILESIZE * xsize)) {
		x = BOUND_RIGHT - (TILESIZE * xsize);
	}
	if(y < BOUND_TOP) {
		y = BOUND_TOP;
	} else if(y > BOUND_BOTTOM - (TILESIZE * ysize)) {
		y = BOUND_BOTTOM - (TILESIZE * ysize);
	}
	if(x != oldx || y != oldy) {
		xspeed = (float)xrel;
		yspeed = (float)yrel;
		return true;
	}
	return false;
}

bool Paddle::MoveX(int amt) {
	int oldx = x;
	if(-3200 > amt || amt > 3200) {
		x += (int)(amt / 32000 * (SDL_GetTicks() - lastdraw));
		if(x < BOUND_LEFT) {
			x = BOUND_LEFT;
		} else if(x > BOUND_RIGHT - (TILESIZE * xsize)) {
			x = BOUND_RIGHT-(TILESIZE * xsize);
		}
		if(oldx != x){
			xspeed = amt / 32000 * (float)(SDL_GetTicks() - lastdraw) / 2.0;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool Paddle::MoveY(int amt) {
	int oldy = y;
	if(-3200 > amt || amt > 3200) {
		y += (int)(amt / 32000 * (SDL_GetTicks() - lastdraw));
		if(y < BOUND_TOP) {
			y = BOUND_TOP;
		} else if(y > BOUND_BOTTOM - (TILESIZE * ysize)) {
			y = BOUND_BOTTOM - (TILESIZE * ysize);
		}
		if(oldy != y) {
			yspeed = amt / 32000 * (float)(SDL_GetTicks() - lastdraw) / 2.0;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

float Paddle::GetXSpeed() {
	//cap it at +-5;
	if(xspeed > 5 || xspeed < -5) {
		return (float)(xspeed > 0 ? 5 : -5);
	}
	return xspeed;
}

float Paddle::GetYSpeed() {
	//cap it at +-5;
	if(yspeed > 5 || yspeed <- 5) {
		return (float)(yspeed > 0 ? 5: -5);
	}
	return yspeed;
}

bool Paddle::IsAtX(int testx) {
	if(x - TILESIZE <= testx && testx <= x + (TILESIZE * xsize)) {
		return true;
	}
	return false;
}

bool Paddle::IsAtY(int testy) {
	if(y - TILESIZE <= testy && testy <= y + (TILESIZE * ysize)) {
		return true;
	}
	return false;
}

void Paddle::Draw(SDL_Surface* surf) {
	int i;
	MouseMoved(0,0);
	//left end of the top/bottom paddles
	paddlerect.x = TILESIZE;
	paddlerect.y = TILESIZE * 3;
	paddlerect.w = TILESIZE * 2;
	paddlerect.h = TILESIZE;
	for(i = 0; i < xsize; i++) {
		if(i == xsize - 1) {
			paddlerect.x = TILESIZE * 3;
		} else if(i > 0) {
			paddlerect.x = TILESIZE * 2;
		}
		destrect.x = x + (TILESIZE * i);
		destrect.y = BOUND_TOP - TILESIZE;
		if(SDL_BlitSurface(pongbits, &paddlerect, surf, &destrect)) {
			LoadGraphics();
			SDL_BlitSurface(pongbits, &paddlerect, surf, &destrect);
		}
		destrect.y = BOUND_BOTTOM;
		if(SDL_BlitSurface(pongbits, &paddlerect, surf, &destrect)) {
			LoadGraphics();
			SDL_BlitSurface(pongbits, &paddlerect, surf, &destrect);
		}
	}

	paddlerect.x = TILESIZE * 7;
	paddlerect.y = TILESIZE * 2;
	paddlerect.w = TILESIZE;
	paddlerect.h = TILESIZE;
	for(i = 0; i < ysize; i++) {
		if(i == ysize - 1) {
			paddlerect.y = TILESIZE * 4;
		} else if(i > 0) {
			paddlerect.y = TILESIZE * 3;
		}
		destrect.y = y + (TILESIZE * i);
		destrect.x = BOUND_LEFT - TILESIZE;
		if(SDL_BlitSurface(pongbits, &paddlerect, surf, &destrect)) {
			LoadGraphics();
			SDL_BlitSurface(pongbits, &paddlerect, surf, &destrect);
		}
		destrect.x = BOUND_RIGHT;
		if(SDL_BlitSurface(pongbits, &paddlerect, surf, &destrect)) {
			LoadGraphics();
			SDL_BlitSurface(pongbits, &paddlerect, surf, &destrect);
		}
	}

	xspeed *= 0.5; //cut speeds in half every draw, fair to slow/fast users
	yspeed *= 0.5; //given the paddle has to be drawn where it can bounce...?
	lastdraw = SDL_GetTicks();
	return;
}

//takes pong x and y coordinates, returns nearest tetris block
//returns -1 on out of bounds
int pong2tetris(int x, int y) {
	if(BOUND_LEFT > x || x > BOUND_RIGHT || BOUND_TOP > y || y > BOUND_BOTTOM) {
		return -1;
	}
	//  printf("tetspot: %d\n", (((y-BOUND_TOP)/16)*10)+((x-BOUND_LEFT)/16));
	return (((y - BOUND_TOP) / TILESIZE) * 10) + ((x - BOUND_LEFT) / TILESIZE);
}

int tetris2pongx(int tetrisblock) {
	if(0 > tetrisblock || tetrisblock > 199) {
		return -1;
	}
	return ((tetrisblock % 10) * TILESIZE) + OFFSET_X;
}

int tetris2pongy(int tetrisblock) {
	if(0 > tetrisblock || tetrisblock > 199) {
		return -1;
	}
	return ((tetrisblock / 10) * TILESIZE) + OFFSET_Y;
}

#ifndef _pong_h
#define _pong_h

#include <stdlib.h>
#include <iostream.h>


class paddle
{
	int xpos, ypos;
   int length;
	void move(int, bool); //bool=alignment, true=vert, false=horiz
   friend void render(bool, int, int, int);
   friend void msg(const char*);
public:
	paddle();
	void tick(int);
   int isthere(int, int, int&);
};


class ball
{
	int x, y;
   int rise, run;
   int wasx, wasy;
   int timer;
public:
	ball();
   void newball();
	int boxthere(int, int, matrix&);
   void tick(paddle&, int&, matrix&, int&);
};


#endif
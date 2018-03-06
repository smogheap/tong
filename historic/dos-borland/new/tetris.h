#ifndef _tetris_h
#define _tetris_h

#include <stdlib.h>
#include <iostream.h>


class matrix
{
	int grid[210];
   void bustline(int);
   friend remvbox(int);
   friend plotbox(int, int);
public:
	matrix();
	bool isthere(int);
   void puthere(int*, int, int);
   void render();
};

class tetrad
{
	int* piece;
   int parts;
   int color;
//   int piv;
   int timer;
	bool down(matrix&);
   void left(matrix&);
   void right(matrix&);
   void rotate(matrix&);
//	friend void render(int*, int&, int);
   friend unsigned char getscancode();
   friend void plotbox(int, int);
public:
	tetrad();
   void newpiece(matrix&, int);
   void tick(matrix&, int&, int);
};

//void plotbox(int, int);
void dumpbox(int, int);
void whitbox(int);
//void remvbox(int);
void bustline(int, int*, int);


#endif
#ifndef _objects_cpp
#define _objects_cpp

#include <stdlib.h>

class tetrad
{
public:
	int piece[8];
   int parts;
   int piv;
   int timer;
	tetrad();
   void newpiece(ofstream&, int*);
   bool canfall(int*);
   void tick(int, int*, bool&, int&);
	void down(int*);
};

tetrad::tetrad()
{
	timer=0;
}

void tetrad::newpiece(ofstream &log, int *matrix)
{
	randomize();
   int x=(rand())%7;
   switch(x)
   {
   	case 0:
			piece[0]=4;		piece[1]=5;		piece[2]=15;	piece[3]=16;
			parts=4;
		   piv=15;
log<<"created new z piece\n";
			break;
   	case 1:
			piece[0]=5;		piece[1]=6;		piece[2]=14;	piece[3]=15;
			parts=4;
		   piv=15;
log<<"created new s piece\n";
			break;
   	case 2:
			piece[0]=5;		piece[1]=14;	piece[2]=15;	piece[3]=16;
			parts=4;
		   piv=15;
log<<"created new t piece\n";
			break;
   	case 3:
			piece[0]=4;		piece[1]=14;	piece[2]=15;	piece[3]=16;
			parts=4;
		   piv=15;
log<<"created new j piece\n";
			break;
   	case 4:
			piece[0]=6;		piece[1]=14;	piece[2]=15;	piece[3]=16;
			parts=4;
		   piv=15;
log<<"created new l piece\n";
			break;
   	case 5:
			piece[0]=4;		piece[1]=5;		piece[2]=14;	piece[3]=15;
			parts=4;
		   piv=14;
log<<"created new b piece\n";
			break;
   	case 6:
			piece[0]=3;		piece[1]=4;		piece[2]=5;		piece[3]=6;
			parts=4;
		   piv=4;
log<<"created new i piece\n";
			break;

	}
	for(int i=0; i<parts; i++)
   	if(matrix[piece[i]]!=0)
			exit(0);
}

bool tetrad::canfall(int *matrix)
{
	for(int i=0; i<parts; i++)
   {
		if(matrix[piece[i]+10]!=0) return false;
   }
   return true;
}

void tetrad::tick(int timer, int *matrix, bool &addtetrad, int &tetrads)
{
	if(timer%6==0)
   	if(canfall(matrix))
      	down(matrix);
      else
      {
			for(int i=0; i<parts; i++)
         	matrix[piece[i]]=2;
         tetrads--;
      	addtetrad=true;
      }
//if keypress stuff
}

void tetrad::down(int *matrix)
{
	for(int i=0; i<parts; i++)
   {
		matrix[piece[i]]=-1;
      piece[i]+=10;
      matrix[piece[i]]=1;
   }
   return;
}

#endif
#ifndef _tetris_cpp
#define _tetris_cpp

#include <stdlib.h>
#include <iostream.h>

const int TETSPEED=200;

class tetrad
{
	int piece[8];
   int parts;
   int piv;
   int timer;
	bool down(int*);
   void left(int*);
   void right(int*);
   void rotate(int*);
	friend void render(int*, int&, int);
   friend unsigned char getscancode();
public:
	tetrad();
   int newpiece(int*, int);
   void tick(int*, int&, int);
};

void plotbox(int, int);
void dumpbox(int, int);
void whitbox(int);
void remvbox(int);
void bustline(int, int*, int);


tetrad::tetrad()
{
	timer=0;
}


int tetrad::newpiece(int *matrix, int x)
{
   switch(x)
   {
   	case 0: //Z
			piece[0]=4;		piece[1]=5;		piece[2]=15;	piece[3]=16;
			parts=4;
		   piv=15;
			break;
   	case 1: //S
			piece[0]=5;		piece[1]=6;		piece[2]=14;	piece[3]=15;
			parts=4;
		   piv=15;
			break;
   	case 2: //T
			piece[0]=5;		piece[1]=14;	piece[2]=15;	piece[3]=16;
			parts=4;
		   piv=15;
			break;
   	case 3: //J
			piece[0]=4;		piece[1]=14;	piece[2]=15;	piece[3]=16;
			parts=4;
		   piv=15;
			break;
   	case 4: //L
			piece[0]=6;		piece[1]=14;	piece[2]=15;	piece[3]=16;
			parts=4;
		   piv=15;
			break;
   	case 5: //B
			piece[0]=4;		piece[1]=5;		piece[2]=14;	piece[3]=15;
			parts=4;
		   piv=14;
			break;
   	default: //I
			piece[0]=3;		piece[1]=4;		piece[2]=5;		piece[3]=6;
			parts=4;
		   piv=4;
			break;
	}

/*
for(int a=0; a<20; a++)
{ for(int b=0; b<10; b++)
  log<<'['<<matrix[(a*10)+b]<<']';
log<<'\n'; }
*/

	for(int i=0; i<parts; i++)
	{
   	if(0>piece[i]||piece[i]>30)
      {
cout<<"had a premature return, game should recover\n";
      	return 1;
		}
   	if(matrix[piece[i]]!=0)
      {
//for(int j=0; j<parts; j++) log<<"piece["<<j<<"]="<<piece[j]<<"\n";
			exit(0);
      }
	}
   return random(6)+9;
}


void tetrad::tick(int *matrix, int &tetrads, int check)
{
	switch(check)
   {
		case 80: //down
	      if(!down(matrix))
	      {
				for(int i=0; i<parts; i++)
	         	matrix[piece[i]]=2;
	         tetrads--;
	      }
/*
        	if(!down(matrix));
	      {
				for(int i=0; i<parts; i++)
	         	matrix[piece[i]]=2;
	         tetrads--;
	      	addtetrad=true;
	      }
*/
         break;
	   case 75: //left
         left(matrix);
         break;
	   case 77: //right
        	right(matrix);
         break;
      case 72: //up
      	rotate(matrix);
         break;
   }
	if(timer>TETSPEED)
   {
      if(!down(matrix))
      {
			for(int i=0; i<parts; i++)
         	matrix[piece[i]]=2;
         tetrads--;
      }
      else timer=0;
   }
   timer++;
   return;
}


bool tetrad::down(int *matrix)
{
	bool canfall=true;
	for(int l=0; l<parts; l++)
		if(matrix[piece[l]+10]>1) canfall=false;
	if(canfall)
   {
		for(int i=0; i<parts; i++)
	   {
			matrix[piece[i]]=-1;
	      piece[i]+=10;
	   }
	   piv+=10;
		for(int k=0; k<parts; k++)
			matrix[piece[k]]=1;
   }
   return canfall;
}


void tetrad::left(int *matrix)
{
	bool canleft=true;
	for(int l=0; l<parts; l++)
		if(matrix[piece[l]-1]!=0||piece[l]%10==0) canleft=false;
	if(canleft)
   {
		for(int i=0; i<parts; i++)
	   {
			matrix[piece[i]]=-1;
	      piece[i]-=1;
	      matrix[piece[i]]=1;
	   }
	   piv-=1;
		for(int k=0; k<parts; k++)
			matrix[piece[k]]=1;
   }
   return;
}


void tetrad::right(int *matrix)
{
	bool canright=true;
	for(int l=0; l<parts; l++)
		if(matrix[piece[l]+1]!=0||piece[l]%10==9) canright=false;
	if(canright)
	{
		for(int i=0; i<parts; i++)
	   {
			matrix[piece[i]]=-1;
	      piece[i]+=1;
	      matrix[piece[i]]=1;
	   }
	   piv++;
		for(int k=0; k<parts; k++)
			matrix[piece[k]]=1;
   }
   return;
}


void tetrad::rotate(int *matrix)
{
	int temp[8]={0,0,0,0,0,0,0,0};
	int xaway=0, yaway=0, taway=0;
 	for(int i=0; i<parts; i++)
   {
   	matrix[i]=0;
   	if(piece[i]==piv) temp[i]=piv;
      else
      {
       	xaway=(piece[i]%10)-(piv%10);
         yaway=(piece[i]/10)-(piv/10);
			taway=yaway;
         yaway=xaway;
         xaway=-1*taway;
         temp[i]=piv+xaway+(yaway*10);
      }
   }
	bool extleft=false;
   bool extright=false;
   bool outabounds=false;
	for(int k=0; k<parts; k++)
	   {
	   	if(temp[k]%10==0) extleft=true;
         if(temp[k]%10==9) extright=true;
         if(matrix[temp[k]]>0||temp[k]<0) outabounds=true;
	   }
   if(!(extleft&&extright)&&!outabounds)
	{
	   for(int j=0; j<parts; j++)
	   {
	   	matrix[piece[j]]=-1;
	   	piece[j]=temp[j];
	   }
      for(int l=0; l<parts; l++)
	      matrix[piece[l]]=1;
   }
   return;
}


void plotbox(int elem, int color)
{
	int x=(elem%10)*20+40;
   int y=(elem/10)*20+40;
   setcolor(color);
   rectangle(x,y,x+19,y+19);
   rectangle(x+4,y+4,x+15,y+15);
   setcolor(color-8);
   rectangle(x+2,y+2,x+17,y+17);
   rectangle(x+6,y+6,x+13,y+13);
}


void dumpbox(int elem, int color)
{
	int x=(elem%10)*20+40;
   int y=(elem/10)*20+40;
   setcolor(color);
   rectangle(x,y,x+19,y+19);
   setcolor(8);
   rectangle(x+2,y+2,x+17,y+17);
   rectangle(x+4,y+4,x+15,y+15);
   rectangle(x+6,y+6,x+13,y+13);
}


void whitbox(int elem)
{
	int x=(elem%10)*20+40;
   int y=(elem/10)*20+40;
   setcolor(15);
   rectangle(x,y,x+19,y+19);
   rectangle(x+2,y+2,x+17,y+17);
   rectangle(x+4,y+4,x+15,y+15);
   rectangle(x+6,y+6,x+13,y+13);
   setcolor(7);
}


void remvbox(int elem)
{
	int x=(elem%10)*20+40;
   int y=(elem/10)*20+40;
   setcolor(0);
   rectangle(x,y,x+19,y+19);
   rectangle(x+2,y+2,x+17,y+17);
   rectangle(x+4,y+4,x+15,y+15);
   rectangle(x+6,y+6,x+13,y+13);
}


void bustline(int x, int* matrix, int s)
{
	for(int j=0; j<10; j++)
   	matrix[x*10+j]=-2;
	render(matrix, -1, 1);
	for(int k=4; k>-1; k--)
   {
//   	sound(100+k*100+s*100);
//		delay(2);
//	   nosound();
      delay(50);
      remvbox(x*10+k);
      remvbox(x*10+9-k);
   }
	for(int i=x*10+9; i>-1; i--)
   	if(matrix[i]==3)
      {
      	matrix[i]=-1;
         matrix[i+10]=2;
		}
}


#endif
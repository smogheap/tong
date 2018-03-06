#ifndef _tetris_cpp
#define _tetris_cpp


#include "tetris.h"


const int TETSPEED=200;


matrix::matrix()
{
 	for(int i=0; i<200; i++)
   	grid[i]=0;
   for(int j=200; j<210; j++)
   	grid[j]=0;
}

bool matrix::isthere(int x)
{
	if(grid[x]!=0)
   	return true;
   else
   	return false;
}

void matrix::render()
{
	for(int i=0; i<200; i++)
     	plotbox(i, grid[i]);
	return;
}

void matrix::puthere(int* piece, int parts, int colr)
{
	for(int i=0; i<parts; i++)
   	grid[piece[i]]=colr;
   render();
   int checkline=piece[parts];
   bool bust;
   for(int j=checkline; j>checkline-7&&j>0; j--)
   {
   	bust=true;
		for(int k=0; k<10; k++)
	   	if(grid[piece[j]]==0)
         	bust=false;
		if(bust)
      	bustline(j);
   }
}





//end matrix, begin tetrad*****************************************



tetrad::tetrad()
{
	piece=new int[8];
	timer=0;
}


void tetrad::newpiece(matrix& bucket, int x)
{
   switch(x)
   {
   	case 0: //Z
			piece[0]=15; 	piece[1]=5;		piece[2]=4;		piece[3]=16;
			parts=4;
			break;
   	case 1: //S
			piece[0]=15;	piece[1]=6;		piece[2]=14;	piece[3]=5;
			parts=4;
			break;
   	case 2: //T
			piece[0]=15;	piece[1]=14;	piece[2]=5;		piece[3]=16;
			parts=4;
			break;
   	case 3: //J
			piece[0]=15;	piece[1]=14;	piece[2]=4;		piece[3]=16;
			parts=4;
			break;
   	case 4: //L
			piece[0]=15;	piece[1]=14;	piece[2]=6;		piece[3]=16;
			parts=4;
			break;
   	case 5: //B
			piece[0]=14;	piece[1]=5;		piece[2]=4;		piece[3]=15;
			parts=4;
			break;
   	default: //I
			piece[0]=4;		piece[1]=3;		piece[2]=5;		piece[3]=6;
			parts=4;
			break;
	}

	for(int i=0; i<parts; i++)
	{
   	if(bucket.isthere(piece[i]))
      {
//for(int j=0; j<parts; j++) log<<"piece["<<j<<"]="<<piece[j]<<"\n";
			exit(0);
      }
	}
   color=random(6)+9;
}


void tetrad::tick(matrix& bucket, int &tetrads, int check)
{
	switch(check)
   {
		case 80: //down
	      if(!down(bucket))
	      {
         	bucket.puthere(piece, parts, color);
	         tetrads--;
	      }
         break;
	   case 75: //left
         left(bucket);
         break;
	   case 77: //right
        	right(bucket);
         break;
      case 72: //up
      	rotate(bucket);
         break;
   }
	if(timer>TETSPEED)
   {
      if(!down(bucket))
      {
        	bucket.puthere(piece, parts, color);
         tetrads--;
      }
      else timer=0;
   }
   timer++;
   return;
}


bool tetrad::down(matrix& bucket)
{
	bool canfall=true;
	for(int l=0; l<parts; l++)
		if(bucket.isthere(piece[l]+10)) canfall=false;
	if(canfall)
		for(int i=0; i<parts; i++)
	   {
			remvbox(piece[i]);
	      piece[i]+=10;
         plotbox(piece[i], color);
	   }
   return canfall;
}


void tetrad::left(matrix& bucket)
{
	bool canleft=true;
	for(int l=0; l<parts; l++)
		if(bucket.isthere(piece[l]-1)!=0||piece[l]%10==0) canleft=false;
	if(canleft)
   {
		for(int i=0; i<parts; i++)
	   {
			remvbox(piece[i]);
	      piece[i]--;
	      plotbox(piece[i], color);
	   }
   }
   return;
}


void tetrad::right(matrix& bucket)
{
	bool canright=true;
	for(int l=0; l<parts; l++)
		if(bucket.isthere(piece[l]+1)!=0||piece[l]%10==9) canright=false;
	if(canright)
	{
		for(int i=0; i<parts; i++)
	   {
			remvbox(piece[i]);
	      piece[i]++;
	      plotbox(piece[i], color);
	   }
   }
   return;
}


void tetrad::rotate(matrix& bucket)
{
	int temp[8]={0,0,0,0,0,0,0,0};
	int xaway=0, yaway=0, taway=0;
	temp[0]=piece[0];
 	for(int i=1; i<parts; i++)
   {
     	xaway=(piece[i]%10)-(piece[0]%10);
      yaway=(piece[i]/10)-(piece[0]/10);
		taway=yaway;
      yaway=xaway;
      xaway=-1*taway;
      temp[i]=piece[0]+xaway+(yaway*10);
   }
	bool extleft=false;
   bool extright=false;
   bool outabounds=false;
	for(int k=0; k<parts; k++)
	   {
	   	if(temp[k]%10==0) extleft=true;
         if(temp[k]%10==9) extright=true;
         if(bucket.isthere(temp[k])||temp[k]<0) outabounds=true;
	   }
   if(!(extleft&&extright&&outabounds))
	   for(int j=0; j<parts; j++)
	   {
	   	remvbox(piece[j]);
	   	piece[j]=temp[j];
			plotbox(piece[j], color);
	   }
   return;
}


//*******************friends**********************************



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


void matrix::bustline(int x)
{
	for(int j=0; j<10; j++)
   	grid[x*10+j]=15;
	render();
	for(int k=4; k>-1; k--)
   {
      delay(50);
      remvbox(x*10+k);
      remvbox(x*10+9-k);
   }
	for(int i=x*10+9; i>-1; i--)
   	if(grid[i]==3)
      {
      	grid[i]=-1;
         grid[i+10]=2;
		}
}


#endif
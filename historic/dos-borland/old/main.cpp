#include <fstream.h>
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include "objects.cpp"

void initgpx();
void plotbox(int);
void dumpbox(int);
void remvbox(int);
//void title(int&);
//void config(options&);
//void play(options);
void render(int*);
ofstream log;

void main()
{
log.open("c:\\windows\\desktop\\log.txt");
log<<"opened log\n";
//	options opts;
	int *matrix=new int[200];
   for(int c=0; c<200; c++) matrix[c]=0;
   for(int d=190; d<200; d+=2) matrix[d]=2;
	int select=0;
   int ticker=0;
   int tetrads=0, balls=0;
   tetrad acttet[8];
//   ball actbal[8];
   bool addtetrad=true;
   bool addball=true;
   bool quit=false;
   int tick=0;
log<<"inited vars\n";
   initgpx();
   getch();
log<<"inited graphics\n";
	while(quit!=true)
   {
log<<"quit!=true";
      if(tetrads==0)
      {
      	addtetrad=true;
log<<"add tetrad=true\n";
		}
		if(addtetrad==true)
      {
			acttet[tetrads].newpiece(log, matrix);
	      tetrads++;
log<<"tetrads="<<tetrads<<'\n';
         addtetrad=false;
      }
		tick++;
log<<"tick="<<tick<<'\n';
		for(int i=0; i<tetrads; i++)
      {
			acttet[i].tick(tick, matrix, addtetrad, tetrads);
log<<"ticking tetrad "<<i<<'\n';
      }
      for(int j=0; j<tetrads; j++)
			for(int k=0; k<acttet[j].parts; k++)
				matrix[acttet[j].piece[k]]=1;
log<<"set active pieces in matrix to 1\n";
for(int l=0; l<20; l++)
{
 for(int m=0; m<10; m++)
  log<<matrix[(l*10+m)];
 log<<'\n';
}
      render(matrix);
log<<"rendered matrix\n";
      delay(100);
	}
   closegraph();
log<<"closed graphics\n";
log.close();
   delete[] matrix;
   return;
}

void title(int& x)
{
	setcolor(1);
	outtextxy((getmaxx()/2), 10, "TONG!");
	x=getch();
	return;
}

/*
void config(options& opts)
{
	return;
}
*/

void initgpx()
{
   int gdriver = 0, gmode, errorcode;
   initgraph(&gdriver, &gmode, "");
   errorcode = graphresult();

   if (errorcode != grOk)
   {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      printf("Press any key to halt:");
      getch();
      exit(1);
   }

   cleardevice();
}

void plotbox(int elem)
{
	int x=(elem%10)*20+20;
   int y=(elem/10)*20+20;
   setcolor(2);
   rectangle(x,y,x+19,y+19);
   setcolor(3);
   rectangle(x+2,y+2,x+17,y+17);
}

void dumpbox(int elem)
{
	int x=(elem%10)*20+20;
   int y=(elem/10)*20+20;
   setcolor(4);
   rectangle(x,y,x+19,y+19);
   setcolor(5);
   rectangle(x+2,y+2,x+17,y+17);
}

void remvbox(int elem)
{
	int x=(elem%10)*20+20;
   int y=(elem/10)*20+20;
   setcolor(0);
   rectangle(x,y,x+19,y+19);
   rectangle(x+2,y+2,x+17,y+17);
}

void render(int *matrix)
{
	for(int i=0; i<200; i++)
   {
		switch(matrix[i])
      {
      	case -1:
         	remvbox(i);
            matrix[i]=0;
log<<"removed box "<<i<<'\n';
            break;
			case 1:
         	plotbox(i);
	         matrix[i]=0;
log<<"drew box "<<i<<'\n';
            break;
         case 2:
         	dumpbox(i);
            matrix[i]=3;
      }
	}
	return;
}

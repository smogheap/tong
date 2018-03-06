/*
TONG! - Tetris meets Pong
(and a hell of a lot of options!)

Author:							Feedback:
Owen Swerkstrom				Chris Shephard
									Joe Wilker
Concept:
Owen Swerkstrom
Kris Holle
Ian Edhlund

Special Thanks:
Heidi Weinert
Laura Levenhagen
*/

//#include <fstream.h>
#include <graphics.h>
//#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include "tetris.cpp"
#include "pong.cpp"


const int TETSPEED=200;


void initgpx();
void drawboard();
unsigned char getscancode();
void msg(const char*);

//void render(int*, int&, int);
//void render(bool, int, int, int);

void main()
{
   initgpx();
	drawboard();
   randomize();
msg("Testing the messager");
	int *matrix=new int[210];
	   for(int c=0; c<200; c++) matrix[c]=0;
	   for(int d=200; d<210; d++) matrix[d]=3;

   matrix bucket;
   int tetrads=0;
   int totrads=0;			//total tetrads
   tetrad acttet[8];
   int colr=6;
   bool addtetrad=true;
//   int checkline=-1;
   int lines=0;
//   int series=1;

   paddle pads;
	ball actball[8];
	int balls=0;
   int toballs=0;			//total balls
	bool addball=true;
   int bounces=0;

	int check=getscancode();
msg("got scan code ");
cout<<check;

   bool quit=false;
	while(quit!=true)
   {
   check=getscancode();

      if(tetrads<1)
      	addtetrad=true;
		if(addtetrad==true)
      {
			colr=acttet[tetrads].newpiece(matrix, random(7));
	   	tetrads++;
         totrads++;
         gotoxy(50, 14);
         cout<<totrads;
		   addtetrad=false;
      }
		for(int i=0; i<tetrads; i++)
			acttet[i].tick(matrix, tetrads, check);
//      render(matrix, checkline, colr);


      pads.tick(check);

      if(balls<1)
			addball=true;
      if(addball==true)
      {
         actball[balls].newball();
      	balls++;
         toballs++;
         gotoxy(64, 14);
         cout<<toballs;
         addball=false;
      }
		else
      {
			for(int j=0; j<balls; j++)
		      actball[j].tick(pads, balls, matrix, bounces);
         gotoxy(64, 15);
	      cout<<bounces;
      }
      delay(5);
	}
   closegraph();
//log.close();
   delete[] matrix;
   return;
}


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


void drawboard()
{
   setcolor(8);
   rectangle(39,39,240,440);
	setcolor(15);
	settextstyle(0, 0, 4);
	outtextxy(340, 16, "TONG!");
	setcolor(8);
	outtextxy(342, 18, "TONG!");
	setcolor(7);
	outtextxy(341, 17, "TONG!");

	gotoxy(41, 14);
	cout<<"Tetrads: 0";
   gotoxy(41, 15);
   cout<<"Lines:   0";

   gotoxy(55, 14);
   cout<<"Balls:   0";
   gotoxy(55,15);
   cout<<"Bounces: 0";
}


unsigned char getscancode()
{
	asm {
   	mov ah, 01h
      int 16h
      jz empty
      mov ah, 00h
		int 16h
      mov al, ah
      xor ah, ah
      jmp done
   empty:
   	xor ax, ax
   done:
   }
}

void render(bool align, int pos, int length, int offset)
{
   if(align)
   {
		setcolor(0);
      rectangle(35,pos, 38,pos+length);
      rectangle(241,pos, 244,pos+length);
      setcolor(15);
      rectangle(35,pos, 38,pos+length);
      rectangle(241,pos, 244,pos+length);
   }
   else
   {
		setcolor(0);
      rectangle(35,pos, 38,pos+length);
      rectangle(441,pos, 444,pos+length);
      setcolor(15);
      rectangle(35,pos, 38,pos+length);
      rectangle(441,pos, 444,pos+length);
   }
   return;
}

void msg(const char* text)
{
	gotoxy(41, 20);
	cout<<text;
}

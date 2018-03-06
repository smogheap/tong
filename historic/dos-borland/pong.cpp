#ifndef _pong_cpp
#define _pong_cpp

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
   int isthere(int, int);
};

paddle::paddle()
{
	xpos=ypos=40;
   length=80;
   move(0, false);
   move(0, true);
}

void paddle::tick(int check)
{
	if(check!=0)
   {
  		switch(check)
	   {
			case 17: //up
msg("scanned an up");
         	move(-20, true);
	         break;
	      case 44: //down
msg("scanned a down");
         	move(20, true);
	         break;
      }
  		switch(check)
	   {
		   case 30: //left
msg("scanned a left");
         	move(-20, false);
	         break;
		   case 31: //right
msg("scanned a right");
         	move(20, false);
	         break;
      }
   }
   return;
}

int paddle::isthere(int x, int y)
{
	int temp=1;
   if(xpos<x&&x<xpos+length)
		temp*=2;
	if(ypos<y&&y<ypos+length)
   	temp*=3;
   return temp;
}

void paddle::move(int x, bool align)
{
	if(!align)
   {
   	if(30<xpos+x&&xpos+length+x<249)
      {
			setcolor(0);
	      rectangle(xpos,35, xpos+length,38);
	      rectangle(xpos,441, xpos+length,444);
			xpos+=x;
	//      render(align, pos, length, x);

	      setcolor(15);
	      rectangle(xpos,35, xpos+length,38);
	      rectangle(xpos,441, xpos+length,444);
      }
	}
   else
   {
   	if(30<ypos+x&&ypos+length+x<449)
      {
			setcolor(0);
	      rectangle(35,ypos, 38,ypos+length);
	      rectangle(241,ypos, 244,ypos+length);
			ypos+=x;
	//      render(align, pos, length, x);

	      setcolor(15);
	      rectangle(35,ypos, 38,ypos+length);
	      rectangle(241,ypos, 244,ypos+length);
		}
   }
   return;
}


class ball
{
	int x, y;
   int rise, run;
   int wasx, wasy;
   int timer;
public:
	ball();
   void newball();
	int boxthere(int, int, int*);
   void tick(paddle&, int&, int*);
};

ball::ball()
{
	x=y=60;
   rise=1;
   run=1;
   timer=0;
   wasx=wasy=20;
}

void ball::newball()
{
	x=y=60;
   rise=1;
   run=1;
   timer=0;
   wasx=wasy=20;
}

int ball::boxthere(int x, int y, int* matrix)
{
	int temp=1;
//   if((1<matrix[(x-40)/10-1] || 1<matrix[(x-40)/10+1]) && 1<matrix[(y-40)/10])
	if(1<matrix[(x-40)/20-1] || 1<matrix[(x-40)/20+1])
   {
cout<<(x-40)/20-1;
//msg(" M A T R I X  I N T E R S E C T I O N");
		temp*=2;
	}
//	if((1<matrix[(y-40)/10-10] || 1<matrix[(y-40)/10+10]) && 1<matrix[(x-40)/10])
	if(1<matrix[(y-40)/20-10] || 1<matrix[(y-40)/20+10])
   {
cout<<(y-40)/20-10;
//msg(" M A T R I X  I N T E R S E C T I O N");
   	temp*=3;
	}
   return temp;
}

void ball::tick(paddle& pads, int& balls, int* matrix)
{
	if(timer%(10-rise)==0)
   {
msg("tick met rise");
		if(rise>0) wasy=y++;
      else wasy=y--;
   }
   if(timer%(10-run)==0)
   {
msg("tick met run");
    	if(run>0) wasx=x++;
      else wasx=x--;
   }

   if(x<40||x>239||y<40||y>439)	//out of bounds
   {
		balls--;
      setcolor(1);
      circle(x, y, 9);
	   setcolor(8);
	   rectangle(39,39,240,440);
   }

//   bounce x
   if(((x<50||x>229)&&(pads.isthere(x, y))%3==0) || boxthere(x, y, matrix)%3==0)
   {
		run*=-1;
      x+=wasx-x;
   }

//   bounce y
   if(((y<50||y>429)&&(pads.isthere(x, y))%2==0) || boxthere(x, y, matrix)%2==0)
   {
		rise*=-1;
      y+=wasy-y;
   }

	if(wasx!=x||wasy!=y)
   {
   	setcolor(0);
		circle(wasx, wasy, 9);
		setcolor(15);
      circle(x, y, 9);
   }
   if(wasx!=x&&wasy!=y)
   	timer=0;
   timer++;
   wasx=x;
   wasy=y;
	return;
}


#endif
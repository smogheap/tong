#ifndef _pong_cpp
#define _pong_cpp


#include "pong.h"


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
//msg("scanned an up");
         	move(-20, true);
	         break;
	      case 44: //down
//msg("scanned a down");
         	move(20, true);
	         break;
      }
  		switch(check)
	   {
		   case 30: //left
//msg("scanned a left");
         	move(-20, false);
	         break;
		   case 31: //right
//msg("scanned a right");
         	move(20, false);
	         break;
      }
   }
   return;
}

int paddle::isthere(int x, int y, int& bounces)
{
	int temp=1;
   if(xpos<x&&x<xpos+length)
   {
  		bounces++;
		temp*=2;
   }
	if(ypos<y&&y<ypos+length)
   {
		bounces++;
   	temp*=3;
   }
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


//********end paddles begin ball*******************


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
   setcolor(0);
   circle(x, y, 9);
	setcolor(8);
	rectangle(39,39,240,440);
msg("erased ball");
	x=y=60;
   rise=random(4)+1;
   run=random(4)+1;
   timer=0;
   wasx=wasy=20;
}

int ball::boxthere(int x, int y, matrix& bucket)
{
	int temp=1;
   int place=((y-50)/20*10)+(x-30)/20;
	if((bucket.isthere(place-1) || bucket.isthere(place+1)) && place>1)
   {
		temp*=3;
msg("bounced at ");
cout<<place<<"   ";
	}
	if((bucket.isthere(place-10) || bucket.isthere(place+10)) && place>9)
   {
   	temp*=2;
msg("bounced at ");
cout<<place<<"   ";
	}
   return temp;
}

void ball::tick(paddle& pads, int& balls, matrix& bucket, int& bounces)
{
	if(timer%(10-rise)==0)
   {
//msg("tick met rise");
		wasy=y;
	   y+=rise;
   }
   if(timer%(10-run)==0)
   {
//msg("tick met run");
    	wasx=x;
      x+=rise;
   }

   if(x<40||x>239||y<40||y>439)	//out of bounds
		balls--;

   xy=(y-40)/20+(x-40)%20;

//   bounce x
   if( ((x<50||x>229)&&(pads.isthere(x,y, bounces))%3==0) ||
   	bucket.isthere(xy-1) || bucket.isthere(xy+1) )
   {
		run*=-1;
      wasx=x;
      x+=run;
   }

//   bounce y
   if( ((y<50||y>429)&&(pads.isthere(x, y, bounces))%2==0) ||
   	bucket.isthere(xy-10) || bucket.isthere(xy+10) )
   {
		rise*=-1;
      wasy=y;
      y+=rise;
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
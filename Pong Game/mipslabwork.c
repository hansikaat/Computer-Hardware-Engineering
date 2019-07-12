/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;
int timeoutcount=0;
int prime = 1234567;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
 //  volatile  int* portE = (volatile int*) 0xbf886110;
 //  if ( IFS(0) & 0x8000){
 //     *portE = *portE + 0x1;
 //      IFS(0) &= 0xffff7fff;
 //  }
 //  if (IFS(0) & 0X100){
 //   if ( timeoutcount < 10){
 //     timeoutcount++;
 //   }
 //  else{
 //      time2string( textstring, mytime );
 //      display_string( 3, textstring );
 //      display_update();
 //      tick( &mytime );
 //      timeoutcount=0;
 //   }
 //   IFS(0) &= 0xfffffeff;
 // }
 return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
 volatile int*E = (volatile int*) 0xbf886100;
 *E = *E & 0xff00;
  TRISD = TRISD & 0x0FE0;
  enable_interrupt();
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
 //volatile int * portE = (volatile int*) 0xbf886110;
  //*portE = 0x00;
  //*portE = *portE + 0x1;

  int BTN = getbtns();
  int SW =  getsw();

  if (BTN == 1 || BTN == 3 || BTN == 5 || BTN == 7){
  mytime = mytime & 0xFF0F;
  mytime = (SW << 4) | mytime;
  }


 if ( BTN == 2 || BTN == 3|| BTN == 6|| BTN == 7){
 mytime = mytime & 0xF0FF;
 mytime = (SW<<8) | mytime;
 }

 if (BTN == 4|| BTN == 5|| BTN == 6|| BTN ==7){
 mytime = mytime & 0x0FFF;
 mytime = (SW<< 12) | mytime;
 }


  delay( 1000 );
  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );

  volatile int * portE = (volatile int*) 0xbf886110;
  *portE = *portE + 0x1;
  display_image(96, icon);
}

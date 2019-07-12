/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#define PADDLE_HEIGHT 8
#define PADDLE_HEIGHT_TURTLE 10
#define PADDLE_HEIGHT_RABBIT 8
#define PADDLE_HEIGHT_HORSE 6

int main(void) {

		    /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */

	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;

	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;

	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);

	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;

	display_init();


	labinit(); /* Do any lab-specific initialization */

	//initialization of variables
	int ballX = 10;
	int ballY = 10;
	int paddle1Y= 5;
	int paddle1X = 5;
	int paddle2Y= 5;
	int paddle2X = 123;
	int ballspeedX= 1;
	int ballspeedY= 1;
	int ballspeedXTurtle= 1;
	int ballspeedYTurtle= 1;
	int ballspeedXRabbit= 3;
	int ballspeedYRabbit= 3;
	int ballspeedXHorse= 4;
	int ballspeedYHorse= 4;
	int paddlespeedY1 = 1;
	int paddlespeedY2 = 1;
	int score=0;
	int player1score=0;
	int player2score=0;
	int menu =0;
	int menu1 =0;
	int menu2 =0;
	int menu3 =0;
	int player;
	int c;
	int level=0;
	int p1=60;
	int p2=70;
	int p3=67;
	int target;

 //draw logo
	while(menu3==0){
		display_image(0,logo);
		lightUpLED2();
		delay(10000000);
		menu3=1;
	}
  menu3=0;
  lightDownLED();

	//select player mode
	while(menu==0){
		drawPlayerMode();
		if(getbtns() & 0x4){
			player=1;
			menu=1;
			clearString();
			drawLevelMode();
		}
		else if(getbtns() & 0x2){
			player=2;
			menu=1;
			clearString();
			drawLevelMode();
		}
	}
	menu=0;

	//select level
	while(menu==0){
			if(getsw() & 0x8){
				level=1;
				menu=1;
				clearString();
				drawInstruction();
				delay(10000000);
				clearString();
			}
			else if(getsw() & 0x4){
				level=2;
				menu=1;
				clearString();
				drawInstruction();
				delay(10000000);
				clearString();
			}
			else if(getsw() & 0x2){
				level=3;
				menu=1;
				clearString();
				drawInstruction();
				delay(10000000);
				clearString();
			}
			else if(getsw() & 0x1){
				clearString();
				main();
			}
	}
	menu=0;

	//play game
	while(player1score<4 && player2score<4){
		if(getsw() & 0x1){
			clearGame();
			main();
		}

	//--------------player1, level1-------------------------------------------------------------------------
		if(player==1 && level==1){

		//draw paddle1
		drawPaddleTurtle(paddle1X, paddle1Y);
		//draw paddle2
    drawPaddleTurtle(paddle2X, paddle2Y);

		//movepaddle1
   	if(getbtns() & 0x4){
			paddle1Y++;
			if (paddle1Y <= 0) {
				 paddle1Y = 0;
				 paddlespeedY1 *= (-1);
			}
			else if (paddle1Y >= 32 - PADDLE_HEIGHT_TURTLE) {
				 paddle1Y =32-PADDLE_HEIGHT_TURTLE;
				 paddlespeedY1 *= (-1);
			}
		}
		else if (getbtns() & 0x2){
			paddle1Y--;
			if (paddle1Y <= 0) {
				 paddle1Y = 0;
				 paddlespeedY1 *= (-1);
			}
			else if (paddle1Y >= 32 - PADDLE_HEIGHT_TURTLE) {
				 paddle1Y =32-PADDLE_HEIGHT_TURTLE;
				 paddlespeedY1 *= (-1);
			}
		}

		//movepaddle2
		if (ballY == 5) {
       paddlespeedY2 = 1;
    } else if (ballY == 32 - 5) {
			 paddlespeedY2 = -1;
    }

    if (paddlespeedY2 == 1 && paddle2Y < 23 && ballX > 100) {
       paddle2Y++;
    } else if (paddlespeedY2 == -1 && paddle2Y > 0 && ballX > 100) {
       paddle2Y--;
    }


		//move balls
		ballX = (ballX + ballspeedXTurtle);
    ballY = (ballY + ballspeedYTurtle);

    // vertical collision detection
    if (ballY <= 5) {
        ballY = 5;
        ballspeedYTurtle *= (-1);
    }
		else if (ballY >= 32 - 5) {
        ballY = 32 - 5;
        ballspeedYTurtle *= (-1);
    }
		// horizontal collision detection
		if (ballX<= 5) {
			 if (ballY < paddle1Y || ballY > paddle1Y + PADDLE_HEIGHT_TURTLE - 1) { player2score += 1; }
			 ballX = 5;
		 	 ballspeedXTurtle *= (-1);
		}
		else if (ballX >= 123 - 1) {
			  if (ballY < paddle2Y || ballY > paddle2Y + PADDLE_HEIGHT_TURTLE - 1) { player1score += 1; }
				ballX = 123 - 1;
				ballspeedXTurtle *= (-1);
	  }

		//draw score
		drawScore(player1score,player2score,p1,p2);
		borderLine(p3);

		//clear ball
		clearGame();

		//draw ball
		drawBall(ballX,ballY);
		}
		//--------------------------end of player 1,level 1-------------------------------------------------------//

		//--------------player1, level2-------------------------------------------------------------------------
		else if(player==1 && level==2){

			//draw paddle1
			drawPaddleRabbit(paddle1X, paddle1Y);
			//draw paddle2
		  drawPaddleRabbit(paddle2X, paddle2Y);

		 //movepaddle1
		 	if(getbtns() & 0x4){
				paddle1Y++;
				if (paddle1Y <= 0) {
					 paddle1Y = 0;
					 paddlespeedY1 *= (-1);
				}
				else if (paddle1Y >= 32 - PADDLE_HEIGHT_RABBIT) {
					 paddle1Y =32-PADDLE_HEIGHT_RABBIT;
					 paddlespeedY1 *= (-1);
				}
			}
			else if (getbtns() & 0x2){
				paddle1Y--;
				if (paddle1Y <= 0) {
					 paddle1Y = 0;
					 paddlespeedY1 *= (-1);
				}
				else if (paddle1Y >= 32 - PADDLE_HEIGHT_RABBIT) {
					 paddle1Y =32-PADDLE_HEIGHT_RABBIT;
					 paddlespeedY1 *= (-1);
				}
			}

			//movepaddle2
			if (ballX == 100 && ballspeedXRabbit > 0) {
					target = 32 - 5 - ballY;
			} else if (ballX == 101 && ballspeedXRabbit > 0) {
					target = 32 - 5 - (ballY - (paddlespeedY2));
			}

			if (paddle2Y < 23 && paddle2Y + 6 < target) {
					paddle2Y++;
			} else if (paddle2Y > 0 && paddle2Y + 6 > target) {
					paddle2Y--;
			}

			//move balls
			ballX = (ballX + ballspeedXRabbit);
		  ballY = (ballY + ballspeedYRabbit);

		   // vertical collision detection
		  if (ballY <= 5) {
		      ballY = 5;
		      ballspeedYRabbit *= (-1);
	    }
			else if (ballY >= 32 - 5) {
		      ballY = 32 - 5;
		      ballspeedYRabbit *= (-1);
		  }

		 // horizontal collision detection
			if (ballX<= 5) {
				 if (ballY < paddle1Y || ballY > paddle1Y + PADDLE_HEIGHT_RABBIT - 1) { player2score += 1; }
				 ballX = 5;
			 	 ballspeedXRabbit *= (-1);
			}
			else if (ballX >= 123 - 1) {
				 if (ballY < paddle2Y || ballY > paddle2Y + PADDLE_HEIGHT_RABBIT - 1) { player1score += 1; }
					ballX = 123 - 1;
					ballspeedXRabbit *= (-1);
			 }

			//draw score
			drawScore(player1score,player2score,p1,p2);

			//clear ball
			clearGame();

			//draw ball
			drawBall(ballX,ballY);

		}
		//--------------------------end of player 1,level 2-------------------------------------------------------//

		//--------------player1, level3-------------------------------------------------------------------------
		if(player==1 && level==3){

				//draw paddle1
				drawPaddleHorse(paddle1X, paddle1Y);
				//draw paddle2
		    drawPaddleHorse(paddle2X, paddle2Y);

				//movepaddle1
		   	if(getbtns() & 0x4){
					paddle1Y++;
					if (paddle1Y <= 0) {
						 paddle1Y = 0;
						 paddlespeedY1 *= (-1);
					}
					else if (paddle1Y >= 32 - PADDLE_HEIGHT_HORSE) {
						 paddle1Y =32-PADDLE_HEIGHT_HORSE;
						 paddlespeedY1 *= (-1);
					}
				}
				else if (getbtns() & 0x2){
					paddle1Y--;
					if (paddle1Y <= 0) {
						 paddle1Y = 0;
						 paddlespeedY1 *= (-1);
					}
					else if (paddle1Y >= 32 - PADDLE_HEIGHT_HORSE) {
						 paddle1Y =32-PADDLE_HEIGHT_HORSE;
						 paddlespeedY1 *= (-1);
					}
				}

				//movepaddle2
				if (ballX == 100 && ballspeedXHorse > 0) {
						target = 32 - 5 - ballY;
				} else if (ballX == 101 && ballspeedXHorse > 0) {
						target = 32 - 5 - (ballY - (paddlespeedY2));
				}

				if (paddle2Y < 23 && paddle2Y + 8 < target) {
						paddle2Y++;
				} else if (paddle2Y > 0 && paddle2Y + 8 > target) {
						paddle2Y--;
				}

				//move balls
				ballX = (ballX + ballspeedXHorse);
		    ballY = (ballY + ballspeedYHorse);

		    // vertical collision detection
		    if (ballY <= 5) {
		        ballY = 5;
		        ballspeedYHorse *= (-1);
		    }
				else if (ballY >= 32 - 5) {
		        ballY = 32 - 5;
		        ballspeedYHorse *= (-1);
		    }

				// horizontal collision detection
				if (ballX<= 5) {
					 if (ballY < paddle1Y || ballY > paddle1Y + PADDLE_HEIGHT_HORSE - 1) { player2score += 1; }
					 ballX = 5;
				 	 ballspeedXHorse *= (-1);
				}
				else if (ballX >= 123 - 1) {
					  if (ballY < paddle2Y || ballY > paddle2Y + PADDLE_HEIGHT_HORSE - 1) { player1score += 1; }
						ballX = 123 - 1;
						ballspeedXHorse *= (-1);
			  }

				//draw score
				drawScore(player1score,player2score,p1,p2);

				//clear ball
				clearGame();

				//draw ball
				drawBall(ballX,ballY);
			}
		//--------------------------end of player 1,level 3-------------------------------------------------------//

		//--------------player2, level1---------------------------------------------------------------------------
		if(player==2 && level==1){

		//draw paddle1
		drawPaddleTurtle(paddle1X, paddle1Y);
		//draw paddle2
    drawPaddleTurtle(paddle2X, paddle2Y);

		//movepaddle1
   	if(getbtns() & 0x4){
			paddle1Y++;
			if (paddle1Y <= 0) {
				 paddle1Y = 0;
				 paddlespeedY1 *= (-1);
			}
			else if (paddle1Y >= 32 - PADDLE_HEIGHT_TURTLE) {
				 paddle1Y =32-PADDLE_HEIGHT_TURTLE;
				 paddlespeedY1 *= (-1);
			}
		}
		else if (getbtns() & 0x2){
			paddle1Y--;
			if (paddle1Y <= 0) {
				 paddle1Y = 0;
				 paddlespeedY1 *= (-1);
			}
			else if (paddle1Y >= 32 - PADDLE_HEIGHT_TURTLE) {
				 paddle1Y =32-PADDLE_HEIGHT_TURTLE;
				 paddlespeedY1 *= (-1);
			}
		}

		//movepaddle2
		if(getbtns() & 0x1){
			paddle2Y++;
			if (paddle2Y <= 0) {
				 paddle2Y = 0;
				 paddlespeedY2 *= (-1);
			}
			else if (paddle2Y >= 32 - PADDLE_HEIGHT_TURTLE) {
				 paddle2Y =32-PADDLE_HEIGHT_TURTLE;
				 paddlespeedY2 *= (-1);
			}
		}
		else if (getbtn() & 0x1){
			paddle2Y--;
			if (paddle2Y <= 0) {
				 paddle2Y = 0;
				 paddlespeedY2 *= (-1);
			}
			else if (paddle2Y >= 32 - PADDLE_HEIGHT_TURTLE) {
				 paddle2Y =32-PADDLE_HEIGHT_TURTLE;
				 paddlespeedY2 *= (-1);
			}
		}

		//move balls
		ballX = (ballX + ballspeedXTurtle);
    ballY = (ballY + ballspeedYTurtle);

    // vertical collision detection
    if (ballY <= 5) {
        ballY = 5;
        ballspeedYTurtle *= (-1);
    }
		else if (ballY >= 32 - 5) {
        ballY = 32 - 5;
        ballspeedYTurtle *= (-1);
    }

		// horizontal collision detection
		if (ballX<= 5) {
			 if (ballY < paddle1Y || ballY > paddle1Y + PADDLE_HEIGHT_TURTLE - 1) { player2score += 1; }
			 ballX = 5;
		 	 ballspeedXTurtle *= (-1);
		}
		else if (ballX >= 123 - 1) {
			  if (ballY < paddle2Y || ballY > paddle2Y + PADDLE_HEIGHT_TURTLE - 1) { player1score += 1; }
				ballX = 123 - 1;
				ballspeedXTurtle *= (-1);
	  }

		//draw score
		drawScore(player1score,player2score,p1,p2);

		//clear ball
		clearGame();

		//draw ball
		drawBall(ballX,ballY);

		}

	//----------------------------------end of player 2, level 1---------------------------------------//


	//--------------player2, level2-------------------------------------------------------------------------
	else if(player==2 && level==2){

		//draw paddle1
		drawPaddleRabbit(paddle1X, paddle1Y);
		//draw paddle2
		drawPaddleRabbit(paddle2X, paddle2Y);

	 //movepaddle1
		if(getbtns() & 0x4){
			paddle1Y++;
			if (paddle1Y <= 0) {
				 paddle1Y = 0;
				 paddlespeedY1 *= (-1);
			}
			else if (paddle1Y >= 32 - PADDLE_HEIGHT_RABBIT) {
				 paddle1Y =32-PADDLE_HEIGHT_RABBIT;
				 paddlespeedY1 *= (-1);
			}
		}
		else if (getbtns() & 0x2){
			paddle1Y--;
			if (paddle1Y <= 0) {
				 paddle1Y = 0;
				 paddlespeedY1 *= (-1);
			}
			else if (paddle1Y >= 32 - PADDLE_HEIGHT_RABBIT) {
				 paddle1Y =32-PADDLE_HEIGHT_RABBIT;
				 paddlespeedY1 *= (-1);
			}
		}

		//movepaddle2
		if(getbtns() & 0x1){
			paddle2Y++;
			if (paddle2Y <= 0) {
				 paddle2Y = 0;
				 paddlespeedY2 *= (-1);
			}
			else if (paddle2Y >= 32 - PADDLE_HEIGHT_RABBIT) {
				 paddle2Y =32-PADDLE_HEIGHT_RABBIT;
				 paddlespeedY2 *= (-1);
			}
		}
		else if (getbtn() & 0x1){
			paddle2Y--;
			if (paddle2Y <= 0) {
				 paddle2Y = 0;
				 paddlespeedY2 *= (-1);
			}
			else if (paddle2Y >= 32 - PADDLE_HEIGHT_RABBIT) {
				 paddle2Y =32-PADDLE_HEIGHT_RABBIT;
				 paddlespeedY2 *= (-1);
			}
		}

		//move balls
		ballX = (ballX + ballspeedXRabbit);
		ballY = (ballY + ballspeedYRabbit);

		// vertical collision detection
		if (ballY <= 5) {
				ballY = 5;
				ballspeedYRabbit *= (-1);
		}
		else if (ballY >= 32 - 5) {
				ballY = 32 - 5;
				ballspeedYRabbit *= (-1);
		}

	 	// horizontal collision detection
		if (ballX<= 5) {
			 if (ballY < paddle1Y || ballY > paddle1Y + PADDLE_HEIGHT_RABBIT - 1) { player2score += 1; }
			 ballX = 5;
			 ballspeedXRabbit *= (-1);
		}
		else if (ballX >= 123 - 1) {
			 if (ballY < paddle2Y || ballY > paddle2Y + PADDLE_HEIGHT_RABBIT - 1) { player1score += 1; }
				ballX = 123 - 1;
				ballspeedXRabbit *= (-1);
		 }

		//draw score
		drawScore(player1score,player2score,p1,p2);

		//clear ball
		clearGame();

		//draw ball
		drawBall(ballX,ballY);
	}
		//--------------------------end of player 2,level 2-------------------------------------------------------//

		//--------------player2, level3---------------------------------------------------------------------------
		if(player==2 && level==3){

				//draw paddle1
				drawPaddleHorse(paddle1X, paddle1Y);
				//draw paddle2
		    drawPaddleHorse(paddle2X, paddle2Y);

				//movepaddle1
		   	if(getbtns() & 0x4){
					paddle1Y++;
					if (paddle1Y <= 0) {
						 paddle1Y = 0;
						 paddlespeedY1 *= (-1);
					}
					else if (paddle1Y >= 32 - PADDLE_HEIGHT_HORSE) {
						 paddle1Y =32-PADDLE_HEIGHT_HORSE;
						 paddlespeedY1 *= (-1);
					}
				}
				else if (getbtns() & 0x2){
					paddle1Y--;
					if (paddle1Y <= 0) {
						 paddle1Y = 0;
						 paddlespeedY1 *= (-1);
					}
					else if (paddle1Y >= 32 - PADDLE_HEIGHT_HORSE) {
						 paddle1Y =32-PADDLE_HEIGHT_HORSE;
						 paddlespeedY1 *= (-1);
					}
				}

				//movepaddle2
				if(getbtns() & 0x1){
					paddle2Y++;
					if (paddle2Y <= 0) {
						 paddle2Y = 0;
						 paddlespeedY2 *= (-1);
					}
					else if (paddle2Y >= 32 - PADDLE_HEIGHT_HORSE) {
						 paddle2Y =32-PADDLE_HEIGHT_HORSE;
						 paddlespeedY2 *= (-1);
					}
				}
				else if (getbtn() & 0x1){
					paddle2Y--;
					if (paddle2Y <= 0) {
						 paddle2Y = 0;
						 paddlespeedY2 *= (-1);
					}
					else if (paddle2Y >= 32 - PADDLE_HEIGHT_HORSE) {
						 paddle2Y =32-PADDLE_HEIGHT_HORSE;
						 paddlespeedY2 *= (-1);
					}
				}

				//move balls
				ballX = (ballX + ballspeedXHorse);
		    ballY = (ballY + ballspeedYHorse);

		    // vertical collision detection
		    if (ballY <= 5) {
		        ballY = 5;
		        ballspeedYHorse *= (-1);
		    }
				else if (ballY >= 32 - 5) {
		        ballY = 32 - 5;
		        ballspeedYHorse *= (-1);
		    }

				// horizontal collision detection
				if (ballX<= 5) {
					 if (ballY < paddle1Y || ballY > paddle1Y + PADDLE_HEIGHT_HORSE - 1) { player2score += 1; }
					 ballX = 5;
				 	 ballspeedXHorse *= (-1);
				}
				else if (ballX >= 123 - 1) {
					  if (ballY < paddle2Y || ballY > paddle2Y + PADDLE_HEIGHT_HORSE - 1) { player1score += 1; }
						ballX = 123 - 1;
						ballspeedXHorse *= (-1);
			  }

				//draw score
				drawScore(player1score,player2score,p1,p2);

				//clear ball
				clearGame();

				//draw ball
				drawBall(ballX,ballY);
			}
		//----------------------------------end of player 2, level 3---------------------------------------//
  display_image(0,icon);
	}

	//draw Winner
	while(menu2==0){
		drawWinner(player1score,player2score);
		lightUpLED1();
		menu2=1;
		player1score=0;
		player2score=0;
	}

	main();
	return 0;
}

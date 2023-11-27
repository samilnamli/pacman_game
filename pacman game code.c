
 */ 
//#include "ugfx_2.9/gfx.h"
#include "MKL25Z4.h"
//#include <time.h>


#define RESET 0x0100    /* PTC8 reset pin */
#define DC    0x0001    /* PTD0 register select pin */
#define CE    0x0200    /* PTC9 chip select */

/* define the pixel size of display */
#define GLCD_WIDTH  84
#define GLCD_HEIGHT 48

/* 
NOKIA LCD

D0-> PTC0
D1-> PTC1
D2-> PTC2
D3-> PTC3
D4-> PTC4
D5-> PTC5
D6-> PTC6
D7-> PTC7

RS-> PTC13
RW-> PTC12
EN-> PTC11


*/
#define RS 0x04     /* PTA2 mask */ 
#define RW 0x10     /* PTA4 mask */ 
#define EN 0x20     /* PTA5 mask */


/* Define Functions*/
void Delay(volatile unsigned int time_del);
void draw_pacman(int x,int y,int xold,int yold,int dir);
void GLCD_setCursor(unsigned char x, unsigned char y);
void GLCD_clear(void);
void GLCD_init(void);
void GLCD_data_write(unsigned char data); 
void GLCD_command_write(unsigned char data);
void SPI0_init(void);
void SPI0_write(unsigned char data);
void GLCD_putchar(int c);
void draw_dot(int x,int y);
void draw_string(int x,int y);
void draw_middlewall(int x, int y);
void draw_upperstring(void);
void draw_upperstring2(void);
void draw_lowerstring(void);
void draw_middlewall2(int x,int y);
void eat_pacman(void);
void eat_food(void);
void eat_dot(void);
void initialize_timer(void);
void draw_solkenar(void);
void draw_solaltkenar(void);

void draw_sagkenar(void);
void draw_sagaltkenar(void);
void GLCD_writeLetter( int c);

void GLCD2_setCursor(unsigned char x, unsigned char y);
void GLCD2_clear(void);
void GLCD2_init(void);
void GLCD2_data_write(unsigned char data); 
void GLCD2_command_write(unsigned char data);
void SPI1_init(void);
void SPI1_write(unsigned char data);
void GLCD2_putchar(int c);

void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
void LCD_ready(void);

void game_over_pacman(void);
void game_over_ghost(void);


void ADC0_init(void);

/* Define Variables */
unsigned int msTime = 350000;
unsigned int x = 42;
unsigned int y = 2;
unsigned int xold = 0;
unsigned int yold = 0;
unsigned foodamount = 1;

unsigned int randomx;
unsigned int randomy;

unsigned int x2 = 72;
unsigned int y2 = 4;
unsigned int xold2 = 0;
unsigned int yold2 = 0;



volatile unsigned int food1x;
volatile unsigned int food1y;
unsigned int food2x;
unsigned int food2y;



unsigned int b1x = 2;
unsigned int b1y = 1;

unsigned int b3x = 12;
unsigned int b3y = 1;

unsigned int b5x = 30;
unsigned int b5y = 1;


unsigned int b7x = 44;
unsigned int b7y = 1;


unsigned int b9x = 58;
unsigned int b9y = 1;


unsigned int b11x = 72;
unsigned int b11y = 1;

unsigned int b12x = 2;
unsigned int b12y = 5;


unsigned int b18x = 44;
unsigned int b18y = 5;


unsigned int b20x = 23;
unsigned int b20y = 2;


unsigned int b23x = 23;
unsigned int b23y = 0;
	

unsigned int b24x = 23;
unsigned int b24y = 3;


unsigned int b28x = 23;
unsigned int b28y = 5;

unsigned int b29x = 58;
unsigned int b29y = 0;

unsigned int b30x = 58;
unsigned int b30y = 2;


unsigned int b32x = 58;
unsigned int b32y = 3;

unsigned int b34x = 58;
unsigned int b34y = 5;


char letters[][8] = {
    {0xfe, 0xff, 0x31, 0x31, 0x31, 0x31, 0xff, 0xfe},
    {0xff, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {0xff, 0xff, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb},
    {0xff, 0xff, 0xc3, 0xc3, 0xdb, 0xdb, 0xdb, 0xfb},
    {0xff, 0xff, 0x18, 0x18, 0x18, 0x18, 0xff, 0xff},
    {0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
    {0xff, 0xff, 0x03, 0xff, 0xff, 0x03, 0xff, 0xff},
    {0xff, 0xff, 0x07, 0x1e, 0x78, 0xe0, 0xff, 0xff},
    {0xff, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xff},
    {0xff, 0xff, 0x1b, 0x1b, 0x1b, 0x1b, 0x1f, 0x1f},
    {0xff, 0xff, 0x13, 0x33, 0x73, 0xf3, 0xdf, 0x9f},
    {0xdf, 0xdf, 0xdb, 0xdb, 0xdb, 0xdb, 0xfb, 0xfb},
    {0x03, 0x03, 0x03, 0xff, 0xff, 0x03, 0x03, 0x03},
    {0x1f, 0x7c, 0xe0, 0xc0, 0xc0, 0xe0, 0x7c, 0x1f},
    {0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff}
};


 char game_table[][8] = {
	  { 0x3c, 0x7e, 0xff, 0xff, 0xff, 0xe7, 0xc3, 0x81},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00},
		{0x81, 0xc3, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c},
		{0x3f, 0x7e, 0xfc, 0xf8, 0xf8, 0xfc, 0x7e, 0x3f},
		{0xfc, 0x7e, 0x3f, 0x1f, 0x1f, 0x1f, 0x7e, 0xfc},
		{0xa5, 0xff, 0xff, 0xdd, 0xab, 0xdd, 0xfe, 0x18},
		{0xa5, 0xff, 0xff, 0xbb, 0x55, 0xbb, 0xfe, 0x18},
	  {0x18, 0x7f, 0xdd, 0xaa, 0xdd, 0xff, 0xff, 0xa5},
    {0x18, 0xfe, 0xbb, 0x55, 0xfb, 0xff, 0xff, 0xa5},
    {0x0e, 0x1f, 0x3e, 0x7c, 0x3e, 0x1f, 0x0e, 0x00},		
		{0x06, 0x0f, 0x0f, 0x06,0x00,0x00,0x00,0x00}
    }; 
    
		
int main(void) {


	
	
unsigned int value=0;
unsigned int oldvalue =0;
volatile int skor = 0;
volatile short int result;
volatile short int result2;
	
volatile short int second1;
volatile short int second2;	


	
	
//init_sysTick();
GLCD_init();    /* initialize the GLCD controller */
GLCD_clear();   /* clear display and home the cursor */
ADC0_init();

initialize_timer();
__enable_irq();

/* Print Loading Screen of the Game*/

GLCD_command_write(0xB8);	

GLCD_setCursor(10,0);

GLCD_writeLetter(14);
GLCD_setCursor(20,0);
GLCD_writeLetter(2);
GLCD_setCursor(30,0);
GLCD_writeLetter(5);
GLCD_setCursor(40,0);
GLCD_writeLetter(1);
GLCD_setCursor(50,0);
GLCD_writeLetter(8);
GLCD_setCursor(60,0);
GLCD_writeLetter(6);
GLCD_setCursor(70,0);
GLCD_writeLetter(2);

GLCD_setCursor (35,2);
GLCD_writeLetter(12);
GLCD_setCursor (45,2);
GLCD_writeLetter(8);


GLCD_setCursor(15,4);
GLCD_writeLetter(9);
GLCD_setCursor(25,4);
GLCD_writeLetter(0);
GLCD_setCursor(35,4);
GLCD_writeLetter(1);
GLCD_setCursor(45,4);
GLCD_writeLetter(6);
GLCD_setCursor(55,4);
GLCD_writeLetter(0);
GLCD_setCursor(65,4);
GLCD_writeLetter(7);








/* Read data for Joysticks*/

while(!(result > 4000) ){

ADC0->SC1[0] = 0; /* start conversion on channel 0 */
while(!(ADC0->SC1[0] & 0x80)) { } /* wait for conversion complete */
result = ADC0->R[0]; /* read conversion result and clear COCO flag PTE20 */	
}

/* Initialize the Map of Game*/
GLCD_clear();


GLCD_setCursor(x,y);
GLCD_putchar(0);

GLCD_setCursor(x2,y2);
GLCD_putchar(5);


draw_middlewall2(10,0);
draw_middlewall(10,2);
draw_middlewall2(10,3);
draw_middlewall(10,5);

draw_solkenar();
draw_solaltkenar();
draw_sagkenar();
draw_sagaltkenar();

draw_middlewall2(66,0);
draw_middlewall(66,2);
draw_middlewall2(66,3);
draw_middlewall(66,5);



/*INITIALIZE POINTS*/

draw_pacman(b1x,b1y,0,0,10);

draw_pacman(b3x,b3y,0,0,10);

draw_pacman(b5x,b5y,0,0,10);

draw_pacman(b7x,b7y,0,0,10);

draw_pacman(b9x,b9y,0,0,10);

draw_pacman(b11x,b11y,0,0,10);
draw_pacman(b12x,b12y,0,0,10);

draw_pacman(b18x,b18y,0,0,10);

draw_pacman(b20x,b20y,0,0,10);

draw_pacman(b23x,b23y,0,0,10);
draw_pacman(b24x,b24y,0,0,10);

draw_pacman(b28x,b28y,0,0,10);
draw_pacman(b29x,b29y,0,0,10);
draw_pacman(b30x,b30y,0,0,10);

draw_pacman(b32x,b32y,0,0,10);

draw_pacman(b34x,b34y,0,0,10);



while(1){

ADC0->SC1[0] = 0; /* start conversion on channel 0 */
while(!(ADC0->SC1[0] & 0x80)) { } /* wait for conversion complete */
result = ADC0->R[0]; /* read conversion result and clear COCO flag PTE20 */	
srand(result);



/* Read Joysticks to move Pacman character

Also control Pacman's interaction with walls & solid blocks


*/

if( result > 4000){
	 
	 
value = 1;

oldvalue = value;	
	
	 if( x >= 76){
	 xold = x;
	 yold = y;
	 x = 0;
   draw_pacman(x,y,xold,yold,0);

 }
	 	 
 else if( y == 2 & x == 30){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }		 
 else if( y == 2 & x == 29){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }	
	 
 else if( y == 2 & x == 28){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }		 
 else if( y == 2 & x == 27){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }		 
	 else if( y == 2 & x == 26){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }		 
		 else if( y == 2 & x == 25){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }
			 else if( y == 2 & x == 24){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }
		 else if( y == 2 & x == 23){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }			 
 		 else if( y == 2 & x == 22){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }			 
 
 
 
 else if( y == 2 & x == 42){
 xold = x;
 yold = y;
 x = 41;
 draw_pacman(x,y,xold,yold,0);



 }	 
  else if( y == 2 & x == 41){
 xold = x;
 yold = y;
 x = 41;
 draw_pacman(x,y,xold,yold,0);



 }	 
	
  else if( y == 4 & x == 42){
 xold = x;
 yold = y;
 x = 41;
 draw_pacman(x,y,xold,yold,0);



 }	 
  else if( y == 4 & x == 41){
 xold = x;
 yold = y;
 x = 41;
 draw_pacman(x,y,xold,yold,0);



 }	 
 
 
 
 
 
  else if( y == 4 & x == 28){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }	
 else if( y == 4 & x == 27){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }		 
	 else if( y == 4 & x == 26){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }		 
		 else if( y == 4 & x == 25){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }
 		 else if( y == 4 & x == 24){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }
		 		 else if( y == 4 & x == 23){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }
 		 else if( y == 4 & x == 22){
 xold = x;
 yold = y;
 x = 22;
 draw_pacman(x,y,xold,yold,0);
 }
 
 
 
	 else if (( x == 0 & y == 3 ) | ( x == 0 & y == 2) | ( x == 0 & y == 0 ) | ( x == 0 & y == 5 )){
		xold = x;
		yold = y;
		x = 0; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 1 & y == 3 )| ( x == 1 & y == 2)){
		xold = x;
		yold = y;
		x = 0; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 2 & y == 3 )| ( x == 2 & y == 2)){
		xold = x;
		yold = y;
		x = 0; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 3 & y == 3 )| ( x == 3 & y == 2)){
		xold = x;
		yold = y;
		x = 0; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 4 & y == 3 )| ( x == 4 & y == 2)){
		xold = x;
		yold = y;
		x = 0; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 5 & y == 3 )| ( x == 5 & y == 2)){
		xold = x;
		yold = y;
		x = 0; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 6 & y == 3 )| ( x == 6 & y == 2)){
		xold = x;
		yold = y;
		x = 0; 
		draw_pacman(x,y,xold,yold,0);
	 }
  
	 else if (( x == 56 & y == 0 ) | ( x == 56 & y == 5)){
		xold = x;
		yold = y;
		x = 56; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 57 & y == 0 )| ( x == 57 & y == 5)){
		xold = x;
		yold = y;
		x = 56; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 58 & y == 0 )| ( x == 58 & y == 5)){
		xold = x;
		yold = y;
		x = 56; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 59 & y == 0 )| ( x == 59 & y == 5)){
		xold = x;
		yold = y;
		x = 56; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 60 & y == 0 )| ( x == 60 & y == 5)){
		xold = x;
		yold = y;
		x = 56; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 61 & y == 0 )| ( x == 61 & y == 5)){
		xold = x;
		yold = y;
		x = 56; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 62 & y == 0 )| ( x == 62 & y == 5)){
		xold = x;
		yold = y;
		x = 56; 
		draw_pacman(x,y,xold,yold,0);
	 }
	 
	 
	 else if (( x == 56 & y == 3 ) | ( x == 56 & y == 2) | ( x == 56 & y == 0 ) | ( x == 56 & y == 5 )){
		xold = x;
		yold = y;
		x = 56; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 57 & y == 3 )| ( x == 57 & y == 2)){
		xold = x;
		yold = y;
		x = 56; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 58 & y == 3 )| ( x == 58 & y == 2)){
		xold = x;
		yold = y;
		x = 56; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 59 & y == 3 )| ( x == 59 & y == 2)){
		xold = x;
		yold = y;
		x = 56; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 60 & y == 3 )| ( x == 60 & y == 2)){
		xold = x;
		yold = y;
		x = 56; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 61 & y == 3 )| ( x == 61 & y == 2)){
		xold = x;
		yold = y;
		x = 56; 
		draw_pacman(x,y,xold,yold,0);
	 }
		 else if (( x == 62 & y == 3 )| ( x == 62 & y == 2)){
		xold = x;
		yold = y;
		x = 56; 
		draw_pacman(x,y,xold,yold,0);
	 }
	
		else
		{
  xold = x;
	yold = y;
	x += 7;
	draw_pacman(x,y,xold,yold,0);
	
	
		}
	

	
}

else if(result < 500){
		
value = 1;
oldvalue = value;
		
  if(x <= 1){
	 xold = x;
	 yold = y;
	 x = 68;
   draw_pacman(x,y,xold,yold,2);
	}

else if ( y == 2 &  x == 42){
	
	
 xold = x;
 yold = y;
 x = 42;
 draw_pacman(x,y,xold,yold,2);



 }		
	
	
	
else if ( y == 2 &  x == 41){
	
	
 xold = x;
 yold = y;
 x = 42;
 draw_pacman(x,y,xold,yold,2);



 }		
	

else if ( y == 2 &  x == 40){
	
	
 xold = x;
 yold = y;
 x = 42;
 draw_pacman(x,y,xold,yold,2);



 }else if ( y == 2 &  x == 39){
	
	
 xold = x;
 yold = y;
 x = 42;
 draw_pacman(x,y,xold,yold,2);



 }	 	else if ( y == 2 &  x == 38){
	
	
 xold = x;
 yold = y;
 x = 42;
 draw_pacman(x,y,xold,yold,2);



 }	 
	
else if ( y == 4 &  x == 42){
	
	
 xold = x;
 yold = y;
 x = 42;
 draw_pacman(x,y,xold,yold,2);



 }		
	
	
	
else if ( y == 4 &  x == 41){
	
	
 xold = x;
 yold = y;
 x = 42;
 draw_pacman(x,y,xold,yold,2);



 }		
	

else if ( y == 4 &  x == 40){
	
	
 xold = x;
 yold = y;
 x = 42;
 draw_pacman(x,y,xold,yold,2);



 }else if ( y == 4 &  x == 39){
	
	
 xold = x;
 yold = y;
 x = 42;
 draw_pacman(x,y,xold,yold,2);



 }	 	else if ( y == 4 &  x == 38){
	
	
 xold = x;
 yold = y;
 x = 42;
 draw_pacman(x,y,xold,yold,2);



 }	 
	
	
	
 else if( y == 2 & x == 54){
 xold = x;
 yold = y;
 x = 54;
 draw_pacman(x,y,xold,yold,2);
 }		 
 else if( y == 2 & x == 55){
 xold = x;
 yold = y;
 x = 54;
 draw_pacman(x,y,xold,yold,2);
 }		 
	 else if( y == 2 & x == 56){
 xold = x;
 yold = y;
 x = 54;
 draw_pacman(x,y,xold,yold,2);
 }		 
		 else if( y == 2 & x == 57){
 xold = x;
 yold = y;
 x = 54;
 draw_pacman(x,y,xold,yold,2);
 }
 
	
 
 else if( y == 4 & x == 54){
 xold = x;
 yold = y;
 x = 54;
 draw_pacman(x,y,xold,yold,2);
 }		 
 else if( y == 4 & x == 55){
 xold = x;
 yold = y;
 x = 54;
 draw_pacman(x,y,xold,yold,2);
 }		 
	 else if( y == 4 & x == 56){
 xold = x;
 yold = y;
 x = 54;
 draw_pacman(x,y,xold,yold,2);
 }		 
		 else if( y == 4 & x == 57){
 xold = x;
 yold = y;
 x = 54;
 draw_pacman(x,y,xold,yold,2);
 }
 
    else if(( y == 2 & x == 82 ) | ( y == 2 & x == 81) | ( y == 2 & x == 80 ) | ( y == 2 & x == 79) | ( y == 2 & x == 78 ) | ( y == 2 & x == 77)){
			
		xold = x;
		yold = y;
		x = 77; 
		draw_pacman(x,y,xold,yold,2);
		
	}
		    else if(( y == 3 & x == 82 ) | ( y == 3 & x == 81) | ( y == 3 & x == 80 ) | ( y == 3 & x == 79) | ( y == 3 & x == 78 ) | ( y == 3 & x == 77)){
			
		xold = x;
		yold = y;
		x = 77; 
		draw_pacman(x,y,xold,yold,2);
		
	}
				    else if(( y == 5 & x == 82 ) | ( y == 5 & x == 81) | ( y == 5 & x == 80 ) | ( y == 5 & x == 79) | ( y == 5 & x == 78 ) | ( y == 5 & x == 77)){
			
		xold = x;
		yold = y;
		x = 77; 
		draw_pacman(x,y,xold,yold,2);
		
	}
		else if(( y == 0 & x == 82 ) | ( y == 0 & x == 81) | ( y == 0 & x == 80 ) | ( y == 0 & x == 79) | ( y == 0 & x == 78 ) | ( y == 0 & x == 77)){
			
		xold = x;
		yold = y;
		x = 77; 
		draw_pacman(x,y,xold,yold,2);
		
	}
		else if(( y == 0 & x == 82 ) | ( y == 0 & x == 81) | ( y == 0 & x == 80 ) | ( y == 0 & x == 79) | ( y == 0 & x == 78 ) | ( y == 0 & x == 77)){
			
		xold = x;
		yold = y;
		x = 77; 
		draw_pacman(x,y,xold,yold,2);
		
	}
	
	
	
	 else if (( x == 20 & y == 3 ) | ( x == 20 & y == 2) | ( x == 20 & y == 0 ) | ( x == 20 & y == 5 ) | ( x == 21 & y == 0 ) | ( x == 21 & y == 5 ) | ( x == 22 & y == 0 ) | ( x == 22 & y == 5 ) | ( x == 23 & y == 0 ) | ( x == 23 & y == 5 ) | ( x == 24 & y == 0 ) | ( x == 24 & y == 5 ) ){
		xold = x;
		yold = y;
		x = 20; 
		draw_pacman(x,y,xold,yold,2);
	 }  
	 
		 else if (( x == 20 & y == 3 )| ( x == 20 & y == 2)){
		xold = x;
		yold = y;
		x = 20; 
		draw_pacman(x,y,xold,yold,2);
	 }
		 else if (( x == 21 & y == 3 )| ( x == 21 & y == 2)){
		xold = x;
		yold = y;
		x = 20; 
		draw_pacman(x,y,xold,yold,2);
	 }
		 else if (( x == 22 & y == 3 )| ( x == 22 & y == 2)){
		xold = x;
		yold = y;
		x = 20; 
		draw_pacman(x,y,xold,yold,2);
	 }
		 else if (( x == 23 & y == 3 )| ( x == 23 & y == 2)){
		xold = x;
		yold = y;
		x = 20; 
		draw_pacman(x,y,xold,yold,2);
	 }
		 else if (( x == 24 & y == 3 )| ( x == 24 & y == 2)){
		xold = x;
		yold = y;
		x = 20; 
		draw_pacman(x,y,xold,yold,2);
	 }
		 else if (( x == 25 & y == 3 )| ( x == 25 & y == 2)){
		xold = x;
		yold = y;
		x = 20; 
		draw_pacman(x,y,xold,yold,2);
	 }
  

 
else
{	
	
	oldvalue = value;
	xold = x;
	yold = y;
	x -= 7;
  draw_pacman(x,y,xold,yold,2);

	}

}


	

	
	
ADC0->SC1[0] = 4; /* start conversion on channel 0 */
while(!(ADC0->SC1[0] & 0x80)) { } /* wait for conversion complete */
result2 = ADC0->R[0]; /* read conversion result and clear COCO flag PTE20 */	



	if( result2 > 4000){
		

		
		
	 if( y == 6){
	 xold = x;
	 yold = y;
	 y = 0;
   draw_pacman(x,y,xold,yold,4);
		 

 }
	 
 else if( y == 1 & x == 7){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 1 & x == 8){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
 
 else if( y == 1 & x == 9){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		
 else if( y == 1 & x == 10){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 1 & x == 11){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
 
 else if( y == 1 & x == 12){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 1 & x == 13){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
	 else if( y == 1 & x == 14){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
		 else if( y == 1 & x == 15){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }
 else if( y == 1 & x == 16){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 1 & x == 17){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 	 
	 else if( y == 1 & x == 18){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 

 else if( ( y == 1 & x == 28) |( y == 1 & x == 29) | ( y == 1 & x == 30) | ( y == 1 & x == 31) | ( y == 1 & x == 32) | ( y == 1 & x == 33) | ( y == 1 & x == 34) | ( y == 1 & x == 35) | ( y == 1 & x == 36) | ( y == 1 & x == 37)){
	
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 
 
}
 
 else if( y == 1 & x == 58){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 1 & x == 59){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
	 else if( y == 1 & x == 60){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
		 else if( y == 1 & x == 61){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }
 else if( y == 1 & x == 62){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 1 & x == 63){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 	 
	 else if( y == 1 & x == 64){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 1 & x == 65){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
 
 else if( y == 1 & x == 66){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 1 & x == 67){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
	 else if( y == 1 & x == 68){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
		 else if( y == 1 & x == 69){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }
 else if( y == 1 & x == 70){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);
 }		 
 
 else if(  ( y == 1 & x == 44) | ( y == 1 & x == 45) | ( y == 1 & x == 46) | ( y == 1 & x == 47) | ( y == 1 & x == 48) | ( y == 1 & x == 49) | ( y == 1 & x == 50) | ( y == 1 & x == 51) | ( y == 1 & x == 52)){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,4);	
}
 
 else if(( y == 3 & x == 28) |( y == 3 & x == 29) | ( y == 3 & x == 30) | ( y == 3 & x == 31) | ( y == 3 & x == 32) | ( y == 3 & x == 33) | ( y == 3 & x == 34) | ( y == 3 & x == 35) | ( y == 3 & x == 36) | ( y == 3 & x == 37)){
	
 xold = x;
 yold = y;
 y = 3;
 draw_pacman(x,y,xold,yold,4);	
}

else if(  ( y == 3 & x == 44) | ( y == 3 & x == 45) | ( y == 3 & x == 46) | ( y == 3 & x == 47) | ( y == 3 & x == 48) | ( y == 3 & x == 49) | ( y == 3 & x == 50) | ( y == 3 & x == 51) | ( y == 3 & x == 52)){
	
 xold = x;
 yold = y;
 y = 3;
 draw_pacman(x,y,xold,yold,4);	
}
 
 
  else if( y == 4 & x == 7){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 4 & x == 8){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
 
 else if( y == 4 & x == 9){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		
 else if( y == 4 & x == 10){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 4 & x == 11){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
 
 else if( y == 4 & x == 12){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 4 & x == 13){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
	 else if( y == 4 & x == 14){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
		 else if( y == 4 & x == 15){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }
 else if( y == 4 & x == 16){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 4 & x == 17){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 	 
	 else if( y == 4 & x == 18){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 

  
 else if( y == 4 & x == 58){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 4 & x == 59){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
	 else if( y == 4 & x == 60){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
		 else if( y == 4 & x == 61){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }
 else if( y == 4 & x == 62){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 4 & x == 63){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 	 
	 else if( y == 4 & x == 64){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 4 & x == 65){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
 
 else if( y == 4 & x == 66){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
 else if( y == 4 & x == 67){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
	 else if( y == 4 & x == 68){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
		 else if( y == 4 & x == 69){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }
 else if( y == 4 & x == 70){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,4);
 }		 
 
 
 
 
 
 
 
 
else
{	
		
		
	xold = x;
	yold = y;
	y += 1;
  draw_pacman(x,y,xold,yold,4);




}
}
	else if(result2 < 500){
		
		
		
		
		
			if(y == 0){
		xold = x;
	  yold = y;
    y = 6;
  draw_pacman(x,y,xold,yold,3);
    }
			
		
else if( y == 1 & x == 7){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 1 & x == 8){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
 
 else if( y == 1 & x == 9){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 			
		
else if( ( y == 5 & x == 28) |( y == 5 & x == 29) | ( y == 5 & x == 30) | ( y == 5 & x == 31) | ( y == 5 & x == 32) | ( y == 5 & x == 33) | ( y == 5 & x == 34) | ( y == 5 & x == 35) | ( y == 5 & x == 36) | ( y == 5 & x == 37)){
	
 xold = x;
 yold = y;
 y = 5;
 draw_pacman(x,y,xold,yold,3);	
}

else if(  ( y == 5 & x == 44) | ( y == 5 & x == 45) | ( y == 5 & x == 46) | ( y == 5 & x == 47) | ( y == 5 & x == 48) | ( y == 5 & x == 49) | ( y == 5 & x == 50) | ( y == 5 & x == 51) | ( y == 5 & x == 52)){
	
 xold = x;
 yold = y;
 y = 5;
 draw_pacman(x,y,xold,yold,3);	
}

else if(  ( y == 3 & x == 44) | ( y == 3 & x == 45) | ( y == 3 & x == 46) | ( y == 3 & x == 47) | ( y == 3 & x == 48) | ( y == 3 & x == 49) | ( y == 3 & x == 50) | ( y == 3 & x == 51) | ( y == 3 & x == 52)){
	
 xold = x;
 yold = y;
 y = 3;
 draw_pacman(x,y,xold,yold,3);	
}

else if( ( y == 3 & x == 28) |( y == 3 & x == 29) | ( y == 3 & x == 30) | ( y == 3 & x == 31) | ( y == 3 & x == 32) | ( y == 3 & x == 33) | ( y == 3 & x == 34) | ( y == 3 & x == 35) | ( y == 3 & x == 36) | ( y == 3 & x == 37)){
	
 xold = x;
 yold = y;
 y = 3;
 draw_pacman(x,y,xold,yold,3);	
}
		
 else if( y == 1 & x == 10){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 1 & x == 11){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
 
 else if( y == 1 & x == 12){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 1 & x == 13){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
	 else if( y == 1 & x == 14){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
		 else if( y == 1 & x == 15){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }
 else if( y == 1 & x == 16){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 1 & x == 17){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 	 
	 else if( y == 1 & x == 18){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 

 
  else if( y == 4 & x == 7){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 4 & x == 8){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
 
 else if( y == 4 & x == 9){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		
 else if( y == 4 & x == 10){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 4 & x == 11){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
 
 else if( y == 4 & x == 12){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 4 & x == 13){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
	 else if( y == 4 & x == 14){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
		 else if( y == 4 & x == 15){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }
 else if( y == 4 & x == 16){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 4 & x == 17){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 	 
	 else if( y == 4 & x == 18){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		  
 
 else if( y == 4 & x == 58){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 4 & x == 59){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
	 else if( y == 4 & x == 60){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
		 else if( y == 4 & x == 61){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }
 else if( y == 4 & x == 62){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 4 & x == 63){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 	 
	 else if( y == 4 & x == 64){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 4 & x == 65){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
 
 else if( y == 4 & x == 66){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 4 & x == 67){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
	 else if( y == 4 & x == 68){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
		 else if( y == 4 & x == 69){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }
 else if( y == 4 & x == 70){
 xold = x;
 yold = y;
 y = 4;
 draw_pacman(x,y,xold,yold,3);
 }		 
 
 
 else if( y == 1 & x == 58){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 1 & x == 59){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
	 else if( y == 1 & x == 60){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
		 else if( y == 1 & x == 61){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }
 else if( y == 1 & x == 62){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 1 & x == 63){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 	 
	 else if( y == 1 & x == 64){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 1 & x == 65){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
 
 else if( y == 1 & x == 66){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
 else if( y == 1 & x == 67){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
	 else if( y == 1 & x == 68){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
		 else if( y == 1 & x == 69){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }
 else if( y == 1 & x == 70){
 xold = x;
 yold = y;
 y = 1;
 draw_pacman(x,y,xold,yold,3);
 }		 
 
		else
			
		{
		
	xold = x;
	yold = y;
	y -= 1;
  draw_pacman(x,y,xold,yold,3);

		}

	}




/* SECOND PLAYER BEGINS */


ADC0->SC1[0] = 7; /* start conversion on channel 0 */
while(!(ADC0->SC1[0] & 0x80)) { } /* wait for conversion complete */
second1 = ADC0->R[0]; /* read conversion result and clear COCO flag PTE20 */	


/* Control movements of Second Player 


*/

	if( second1 > 4000){
		
		
	 if( x2 >= 76){
	 xold2 = x2;
	 yold2 = y2;
	 x2 = 0;
   draw_pacman(x2,y2,xold2,yold2,5);
	 

 }	
		 
 else if( y2 == 2 & x2 == 28){
 xold2 = x2;
 yold2 = y2;
 x2 = 25;
 draw_pacman(x2,y2,xold2,yold2,5);
 }		 
 else if( y2 == 2 & x2 == 27){
 xold2 = x2;
 yold2 = y2;
 x2 = 25;
 draw_pacman(x2,y2,xold2,yold2,5);
 }		 
	 else if( y2 == 2 & x2 == 26){
 xold2 = x2;
 yold2 = y2;
 x2 = 25;
 draw_pacman(x2,y2,xold2,yold2,5);
 }		 
		 else if( y2 == 2 & x2 == 25){
 xold2 = x2;
 yold2 = y2;
 x2 = 25;
 draw_pacman(x2,y2,xold2,yold,5);
 }
 
 	 
  else if( y2 == 4 & x2 == 28){
 xold2 = x2;
 yold2 = y;
 x2 = 24;
 draw_pacman(x2,y2,xold2,yold2,5);
 }	
 else if( y2 == 4 & x2 == 27){
 xold2 = x2;
 yold2 = y2;
 x2 = 24;
 draw_pacman(x2,y2,xold2,yold2,5);
 }		 
	 else if( y2 == 4 & x2 == 26){
 xold2 = x2;
 yold2 = y2;
 x2 = 24;
 draw_pacman(x2,y2,xold2,yold2,5);
 }		 
		 else if( y2 == 4 & x2 == 25){
 xold2 = x2;
 yold2 = y2;
 x2 = 24;
 draw_pacman(x2,y2,xold2,yold2,5);
 }
 		 else if( y2 == 4 & x2 == 24){
 xold2 = x2;
 yold2 = y2;
 x2 = 24;
 draw_pacman(x2,y2,xold2,yold2,5);
 }
 
  
 else if( y2 == 2 & x2 == 42){
 xold2 = x2;
 yold2 = y2;
 x2 = 41;
 draw_pacman(x2,y2,xold2,yold2,0);



 }	 
  else if( y2 == 2 & x2 == 41){
 xold2 = x2;
 yold2 = y2;
 x2 = 41;
 draw_pacman(x2,y2,xold2,yold2,0);



 }	 
	
  else if( y2 == 4 & x2 == 42){
 xold2 = x2;
 yold2 = y2;
 x2 = 41;
 draw_pacman(x2,y2,xold2,yold2,0);



 }	 
  else if( y2 == 4 & x2 == 41){
 xold2 = x2;
 yold2 = y2;
 x2 = 41;
 draw_pacman(x2,y2,xold2,yold2,0);



 }	 
 
 
	 else if (( x2 == 0 & y2 == 3 ) | ( x2 == 0 & y2 == 2) | ( x2 == 0 & y2 == 0 ) | ( x2 == 0 & y2 == 5 )){
		xold2 = x2;
		yold2 = y2;
		x2 = 0; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 1 & y2 == 3 )| ( x2 == 1 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 0; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 2 & y2 == 3 )| ( x2 == 2 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 0; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 3 & y2 == 3 )| ( x2 == 3 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 0; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 4 & y2 == 3 )| ( x2 == 4 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 0; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 5 & y2 == 3 )| ( x2 == 5 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 0; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 6 & y2 == 3 )| ( x2 == 6 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 0; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
  
	 else if (( x2 == 56 & y2 == 0 ) | ( x2 == 56 & y2 == 5)){
		xold2 = x2;
		yold2 = y2;
		x2 = 56; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 57 & y2 == 0 )| ( x2 == 57 & y2 == 5)){
		xold2 = x2;
		yold2 = y2;
		x2 = 56; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 58 & y2 == 0 )| ( x2 == 58 & y2 == 5)){
		xold2 = x2;
		yold2 = y2;
		x2 = 56; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 59 & y2 == 0 )| ( x2 == 59 & y2 == 5)){
		xold2 = x2;
		yold2 = y2;
		x2 = 56; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 60 & y2 == 0 )| ( x2 == 60 & y2 == 5)){
		xold2 = x2;
		yold2 = y2;
		x2 = 56; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 61 & y2 == 0 )| ( x2 == 61 & y2 == 5)){
		xold2 = x2;
		yold2 = y2;
		x2 = 56; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 62 & y2 == 0 )| ( x2 == 62 & y2 == 5)){
		xold2 = x2;
		yold2 = y2;
		x2 = 56; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
	 
	 
	 else if (( x2 == 56 & y2 == 3 ) | ( x2 == 56 & y2 == 2) | ( x2 == 56 & y2 == 0 ) | ( x2 == 56 & y2 == 5 )){
		xold2 = x2;
		yold2 = y2;
		x2 = 56; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 57 & y2 == 3 )| ( x2 == 57 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 56; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 58 & y2 == 3 )| ( x2 == 58 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 56; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 59 & y2 == 3 )| ( x2 == 59 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 56; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 60 & y2 == 3 )| ( x2 == 60 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 56; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 61 & y2 == 3 )| ( x2 == 61 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 56; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
		 else if (( x2 == 62 & y2 == 3 )| ( x2 == 62 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 56; 
		draw_pacman(x2,y2,xold2,yold2,5);
	 }
	

 
 
 
 
 
	else
	{
  xold2 = x2;
	yold2 = y2;
	x2 += 7;
	draw_pacman(x2,y2,xold2,yold2,5);
  
		
			if( (( xold2 == b1x) & (yold2 == b1y)) | ( (xold2 == b1x-1) & ( yold2 == b1y )) | ( (xold2 == b1x-2) & ( yold2 == b1y )) | ( (xold2 == b1x-3) & ( yold2 == b1y )) | ( (xold2 == b1x-4) & ( yold2 == b1y )) | ( (xold2 == b1x-5) & ( yold2 == b1y )) | ( (xold2 == b1x-6) & ( yold2 == b1y )) | ((xold2+1 == b1x ) & (yold2 == b1y)) | ((xold2+2 == b1x ) & (yold2 == b1y)) | ((xold2+3 == b1x ) & (yold2 == b1y)) | ((xold2+4 == b1x ) & (yold2 == b1y)) | ((xold2+5 == b1x ) & (yold2 == b1y))) {



draw_pacman(b1x,b1y,0,0,10);
	
}
//		else if( (( xold2 == b2x) & (yold2 == b2y)) | ( (xold2 == b2x-1) & ( yold2 == b2y )) | ( (xold2 == b2x-2) & ( yold2 == b2y )) | ( (xold2 == b2x-3) & ( yold2 == b2y )) | ( (xold2 == b2x-4) & ( yold2 == b2y )) | ( (xold2 == b2x-5) & ( yold2 == b2y )) | ( (xold2 == b2x-6) & ( yold2 == b2y )) | ((xold2+1 == b2x ) & (yold2 == b2y)) | ((xold2+2 == b2x ) & (yold2 == b2y)) | ((xold2+3 == b2x ) & (yold2 == b2y)) | ((xold2+4 == b2x ) & (yold2 == b2y)) | ((xold2+5 == b2x ) & (yold2 == b2y))) {

//	draw_pacman(b2x,b2y,0,0,10);
//}
		
else if( (( xold2 == b3x) & (yold2 == b3y)) | ( (xold2 == b3x-1) & ( yold2 == b3y )) | ( (xold2 == b3x-2) & ( yold2 == b3y )) | ( (xold2 == b3x-3) & ( yold2 == b3y )) | ( (xold2 == b3x-4) & ( yold2 == b3y )) | ( (xold2 == b3x-5) & ( yold2 == b3y )) | ( (xold2 == b3x-6) & ( yold2 == b3y )) | ((xold2+1 == b3x ) & (yold2 == b3y)) | ((xold2+2 == b3x ) & (yold2 == b3y)) | ((xold2+3 == b3x ) & (yold2 == b3y)) | ((xold2+4 == b3x ) & (yold2 == b3y)) | ((xold2+5 == b3x ) & (yold2 == b3y))) {

draw_pacman(b3x,b3y,0,0,10);	
}
//else if( (( xold2 == b4x) & (yold2 == b4y)) | ( (xold2 == b4x-1) & ( yold2 == b4y )) | ( (xold2 == b4x-2) & ( yold2 == b4y )) | ( (xold2 == b4x-3) & ( yold2 == b4y )) | ( (xold2 == b4x-4) & ( yold2 == b4y )) | ( (xold2 == b4x-5) & ( yold2 == b4y )) | ( (xold2 == b4x-6) & ( yold2 == b4y )) | ((xold2+1 == b4x ) & (yold2 == b4y)) | ((xold2+2 == b4x ) & (yold2 == b4y)) | ((xold2+3 == b4x ) & (yold2 == b4y)) | ((xold2+4 == b4x ) & (yold2 == b4y)) | ((xold2+5 == b4x ) & (yold2 == b4y))) {

//draw_pacman(b4x,b4y,0,0,10);
//	
//}
else if( (( xold2 == b5x) & (yold2 == b5y)) | ( (xold2 == b5x-1) & ( yold2 == b5y )) | ( (xold2 == b5x-2) & ( yold2 == b5y )) | ( (xold2 == b5x-3) & ( yold2 == b5y )) | ( (xold2 == b5x-4) & ( yold2 == b5y )) | ( (xold2 == b5x-5) & ( yold2 == b5y )) | ( (xold2 == b5x-6) & ( yold2 == b5y )) | ((xold2+1 == b5x ) & (yold2 == b5y)) | ((xold2+2 == b5x ) & (yold2 == b5y)) | ((xold2+3 == b5x ) & (yold2 == b5y)) | ((xold2+4 == b5x ) & (yold2 == b5y)) | ((xold2+5 == b5x ) & (yold2 == b5y))) {

draw_pacman(b5x,b5y,0,0,10);

}
//else if( (( xold2 == b6x) & (yold2 == b6y)) | ( (xold2 == b6x-1) & ( yold2 == b6y )) | ( (xold2 == b6x-2) & ( yold2 == b6y )) | ( (xold2 == b6x-3) & ( yold2 == b6y )) | ( (xold2 == b6x-4) & ( yold2 == b6y )) | ( (xold2 == b6x-5) & ( yold2 == b6y )) | ( (xold2 == b6x-6) & ( yold2 == b6y )) | ((xold2+1 == b6x ) & (yold2 == b6y)) | ((xold2+2 == b6x ) & (yold2 == b6y)) | ((xold2+3 == b6x ) & (yold2 == b6y)) | ((xold2+4 == b6x ) & (yold2 == b6y)) | ((xold2+5 == b6x ) & (yold2 == b6y))) {

//draw_pacman(b6x,b6y,0,0,10);
//	
//}
else if( (( xold2 == b7x) & (yold2 == b7y)) | ( (xold2 == b7x-1) & ( yold2 == b7y )) | ( (xold2 == b7x-2) & ( yold2 == b7y )) | ( (xold2 == b7x-3) & ( yold2 == b7y )) | ( (xold2 == b7x-4) & ( yold2 == b7y )) | ( (xold2 == b7x-5) & ( yold2 == b7y )) | ( (xold2 == b7x-6) & ( yold2 == b7y )) | ((xold2+1 == b7x ) & (yold2 == b7y)) | ((xold2+2 == b7x ) & (yold2 == b7y)) | ((xold2+3 == b7x ) & (yold2 == b7y)) | ((xold2+4 == b7x ) & (yold2 == b7y)) | ((xold2+5 == b7x ) & (yold2 == b7y))) {

draw_pacman(b7x,b7y,0,0,10);	
}
//else if( (( xold2 == b8x) & (yold2 == b8y)) | ( (xold2 == b8x-1) & ( yold2 == b8y )) | ( (xold2 == b8x-2) & ( yold2 == b8y )) | ( (xold2 == b8x-3) & ( yold2 == b8y )) | ( (xold2 == b8x-4) & ( yold2 == b8y )) | ( (xold2 == b8x-5) & ( yold2 == b8y )) | ( (xold2 == b8x-6) & ( yold2 == b8y )) | ((xold2+1 == b8x ) & (yold2 == b8y)) | ((xold2+2 == b8x ) & (yold2 == b8y)) | ((xold2+3 == b8x ) & (yold2 == b8y)) | ((xold2+4 == b8x ) & (yold2 == b8y)) | ((xold2+5 == b8x ) & (yold2 == b8y))) {
//draw_pacman(b8x,b8y,0,0,10);

//}
else if( (( xold2 == b9x) & (yold2 == b9y)) | ( (xold2 == b9x-1) & ( yold2 == b9y )) | ( (xold2 == b9x-2) & ( yold2 == b9y )) | ( (xold2 == b9x-3) & ( yold2 == b9y )) | ( (xold2 == b9x-4) & ( yold2 == b9y )) | ( (xold2 == b9x-5) & ( yold2 == b9y )) | ( (xold2 == b9x-6) & ( yold2 == b9y )) | ((xold2+1 == b9x ) & (yold2 == b9y)) | ((xold2+2 == b9x ) & (yold2 == b9y)) | ((xold2+3 == b9x ) & (yold2 == b9y)) | ((xold2+4 == b9x ) & (yold2 == b9y)) | ((xold2+5 == b9x ) & (yold2 == b9y))) {

draw_pacman(b9x,b9y,0,0,10);
	
}
//else if( (( xold2 == b10x) & (yold2 == b10y)) | ( (xold2 == b10x-1) & ( yold2 == b10y )) | ( (xold2 == b10x-2) & ( yold2 == b10y )) | ( (xold2 == b10x-3) & ( yold2 == b10y )) | ( (xold2 == b10x-4) & ( yold2 == b10y )) | ( (xold2 == b10x-5) & ( yold2 == b10y )) | ( (xold2 == b10x-6) & ( yold2 == b10y )) | ((xold2+1 == b10x ) & (yold2 == b10y)) | ((xold2+2 == b10x ) & (yold2 == b10y)) | ((xold2+3 == b10x ) & (yold2 == b10y)) | ((xold2+4 == b10x ) & (yold2 == b10y)) | ((xold2+5 == b10x ) & (yold2 == b10y))) {
//draw_pacman(b10x,b10y,0,0,10);

//}
else if( (( xold2 == b11x) & (yold2 == b11y)) | ( (xold2 == b11x-1) & ( yold2 == b11y )) | ( (xold2 == b11x-2) & ( yold2 == b11y )) | ( (xold2 == b11x-3) & ( yold2 == b11y )) | ( (xold2 == b11x-4) & ( yold2 == b11y )) | ( (xold2 == b11x-5) & ( yold2 == b11y )) | ( (xold2 == b11x-6) & ( yold2 == b11y )) | ((xold2+1 == b11x ) & (yold2 == b11y)) | ((xold2+2 == b11x ) & (yold2 == b11y)) | ((xold2+3 == b11x ) & (yold2 == b11y)) | ((xold2+4 == b11x ) & (yold2 == b11y)) | ((xold2+5 == b11x ) & (yold2 == b11y))) {

draw_pacman(b11x,b11y,0,0,10);
	
}
else if( (( xold2 == b12x) & (yold2 == b12y)) | ( (xold2 == b12x-1) & ( yold2 == b12y )) | ( (xold2 == b12x-2) & ( yold2 == b12y )) | ( (xold2 == b12x-3) & ( yold2 == b12y )) | ( (xold2 == b12x-4) & ( yold2 == b12y )) | ( (xold2 == b12x-5) & ( yold2 == b12y )) | ( (xold2 == b12x-6) & ( yold2 == b12y )) | ((xold2+1 == b12x ) & (yold2 == b12y)) | ((xold2+2 == b12x ) & (yold2 == b12y)) | ((xold2+3 == b12x ) & (yold2 == b12y)) | ((xold2+4 == b12x ) & (yold2 == b12y)) | ((xold2+5 == b12x ) & (yold2 == b12y))) {

draw_pacman(b12x,b12y,0,0,10);
	
}

else if( (( xold2 == b18x) & (yold2 == b18y)) | ( (xold2 == b18x-1) & ( yold2 == b18y )) | ( (xold2 == b18x-2) & ( yold2 == b18y )) | ( (xold2 == b18x-3) & ( yold2 == b18y )) | ( (xold2 == b18x-4) & ( yold2 == b18y )) | ( (xold2 == b18x-5) & ( yold2 == b18y )) | ( (xold2 == b18x-6) & ( yold2 == b18y )) | ((xold2+1 == b18x ) & (yold2 == b18y)) | ((xold2+2 == b18x ) & (yold2 == b18y)) | ((xold2+3 == b18x ) & (yold2 == b18y)) | ((xold2+4 == b18x ) & (yold2 == b18y)) | ((xold2+5 == b18x ) & (yold2 == b18y))) {

draw_pacman(b18x,b18y,0,0,10);
}

else if( (( xold2 == b20x) & (yold2 == b20y)) | ( (xold2 == b20x-1) & ( yold2 == b20y )) | ( (xold2 == b20x-2) & ( yold2 == b20y )) | ( (xold2 == b20x-3) & ( yold2 == b20y )) | ( (xold2 == b20x-4) & ( yold2 == b20y )) | ( (xold2 == b20x-5) & ( yold2 == b20y )) | ( (xold2 == b20x-6) & ( yold2 == b20y )) | ((xold2+1 == b20x ) & (yold2 == b20y)) | ((xold2+2 == b20x ) & (yold2 == b20y)) | ((xold2+3 == b20x ) & (yold2 == b20y)) | ((xold2+4 == b20x ) & (yold2 == b20y)) | ((xold2+5 == b20x ) & (yold2 == b20y))) {

draw_pacman(b20x,b20y,0,0,10);
	
}

else if( (( xold2 == b23x) & (yold2 == b23y)) | ( (xold2 == b23x-1) & ( yold2 == b23y )) | ( (xold2 == b23x-2) & ( yold2 == b23y )) | ( (xold2 == b23x-3) & ( yold2 == b23y )) | ( (xold2 == b23x-4) & ( yold2 == b23y )) | ( (xold2 == b23x-5) & ( yold2 == b23y )) | ( (xold2 == b23x-6) & ( yold2 == b23y )) | ((xold2+1 == b23x ) & (yold2 == b23y)) | ((xold2+2 == b23x ) & (yold2 == b23y)) | ((xold2+3 == b23x ) & (yold2 == b23y)) | ((xold2+4 == b23x ) & (yold2 == b23y)) | ((xold2+5 == b23x ) & (yold2 == b23y))) {

draw_pacman(b23x,b23y,0,0,10);
}
else if( (( xold2 == b24x) & (yold2 == b24y)) | ( (xold2 == b24x-1) & ( yold2 == b24y )) | ( (xold2 == b24x-2) & ( yold2 == b24y )) | ( (xold2 == b24x-3) & ( yold2 == b24y )) | ( (xold2 == b24x-4) & ( yold2 == b24y )) | ( (xold2 == b24x-5) & ( yold2 == b24y )) | ( (xold2 == b24x-6) & ( yold2 == b24y )) | ((xold2+1 == b24x ) & (yold2 == b24y)) | ((xold2+2 == b24x ) & (yold2 == b24y)) | ((xold2+3 == b24x ) & (yold2 == b24y)) | ((xold2+4 == b24x ) & (yold2 == b24y)) | ((xold2+5 == b24x ) & (yold2 == b24y))) {

	draw_pacman(b24x,b24y,0,0,10);
}

else if( (( xold2 == b28x) & (yold2 == b28y)) | ( (xold2 == b28x-1) & ( yold2 == b28y )) | ( (xold2 == b28x-2) & ( yold2 == b28y )) | ( (xold2 == b28x-3) & ( yold2 == b28y )) | ( (xold2 == b28x-4) & ( yold2 == b28y )) | ( (xold2 == b28x-5) & ( yold2 == b28y )) | ( (xold2 == b28x-6) & ( yold2 == b28y )) | ((xold2+1 == b28x ) & (yold2 == b28y)) | ((xold2+2 == b28x ) & (yold2 == b28y)) | ((xold2+3 == b28x ) & (yold2 == b28y)) | ((xold2+4 == b28x ) & (yold2 == b28y)) | ((xold2+5 == b28x ) & (yold2 == b28y))) {

	draw_pacman(b28x,b28y,0,0,10);
}
else if( (( xold2 == b29x) & (yold2 == b29y)) | ( (xold2 == b29x-1) & ( yold2 == b29y )) | ( (xold2 == b29x-2) & ( yold2 == b29y )) | ( (xold2 == b29x-3) & ( yold2 == b29y )) | ( (xold2 == b29x-4) & ( yold2 == b29y )) | ( (xold2 == b29x-5) & ( yold2 == b29y )) | ( (xold2 == b29x-6) & ( yold2 == b29y )) | ((xold2+1 == b29x ) & (yold2 == b29y)) | ((xold2+2 == b29x ) & (yold2 == b29y)) | ((xold2+3 == b29x ) & (yold2 == b29y)) | ((xold2+4 == b29x ) & (yold2 == b29y)) | ((xold2+5 == b29x ) & (yold2 == b29y))) {

draw_pacman(b29x,b29y,0,0,10);
}
else if( (( xold2 == b30x) & (yold2 == b30y)) | ( (xold2 == b30x-1) & ( yold2 == b30y )) | ( (xold2 == b30x-2) & ( yold2 == b30y )) | ( (xold2 == b30x-3) & ( yold2 == b30y )) | ( (xold2 == b30x-4) & ( yold2 == b30y )) | ( (xold2 == b30x-5) & ( yold2 == b30y )) | ( (xold2 == b30x-6) & ( yold2 == b30y )) | ((xold2+1 == b30x ) & (yold2 == b30y)) | ((xold2+2 == b30x ) & (yold2 == b30y)) | ((xold2+3 == b30x ) & (yold2 == b30y)) | ((xold2+4 == b30x ) & (yold2 == b30y)) | ((xold2+5 == b30x ) & (yold2 == b30y))) {
	draw_pacman(b30x,b30y,0,0,10);
}

else if( (( xold2 == b32x) & (yold2 == b32y)) | ( (xold2 == b32x-1) & ( yold2 == b32y )) | ( (xold2 == b32x-2) & ( yold2 == b32y )) | ( (xold2 == b32x-3) & ( yold2 == b32y )) | ( (xold2 == b32x-4) & ( yold2 == b32y )) | ( (xold2 == b32x-5) & ( yold2 == b32y )) | ( (xold2 == b32x-6) & ( yold2 == b32y )) | ((xold2+1 == b32x ) & (yold2 == b32y)) | ((xold2+2 == b32x ) & (yold2 == b32y)) | ((xold2+3 == b32x ) & (yold2 == b32y)) | ((xold2+4 == b32x ) & (yold2 == b32y)) | ((xold2+5 == b32x ) & (yold2 == b32y))) {
draw_pacman(b32x,b32y,0,0,10);

}

else if( (( xold2 == b34x) & (yold2 == b34y)) | ( (xold2 == b34x-1) & ( yold2 == b34y )) | ( (xold2 == b34x-2) & ( yold2 == b34y )) | ( (xold2 == b34x-3) & ( yold2 == b34y )) | ( (xold2 == b34x-4) & ( yold2 == b34y )) | ( (xold2 == b34x-5) & ( yold2 == b34y )) | ( (xold2 == b34x-6) & ( yold2 == b34y )) | ((xold2+1 == b34x ) & (yold2 == b34y)) | ((xold2+2 == b34x ) & (yold2 == b34y)) | ((xold2+3 == b34x ) & (yold2 == b34y)) | ((xold2+4 == b34x ) & (yold2 == b34y)) | ((xold2+5 == b34x ) & (yold2 == b34y))) {
	draw_pacman(b34x,b34y,0,0,10);
}
				
	}

	
}

/* Control game duration*/
	else if(second1 < 500){
		
		

	
  if(x2 <= 1){
	 xold2 = x2;
	 yold2 = y2;
	 x2 = 68;
   draw_pacman(x2,y2,xold2,yold2,6);
		
				if( (( xold2 == b1x) & (yold2 == b1y)) | ( (xold2 == b1x-1) & ( yold2 == b1y )) | ( (xold2 == b1x-2) & ( yold2 == b1y )) | ( (xold2 == b1x-3) & ( yold2 == b1y )) | ( (xold2 == b1x-4) & ( yold2 == b1y )) | ( (xold2 == b1x-5) & ( yold2 == b1y )) | ( (xold2 == b1x-6) & ( yold2 == b1y )) | ((xold2+1 == b1x ) & (yold2 == b1y)) | ((xold2+2 == b1x ) & (yold2 == b1y)) | ((xold2+3 == b1x ) & (yold2 == b1y)) | ((xold2+4 == b1x ) & (yold2 == b1y)) | ((xold2+5 == b1x ) & (yold2 == b1y))) {



draw_pacman(b1x,b1y,0,0,10);
	
}
		
	}
	
	
else if ( y2 == 2 &  x2 == 42){
	
	
 xold2 = x2;
 yold = y2;
 x2 = 42;
 draw_pacman(x2,y2,xold2,yold,6);

 }		
	
	
else if ( y2 == 2 &  x2 == 41){
	
	
 xold2 = x2;
 yold = y2;
 x2 = 42;
 draw_pacman(x2,y2,xold2,yold,6);



 }		
	

else if ( y2 == 2 &  x2 == 40){
	
	
 xold2 = x2;
 yold = y2;
 x2 = 42;
 draw_pacman(x2,y2,xold2,yold,6);

 }
else if ( y2 == 2 &  x2 == 39){
	
	
 xold2 = x2;
 yold = y2;
 x2 = 42;
 draw_pacman(x2,y2,xold2,yold,6);



 }	 	else if ( y2 == 2 &  x2 == 38){
	
	
 xold2 = x2;
 yold = y2;
 x2 = 42;
 draw_pacman(x2,y2,xold2,yold,6);



 }	 
	
else if ( y2 == 4 &  x2 == 42){
	
	
 xold2 = x2;
 yold = y2;
 x2 = 42;
 draw_pacman(x2,y2,xold2,yold,6);



 }		
	
	
	
else if ( y2 == 4 &  x2 == 41){
	
	
 xold2 = x2;
 yold = y2;
 x2 = 42;
 draw_pacman(x2,y2,xold2,yold,6);



 }		
	

else if ( y2 == 4 &  x2 == 40){
	
	
 xold2 = x2;
 yold = y2;
 x2 = 42;
 draw_pacman(x2,y2,xold2,yold,6);



 }else if ( y2 == 4 &  x2 == 39){
	
	
 xold2 = x2;
 yold = y2;
 x2 = 42;
 draw_pacman(x2,y2,xold2,yold,6);



 }	 	else if ( y2 == 4 &  x2 == 38){
	
	
 xold2 = x2;
 yold = y2;
 x2 = 42;
 draw_pacman(x2,y2,xold2,yold,6);



 }	 
	
	
	
 else if( y2 == 2 & x2 == 54){
 xold2 = x2;
 yold = y2;
 x2 = 54;
 draw_pacman(x2,y2,xold2,yold,6);
 }		 
 else if( y2 == 2 & x2 == 55){
 xold2 = x2;
 yold = y2;
 x2 = 54;
 draw_pacman(x2,y2,xold2,yold,6);
 }		 
	 else if( y2 == 2 & x2 == 56){
 xold2 = x2;
 yold = y2;
 x2 = 54;
 draw_pacman(x2,y2,xold2,yold,6);
 }		 
		 else if( y2 == 2 & x2 == 57){
 xold2 = x2;
 yold = y2;
 x2 = 54;
 draw_pacman(x2,y2,xold2,yold,6);
 }
 
	
 
 else if( y2 == 4 & x2 == 54){
 xold2 = x2;
 yold = y2;
 x2 = 54;
 draw_pacman(x2,y2,xold2,yold,6);
 }		 
 else if( y2 == 4 & x2 == 55){
 xold2 = x2;
 yold = y2;
 x2 = 54;
 draw_pacman(x2,y2,xold2,yold,6);
 }		 
	 else if( y2 == 4 & x2 == 56){
 xold2 = x2;
 yold = y2;
 x2 = 54;
 draw_pacman(x2,y2,xold2,yold,6);
 }		 
		 else if( y2 == 4 & x2 == 57){
 xold2 = x2;
 yold = y2;
 x2 = 54;
 draw_pacman(x2,y2,xold2,yold,6);
 }
 
	 else if(( y2 == 2 & x2 == 82 ) | ( y2 == 2 & x2 == 81) | ( y2 == 2 & x2 == 80 ) | ( y2 == 2 & x2 == 79) | ( y2 == 2 & x2 == 78 ) | ( y2 == 2 & x2 == 77)){
			
		xold2 = x2;
		yold2 = y2;
		x2 = 77; 
		draw_pacman(x2,y2,xold2,yold2,6);
		
	}
		    else if(( y2 == 3 & x2 == 82 ) | ( y2 == 3 & x2 == 81) | ( y2 == 3 & x2 == 80 ) | ( y2 == 3 & x2 == 79) | ( y2 == 3 & x2 == 78 ) | ( y2 == 3 & x2 == 77)){
			
		xold2 = x2;
		yold2 = y2;
		x2 = 77; 
		draw_pacman(x2,y2,xold2,yold2,6);
		
	}
				    else if(( y2 == 5 & x2 == 82 ) | ( y2 == 5 & x2 == 81) | ( y2 == 5 & x2 == 80 ) | ( y2 == 5 & x2 == 79) | ( y2 == 5 & x2 == 78 ) | ( y2 == 5 & x2 == 77)){
			
		xold2 = x2;
		yold2 = y2;
		x2 = 77; 
		draw_pacman(x2,y2,xold2,yold2,6);
		
	}
		else if(( y2 == 0 & x2 == 82 ) | ( y2 == 0 & x2 == 81) | ( y2 == 0 & x2 == 80 ) | ( y2 == 0 & x2 == 79) | ( y2 == 0 & x2 == 78 ) | ( y2 == 0 & x2 == 77)){
			
		xold2 = x2;
		yold2 = y2;
		x2 = 77; 
		draw_pacman(x2,y2,xold2,yold2,6);
		
	}


 
 else if( y2 == 4 & x2 == 54){
 xold2 = x2;
 yold2 = y2;
 x2 = 54;
 draw_pacman(x2,y2,xold2,yold2,6);
 }		 
 else if( y2 == 4 & x2 == 55){
 xold2 = x2;
 yold2 = y2;
 x2 = 54;
 draw_pacman(x2,y2,xold2,yold2,6);
 }		 
	 else if( y2 == 4 & x2 == 56){
 xold2 = x2;
 yold2 = y2;
 x2 = 54;
 draw_pacman(x2,y2,xold2,yold2,6);
 }		 
		 else if( y2 == 4 & x2 == 57){
 xold2 = x2;
 yold2 = y2;
 x2 = 54;
 draw_pacman(x2,y2,xold2,yold2,6);
 }
 
	
	
	
	 else if (( x2 == 20 & y2 == 3 ) | ( x2 == 20 & y2 == 2) | ( x2 == 20 & y2 == 5 )  | ( x2 == 21 & y2 == 5 ) | ( x2 == 22 & y2 == 5 )  | ( x2 == 23 & y2 == 5 ) | ( x2 == 24 & y2 == 5 ) ){
		xold2 = x2;
		yold2 = y2;
		x2 = 20; 
		draw_pacman(x2,y2,xold2,yold2,6);
	 }  
	 
		 else if (( x2 == 20 & y2 == 3 )| ( x2 == 20 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 20; 
		draw_pacman(x2,y2,xold2,yold2,6);
	 }
		 else if (( x2 == 21 & y2 == 3 )| ( x2 == 21 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 20; 
		draw_pacman(x2,y2,xold2,yold2,6);
	 }
		 else if (( x2 == 22 & y2 == 3 )| ( x2 == 22 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 20; 
		draw_pacman(x2,y2,xold2,yold2,6);
	 }
		 else if (( x2 == 23 & y2 == 3 )| ( x2 == 23 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 20; 
		draw_pacman(x2,y2,xold2,yold2,6);
	 }
		 else if (( x2 == 24 & y2 == 3 )| ( x2 == 24 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 20; 
		draw_pacman(x2,y2,xold2,yold2,6);
	 }
		 else if (( x2 == 25 & y2 == 3 )| ( x2 == 25 & y2 == 2)){
		xold2 = x2;
		yold2 = y2;
		x2 = 20; 
		draw_pacman(x2,y2,xold2,yold2,6);
	 }
//   else if (( x2 == 48 & y2 == 0 ) | ( x2 == 49 & y2 == 0) | ( x2 == 50 & y2 == 0 ) | ( x2 == 51  & y2 == 0 ) | ( x2 == 52 & y2 == 0)){
//		xold2 = x2;
//		yold2 = y2;
//		x2 = 48; 
//		draw_pacman(x2,y2,xold2,yold2,6);
//	 }

	
	
	
else
{	
		
		

	
	
	xold2 = x2;
	yold2 = y2;
	x2 -= 7;
  draw_pacman(x2,y2,xold2,yold2,6);
	

		if( (( xold2 == b1x) & (yold2 == b1y)) | ( (xold2 == b1x-1) & ( yold2 == b1y )) | ( (xold2 == b1x-2) & ( yold2 == b1y )) | ( (xold2 == b1x-3) & ( yold2 == b1y )) | ( (xold2 == b1x-4) & ( yold2 == b1y )) | ( (xold2 == b1x-5) & ( yold2 == b1y )) | ( (xold2 == b1x-6) & ( yold2 == b1y )) | ((xold2+1 == b1x ) & (yold2 == b1y)) | ((xold2+2 == b1x ) & (yold2 == b1y)) | ((xold2+3 == b1x ) & (yold2 == b1y)) | ((xold2+4 == b1x ) & (yold2 == b1y)) | ((xold2+5 == b1x ) & (yold2 == b1y))) {



draw_pacman(b1x,b1y,0,0,10);
	
}
//		else if( (( xold2 == b2x) & (yold2 == b2y)) | ( (xold2 == b2x-1) & ( yold2 == b2y )) | ( (xold2 == b2x-2) & ( yold2 == b2y )) | ( (xold2 == b2x-3) & ( yold2 == b2y )) | ( (xold2 == b2x-4) & ( yold2 == b2y )) | ( (xold2 == b2x-5) & ( yold2 == b2y )) | ( (xold2 == b2x-6) & ( yold2 == b2y )) | ((xold2+1 == b2x ) & (yold2 == b2y)) | ((xold2+2 == b2x ) & (yold2 == b2y)) | ((xold2+3 == b2x ) & (yold2 == b2y)) | ((xold2+4 == b2x ) & (yold2 == b2y)) | ((xold2+5 == b2x ) & (yold2 == b2y))) {

//	draw_pacman(b2x,b2y,0,0,10);
//}
		
else if( (( xold2 == b3x) & (yold2 == b3y)) | ( (xold2 == b3x-1) & ( yold2 == b3y )) | ( (xold2 == b3x-2) & ( yold2 == b3y )) | ( (xold2 == b3x-3) & ( yold2 == b3y )) | ( (xold2 == b3x-4) & ( yold2 == b3y )) | ( (xold2 == b3x-5) & ( yold2 == b3y )) | ( (xold2 == b3x-6) & ( yold2 == b3y )) | ((xold2+1 == b3x ) & (yold2 == b3y)) | ((xold2+2 == b3x ) & (yold2 == b3y)) | ((xold2+3 == b3x ) & (yold2 == b3y)) | ((xold2+4 == b3x ) & (yold2 == b3y)) | ((xold2+5 == b3x ) & (yold2 == b3y))) {

draw_pacman(b3x,b3y,0,0,10);	
}
//else if( (( xold2 == b4x) & (yold2 == b4y)) | ( (xold2 == b4x-1) & ( yold2 == b4y )) | ( (xold2 == b4x-2) & ( yold2 == b4y )) | ( (xold2 == b4x-3) & ( yold2 == b4y )) | ( (xold2 == b4x-4) & ( yold2 == b4y )) | ( (xold2 == b4x-5) & ( yold2 == b4y )) | ( (xold2 == b4x-6) & ( yold2 == b4y )) | ((xold2+1 == b4x ) & (yold2 == b4y)) | ((xold2+2 == b4x ) & (yold2 == b4y)) | ((xold2+3 == b4x ) & (yold2 == b4y)) | ((xold2+4 == b4x ) & (yold2 == b4y)) | ((xold2+5 == b4x ) & (yold2 == b4y))) {

//draw_pacman(b4x,b4y,0,0,10);
//	
//}
else if( (( xold2 == b5x) & (yold2 == b5y)) | ( (xold2 == b5x-1) & ( yold2 == b5y )) | ( (xold2 == b5x-2) & ( yold2 == b5y )) | ( (xold2 == b5x-3) & ( yold2 == b5y )) | ( (xold2 == b5x-4) & ( yold2 == b5y )) | ( (xold2 == b5x-5) & ( yold2 == b5y )) | ( (xold2 == b5x-6) & ( yold2 == b5y )) | ((xold2+1 == b5x ) & (yold2 == b5y)) | ((xold2+2 == b5x ) & (yold2 == b5y)) | ((xold2+3 == b5x ) & (yold2 == b5y)) | ((xold2+4 == b5x ) & (yold2 == b5y)) | ((xold2+5 == b5x ) & (yold2 == b5y))) {

draw_pacman(b5x,b5y,0,0,10);

}
//else if( (( xold2 == b6x) & (yold2 == b6y)) | ( (xold2 == b6x-1) & ( yold2 == b6y )) | ( (xold2 == b6x-2) & ( yold2 == b6y )) | ( (xold2 == b6x-3) & ( yold2 == b6y )) | ( (xold2 == b6x-4) & ( yold2 == b6y )) | ( (xold2 == b6x-5) & ( yold2 == b6y )) | ( (xold2 == b6x-6) & ( yold2 == b6y )) | ((xold2+1 == b6x ) & (yold2 == b6y)) | ((xold2+2 == b6x ) & (yold2 == b6y)) | ((xold2+3 == b6x ) & (yold2 == b6y)) | ((xold2+4 == b6x ) & (yold2 == b6y)) | ((xold2+5 == b6x ) & (yold2 == b6y))) {

//draw_pacman(b6x,b6y,0,0,10);
//	
//}
else if( (( xold2 == b7x) & (yold2 == b7y)) | ( (xold2 == b7x-1) & ( yold2 == b7y )) | ( (xold2 == b7x-2) & ( yold2 == b7y )) | ( (xold2 == b7x-3) & ( yold2 == b7y )) | ( (xold2 == b7x-4) & ( yold2 == b7y )) | ( (xold2 == b7x-5) & ( yold2 == b7y )) | ( (xold2 == b7x-6) & ( yold2 == b7y )) | ((xold2+1 == b7x ) & (yold2 == b7y)) | ((xold2+2 == b7x ) & (yold2 == b7y)) | ((xold2+3 == b7x ) & (yold2 == b7y)) | ((xold2+4 == b7x ) & (yold2 == b7y)) | ((xold2+5 == b7x ) & (yold2 == b7y))) {

draw_pacman(b7x,b7y,0,0,10);	
}
//else if( (( xold2 == b8x) & (yold2 == b8y)) | ( (xold2 == b8x-1) & ( yold2 == b8y )) | ( (xold2 == b8x-2) & ( yold2 == b8y )) | ( (xold2 == b8x-3) & ( yold2 == b8y )) | ( (xold2 == b8x-4) & ( yold2 == b8y )) | ( (xold2 == b8x-5) & ( yold2 == b8y )) | ( (xold2 == b8x-6) & ( yold2 == b8y )) | ((xold2+1 == b8x ) & (yold2 == b8y)) | ((xold2+2 == b8x ) & (yold2 == b8y)) | ((xold2+3 == b8x ) & (yold2 == b8y)) | ((xold2+4 == b8x ) & (yold2 == b8y)) | ((xold2+5 == b8x ) & (yold2 == b8y))) {
//draw_pacman(b8x,b8y,0,0,10);

//}
else if( (( xold2 == b9x) & (yold2 == b9y)) | ( (xold2 == b9x-1) & ( yold2 == b9y )) | ( (xold2 == b9x-2) & ( yold2 == b9y )) | ( (xold2 == b9x-3) & ( yold2 == b9y )) | ( (xold2 == b9x-4) & ( yold2 == b9y )) | ( (xold2 == b9x-5) & ( yold2 == b9y )) | ( (xold2 == b9x-6) & ( yold2 == b9y )) | ((xold2+1 == b9x ) & (yold2 == b9y)) | ((xold2+2 == b9x ) & (yold2 == b9y)) | ((xold2+3 == b9x ) & (yold2 == b9y)) | ((xold2+4 == b9x ) & (yold2 == b9y)) | ((xold2+5 == b9x ) & (yold2 == b9y))) {

draw_pacman(b9x,b9y,0,0,10);
	
}
//else if( (( xold2 == b10x) & (yold2 == b10y)) | ( (xold2 == b10x-1) & ( yold2 == b10y )) | ( (xold2 == b10x-2) & ( yold2 == b10y )) | ( (xold2 == b10x-3) & ( yold2 == b10y )) | ( (xold2 == b10x-4) & ( yold2 == b10y )) | ( (xold2 == b10x-5) & ( yold2 == b10y )) | ( (xold2 == b10x-6) & ( yold2 == b10y )) | ((xold2+1 == b10x ) & (yold2 == b10y)) | ((xold2+2 == b10x ) & (yold2 == b10y)) | ((xold2+3 == b10x ) & (yold2 == b10y)) | ((xold2+4 == b10x ) & (yold2 == b10y)) | ((xold2+5 == b10x ) & (yold2 == b10y))) {
//draw_pacman(b10x,b10y,0,0,10);

//}
else if( (( xold2 == b11x) & (yold2 == b11y)) | ( (xold2 == b11x-1) & ( yold2 == b11y )) | ( (xold2 == b11x-2) & ( yold2 == b11y )) | ( (xold2 == b11x-3) & ( yold2 == b11y )) | ( (xold2 == b11x-4) & ( yold2 == b11y )) | ( (xold2 == b11x-5) & ( yold2 == b11y )) | ( (xold2 == b11x-6) & ( yold2 == b11y )) | ((xold2+1 == b11x ) & (yold2 == b11y)) | ((xold2+2 == b11x ) & (yold2 == b11y)) | ((xold2+3 == b11x ) & (yold2 == b11y)) | ((xold2+4 == b11x ) & (yold2 == b11y)) | ((xold2+5 == b11x ) & (yold2 == b11y))) {

draw_pacman(b11x,b11y,0,0,10);
	
}
else if( (( xold2 == b12x) & (yold2 == b12y)) | ( (xold2 == b12x-1) & ( yold2 == b12y )) | ( (xold2 == b12x-2) & ( yold2 == b12y )) | ( (xold2 == b12x-3) & ( yold2 == b12y )) | ( (xold2 == b12x-4) & ( yold2 == b12y )) | ( (xold2 == b12x-5) & ( yold2 == b12y )) | ( (xold2 == b12x-6) & ( yold2 == b12y )) | ((xold2+1 == b12x ) & (yold2 == b12y)) | ((xold2+2 == b12x ) & (yold2 == b12y)) | ((xold2+3 == b12x ) & (yold2 == b12y)) | ((xold2+4 == b12x ) & (yold2 == b12y)) | ((xold2+5 == b12x ) & (yold2 == b12y))) {

draw_pacman(b12x,b12y,0,0,10);
	
}

else if( (( xold2 == b18x) & (yold2 == b18y)) | ( (xold2 == b18x-1) & ( yold2 == b18y )) | ( (xold2 == b18x-2) & ( yold2 == b18y )) | ( (xold2 == b18x-3) & ( yold2 == b18y )) | ( (xold2 == b18x-4) & ( yold2 == b18y )) | ( (xold2 == b18x-5) & ( yold2 == b18y )) | ( (xold2 == b18x-6) & ( yold2 == b18y )) | ((xold2+1 == b18x ) & (yold2 == b18y)) | ((xold2+2 == b18x ) & (yold2 == b18y)) | ((xold2+3 == b18x ) & (yold2 == b18y)) | ((xold2+4 == b18x ) & (yold2 == b18y)) | ((xold2+5 == b18x ) & (yold2 == b18y))) {

draw_pacman(b18x,b18y,0,0,10);
}

else if( (( xold2 == b20x) & (yold2 == b20y)) | ( (xold2 == b20x-1) & ( yold2 == b20y )) | ( (xold2 == b20x-2) & ( yold2 == b20y )) | ( (xold2 == b20x-3) & ( yold2 == b20y )) | ( (xold2 == b20x-4) & ( yold2 == b20y )) | ( (xold2 == b20x-5) & ( yold2 == b20y )) | ( (xold2 == b20x-6) & ( yold2 == b20y )) | ((xold2+1 == b20x ) & (yold2 == b20y)) | ((xold2+2 == b20x ) & (yold2 == b20y)) | ((xold2+3 == b20x ) & (yold2 == b20y)) | ((xold2+4 == b20x ) & (yold2 == b20y)) | ((xold2+5 == b20x ) & (yold2 == b20y))) {

draw_pacman(b20x,b20y,0,0,10);
	
}

else if( (( xold2 == b23x) & (yold2 == b23y)) | ( (xold2 == b23x-1) & ( yold2 == b23y )) | ( (xold2 == b23x-2) & ( yold2 == b23y )) | ( (xold2 == b23x-3) & ( yold2 == b23y )) | ( (xold2 == b23x-4) & ( yold2 == b23y )) | ( (xold2 == b23x-5) & ( yold2 == b23y )) | ( (xold2 == b23x-6) & ( yold2 == b23y )) | ((xold2+1 == b23x ) & (yold2 == b23y)) | ((xold2+2 == b23x ) & (yold2 == b23y)) | ((xold2+3 == b23x ) & (yold2 == b23y)) | ((xold2+4 == b23x ) & (yold2 == b23y)) | ((xold2+5 == b23x ) & (yold2 == b23y))) {

draw_pacman(b23x,b23y,0,0,10);
}
else if( (( xold2 == b24x) & (yold2 == b24y)) | ( (xold2 == b24x-1) & ( yold2 == b24y )) | ( (xold2 == b24x-2) & ( yold2 == b24y )) | ( (xold2 == b24x-3) & ( yold2 == b24y )) | ( (xold2 == b24x-4) & ( yold2 == b24y )) | ( (xold2 == b24x-5) & ( yold2 == b24y )) | ( (xold2 == b24x-6) & ( yold2 == b24y )) | ((xold2+1 == b24x ) & (yold2 == b24y)) | ((xold2+2 == b24x ) & (yold2 == b24y)) | ((xold2+3 == b24x ) & (yold2 == b24y)) | ((xold2+4 == b24x ) & (yold2 == b24y)) | ((xold2+5 == b24x ) & (yold2 == b24y))) {

	draw_pacman(b24x,b24y,0,0,10);
}

else if( (( xold2 == b28x) & (yold2 == b28y)) | ( (xold2 == b28x-1) & ( yold2 == b28y )) | ( (xold2 == b28x-2) & ( yold2 == b28y )) | ( (xold2 == b28x-3) & ( yold2 == b28y )) | ( (xold2 == b28x-4) & ( yold2 == b28y )) | ( (xold2 == b28x-5) & ( yold2 == b28y )) | ( (xold2 == b28x-6) & ( yold2 == b28y )) | ((xold2+1 == b28x ) & (yold2 == b28y)) | ((xold2+2 == b28x ) & (yold2 == b28y)) | ((xold2+3 == b28x ) & (yold2 == b28y)) | ((xold2+4 == b28x ) & (yold2 == b28y)) | ((xold2+5 == b28x ) & (yold2 == b28y))) {

	draw_pacman(b28x,b28y,0,0,10);
}
else if( (( xold2 == b29x) & (yold2 == b29y)) | ( (xold2 == b29x-1) & ( yold2 == b29y )) | ( (xold2 == b29x-2) & ( yold2 == b29y )) | ( (xold2 == b29x-3) & ( yold2 == b29y )) | ( (xold2 == b29x-4) & ( yold2 == b29y )) | ( (xold2 == b29x-5) & ( yold2 == b29y )) | ( (xold2 == b29x-6) & ( yold2 == b29y )) | ((xold2+1 == b29x ) & (yold2 == b29y)) | ((xold2+2 == b29x ) & (yold2 == b29y)) | ((xold2+3 == b29x ) & (yold2 == b29y)) | ((xold2+4 == b29x ) & (yold2 == b29y)) | ((xold2+5 == b29x ) & (yold2 == b29y))) {

draw_pacman(b29x,b29y,0,0,10);
}
else if( (( xold2 == b30x) & (yold2 == b30y)) | ( (xold2 == b30x-1) & ( yold2 == b30y )) | ( (xold2 == b30x-2) & ( yold2 == b30y )) | ( (xold2 == b30x-3) & ( yold2 == b30y )) | ( (xold2 == b30x-4) & ( yold2 == b30y )) | ( (xold2 == b30x-5) & ( yold2 == b30y )) | ( (xold2 == b30x-6) & ( yold2 == b30y )) | ((xold2+1 == b30x ) & (yold2 == b30y)) | ((xold2+2 == b30x ) & (yold2 == b30y)) | ((xold2+3 == b30x ) & (yold2 == b30y)) | ((xold2+4 == b30x ) & (yold2 == b30y)) | ((xold2+5 == b30x ) & (yold2 == b30y))) {
	draw_pacman(b30x,b30y,0,0,10);
}

else if( (( xold2 == b32x) & (yold2 == b32y)) | ( (xold2 == b32x-1) & ( yold2 == b32y )) | ( (xold2 == b32x-2) & ( yold2 == b32y )) | ( (xold2 == b32x-3) & ( yold2 == b32y )) | ( (xold2 == b32x-4) & ( yold2 == b32y )) | ( (xold2 == b32x-5) & ( yold2 == b32y )) | ( (xold2 == b32x-6) & ( yold2 == b32y )) | ((xold2+1 == b32x ) & (yold2 == b32y)) | ((xold2+2 == b32x ) & (yold2 == b32y)) | ((xold2+3 == b32x ) & (yold2 == b32y)) | ((xold2+4 == b32x ) & (yold2 == b32y)) | ((xold2+5 == b32x ) & (yold2 == b32y))) {
draw_pacman(b32x,b32y,0,0,10);

}

else if( (( xold2 == b34x) & (yold2 == b34y)) | ( (xold2 == b34x-1) & ( yold2 == b34y )) | ( (xold2 == b34x-2) & ( yold2 == b34y )) | ( (xold2 == b34x-3) & ( yold2 == b34y )) | ( (xold2 == b34x-4) & ( yold2 == b34y )) | ( (xold2 == b34x-5) & ( yold2 == b34y )) | ( (xold2 == b34x-6) & ( yold2 == b34y )) | ((xold2+1 == b34x ) & (yold2 == b34y)) | ((xold2+2 == b34x ) & (yold2 == b34y)) | ((xold2+3 == b34x ) & (yold2 == b34y)) | ((xold2+4 == b34x ) & (yold2 == b34y)) | ((xold2+5 == b34x ) & (yold2 == b34y))) {
	draw_pacman(b34x,b34y,0,0,10);
}
	
	
}
}
	
	

	
	
ADC0->SC1[0] = 23; /* start conversion on channel 0 */
while(!(ADC0->SC1[0] & 0x80)) { } /* wait for conversion complete */
second2 = ADC0->R[0]; /* read conversion result and clear COCO flag PTE20 */	



	if( second2 > 4000){
		
	 if( y2 == 6){
	 xold2 = x2;
	 yold2 = y2;
	 y2 = 0;
   draw_pacman(x2,y2,xold2,yold2,7);

 }
	 
 else if( y2 == 1 & x2 == 7){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 else if( y2 == 1 & x2 == 8){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 
 else if( y2 == 1 & x2 == 9){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		
 else if( y2 == 1 & x2 == 10){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 else if( y2 == 1 & x2 == 11){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 
 else if( y2 == 1 & x2 == 12){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 else if( y2 == 1 & x2 == 13){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
	 else if( y2 == 1 & x2 == 14){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
		 else if( y2 == 1 & x2 == 15){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }
 else if( y2 == 1 & x2 == 16){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 else if( y2 == 1 & x2 == 17){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 	 
	 else if( y2 == 1 & x2 == 18){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 

 else if( ( y2 == 1 & x2 == 28) |( y2 == 1 & x2 == 29) | ( y2 == 1 & x2 == 30) | ( y2 == 1 & x2 == 31) | ( y2 == 1 & x2 == 32) | ( y2 == 1 & x2 == 33) | ( y2 == 1 & x2 == 34) | ( y2 == 1 & x2 == 35) | ( y2 == 1 & x2 == 36) | ( y2 == 1 & x2 == 37)){
	
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 
}
 
 else if( y2 == 1 & x2 == 62){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 else if( y2 == 1 & x2 == 63){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 	 
	 else if( y2 == 1 & x2 == 64){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 else if( y2 == 1 & x2 == 65){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 
 else if( y2 == 1 & x2 == 66){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 else if( y2 == 1 & x2 == 67){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
	 else if( y2 == 1 & x2 == 68){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
		 else if( y2 == 1 & x2 == 69){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }
 else if( y2 == 1 & x2 == 70){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 
 
 
  else if( y2 == 4 & x2 == 7){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 else if( y2 == 4 & x2 == 8){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 
 else if( y2 == 4 & x2 == 9){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		
 else if( y2 == 4 & x2 == 10){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 else if( y2 == 4 & x2 == 11){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 
 else if( y2 == 4 & x2 == 12){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 else if( y2 == 4 & x2 == 13){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
	 else if( y2 == 4 & x2 == 14){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
		 else if( y2 == 4 & x2 == 15){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }
 else if( y2 == 4 & x2 == 16){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 else if( y2 == 4 & x2 == 17){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 	 
	 else if( y2 == 4 & x2 == 18){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 

 else if( y2 == 4 & x2 == 63){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 	 
	 else if( y2 == 4 & x2 == 64){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 else if( y2 == 4 & x2 == 65){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 
 else if( y2 == 4 & x2 == 66){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
 else if( y2 == 4 & x2 == 67){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
	 else if( y2 == 4 & x2 == 68){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
		 else if( y2 == 4 & x2 == 69){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }
 else if( y2 == 4 & x2 == 70){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,7);
 }		 
else
{	
		
		
	xold2 = x2;
	yold2 = y2;
	y2 += 1;
  draw_pacman(x2,y2,xold2,yold2,7);
	

		if( (( xold2 == b1x) & (yold2 == b1y)) | ( (xold2 == b1x-1) & ( yold2 == b1y )) | ( (xold2 == b1x-2) & ( yold2 == b1y )) | ( (xold2 == b1x-3) & ( yold2 == b1y )) | ( (xold2 == b1x-4) & ( yold2 == b1y )) | ( (xold2 == b1x-5) & ( yold2 == b1y )) | ( (xold2 == b1x-6) & ( yold2 == b1y )) | ((xold2+1 == b1x ) & (yold2 == b1y)) | ((xold2+2 == b1x ) & (yold2 == b1y)) | ((xold2+3 == b1x ) & (yold2 == b1y)) | ((xold2+4 == b1x ) & (yold2 == b1y)) | ((xold2+5 == b1x ) & (yold2 == b1y))) {



draw_pacman(b1x,b1y,0,0,10);
	
}

		
else if( (( xold2 == b3x) & (yold2 == b3y)) | ( (xold2 == b3x-1) & ( yold2 == b3y )) | ( (xold2 == b3x-2) & ( yold2 == b3y )) | ( (xold2 == b3x-3) & ( yold2 == b3y )) | ( (xold2 == b3x-4) & ( yold2 == b3y )) | ( (xold2 == b3x-5) & ( yold2 == b3y )) | ( (xold2 == b3x-6) & ( yold2 == b3y )) | ((xold2+1 == b3x ) & (yold2 == b3y)) | ((xold2+2 == b3x ) & (yold2 == b3y)) | ((xold2+3 == b3x ) & (yold2 == b3y)) | ((xold2+4 == b3x ) & (yold2 == b3y)) | ((xold2+5 == b3x ) & (yold2 == b3y))) {

draw_pacman(b3x,b3y,0,0,10);	
}

else if( (( xold2 == b5x) & (yold2 == b5y)) | ( (xold2 == b5x-1) & ( yold2 == b5y )) | ( (xold2 == b5x-2) & ( yold2 == b5y )) | ( (xold2 == b5x-3) & ( yold2 == b5y )) | ( (xold2 == b5x-4) & ( yold2 == b5y )) | ( (xold2 == b5x-5) & ( yold2 == b5y )) | ( (xold2 == b5x-6) & ( yold2 == b5y )) | ((xold2+1 == b5x ) & (yold2 == b5y)) | ((xold2+2 == b5x ) & (yold2 == b5y)) | ((xold2+3 == b5x ) & (yold2 == b5y)) | ((xold2+4 == b5x ) & (yold2 == b5y)) | ((xold2+5 == b5x ) & (yold2 == b5y))) {

draw_pacman(b5x,b5y,0,0,10);

}

else if( (( xold2 == b7x) & (yold2 == b7y)) | ( (xold2 == b7x-1) & ( yold2 == b7y )) | ( (xold2 == b7x-2) & ( yold2 == b7y )) | ( (xold2 == b7x-3) & ( yold2 == b7y )) | ( (xold2 == b7x-4) & ( yold2 == b7y )) | ( (xold2 == b7x-5) & ( yold2 == b7y )) | ( (xold2 == b7x-6) & ( yold2 == b7y )) | ((xold2+1 == b7x ) & (yold2 == b7y)) | ((xold2+2 == b7x ) & (yold2 == b7y)) | ((xold2+3 == b7x ) & (yold2 == b7y)) | ((xold2+4 == b7x ) & (yold2 == b7y)) | ((xold2+5 == b7x ) & (yold2 == b7y))) {

draw_pacman(b7x,b7y,0,0,10);	
}

else if( (( xold2 == b9x) & (yold2 == b9y)) | ( (xold2 == b9x-1) & ( yold2 == b9y )) | ( (xold2 == b9x-2) & ( yold2 == b9y )) | ( (xold2 == b9x-3) & ( yold2 == b9y )) | ( (xold2 == b9x-4) & ( yold2 == b9y )) | ( (xold2 == b9x-5) & ( yold2 == b9y )) | ( (xold2 == b9x-6) & ( yold2 == b9y )) | ((xold2+1 == b9x ) & (yold2 == b9y)) | ((xold2+2 == b9x ) & (yold2 == b9y)) | ((xold2+3 == b9x ) & (yold2 == b9y)) | ((xold2+4 == b9x ) & (yold2 == b9y)) | ((xold2+5 == b9x ) & (yold2 == b9y))) {

draw_pacman(b9x,b9y,0,0,10);
	
}

else if( (( xold2 == b11x) & (yold2 == b11y)) | ( (xold2 == b11x-1) & ( yold2 == b11y )) | ( (xold2 == b11x-2) & ( yold2 == b11y )) | ( (xold2 == b11x-3) & ( yold2 == b11y )) | ( (xold2 == b11x-4) & ( yold2 == b11y )) | ( (xold2 == b11x-5) & ( yold2 == b11y )) | ( (xold2 == b11x-6) & ( yold2 == b11y )) | ((xold2+1 == b11x ) & (yold2 == b11y)) | ((xold2+2 == b11x ) & (yold2 == b11y)) | ((xold2+3 == b11x ) & (yold2 == b11y)) | ((xold2+4 == b11x ) & (yold2 == b11y)) | ((xold2+5 == b11x ) & (yold2 == b11y))) {

draw_pacman(b11x,b11y,0,0,10);
	
}
else if( (( xold2 == b12x) & (yold2 == b12y)) | ( (xold2 == b12x-1) & ( yold2 == b12y )) | ( (xold2 == b12x-2) & ( yold2 == b12y )) | ( (xold2 == b12x-3) & ( yold2 == b12y )) | ( (xold2 == b12x-4) & ( yold2 == b12y )) | ( (xold2 == b12x-5) & ( yold2 == b12y )) | ( (xold2 == b12x-6) & ( yold2 == b12y )) | ((xold2+1 == b12x ) & (yold2 == b12y)) | ((xold2+2 == b12x ) & (yold2 == b12y)) | ((xold2+3 == b12x ) & (yold2 == b12y)) | ((xold2+4 == b12x ) & (yold2 == b12y)) | ((xold2+5 == b12x ) & (yold2 == b12y))) {

draw_pacman(b12x,b12y,0,0,10);
	
}

else if( (( xold2 == b18x) & (yold2 == b18y)) | ( (xold2 == b18x-1) & ( yold2 == b18y )) | ( (xold2 == b18x-2) & ( yold2 == b18y )) | ( (xold2 == b18x-3) & ( yold2 == b18y )) | ( (xold2 == b18x-4) & ( yold2 == b18y )) | ( (xold2 == b18x-5) & ( yold2 == b18y )) | ( (xold2 == b18x-6) & ( yold2 == b18y )) | ((xold2+1 == b18x ) & (yold2 == b18y)) | ((xold2+2 == b18x ) & (yold2 == b18y)) | ((xold2+3 == b18x ) & (yold2 == b18y)) | ((xold2+4 == b18x ) & (yold2 == b18y)) | ((xold2+5 == b18x ) & (yold2 == b18y))) {

draw_pacman(b18x,b18y,0,0,10);
}

else if( (( xold2 == b20x) & (yold2 == b20y)) | ( (xold2 == b20x-1) & ( yold2 == b20y )) | ( (xold2 == b20x-2) & ( yold2 == b20y )) | ( (xold2 == b20x-3) & ( yold2 == b20y )) | ( (xold2 == b20x-4) & ( yold2 == b20y )) | ( (xold2 == b20x-5) & ( yold2 == b20y )) | ( (xold2 == b20x-6) & ( yold2 == b20y )) | ((xold2+1 == b20x ) & (yold2 == b20y)) | ((xold2+2 == b20x ) & (yold2 == b20y)) | ((xold2+3 == b20x ) & (yold2 == b20y)) | ((xold2+4 == b20x ) & (yold2 == b20y)) | ((xold2+5 == b20x ) & (yold2 == b20y))) {

draw_pacman(b20x,b20y,0,0,10);
	
}

else if( (( xold2 == b23x) & (yold2 == b23y)) | ( (xold2 == b23x-1) & ( yold2 == b23y )) | ( (xold2 == b23x-2) & ( yold2 == b23y )) | ( (xold2 == b23x-3) & ( yold2 == b23y )) | ( (xold2 == b23x-4) & ( yold2 == b23y )) | ( (xold2 == b23x-5) & ( yold2 == b23y )) | ( (xold2 == b23x-6) & ( yold2 == b23y )) | ((xold2+1 == b23x ) & (yold2 == b23y)) | ((xold2+2 == b23x ) & (yold2 == b23y)) | ((xold2+3 == b23x ) & (yold2 == b23y)) | ((xold2+4 == b23x ) & (yold2 == b23y)) | ((xold2+5 == b23x ) & (yold2 == b23y))) {

draw_pacman(b23x,b23y,0,0,10);
}
else if( (( xold2 == b24x) & (yold2 == b24y)) | ( (xold2 == b24x-1) & ( yold2 == b24y )) | ( (xold2 == b24x-2) & ( yold2 == b24y )) | ( (xold2 == b24x-3) & ( yold2 == b24y )) | ( (xold2 == b24x-4) & ( yold2 == b24y )) | ( (xold2 == b24x-5) & ( yold2 == b24y )) | ( (xold2 == b24x-6) & ( yold2 == b24y )) | ((xold2+1 == b24x ) & (yold2 == b24y)) | ((xold2+2 == b24x ) & (yold2 == b24y)) | ((xold2+3 == b24x ) & (yold2 == b24y)) | ((xold2+4 == b24x ) & (yold2 == b24y)) | ((xold2+5 == b24x ) & (yold2 == b24y))) {

	draw_pacman(b24x,b24y,0,0,10);
}

else if( (( xold2 == b28x) & (yold2 == b28y)) | ( (xold2 == b28x-1) & ( yold2 == b28y )) | ( (xold2 == b28x-2) & ( yold2 == b28y )) | ( (xold2 == b28x-3) & ( yold2 == b28y )) | ( (xold2 == b28x-4) & ( yold2 == b28y )) | ( (xold2 == b28x-5) & ( yold2 == b28y )) | ( (xold2 == b28x-6) & ( yold2 == b28y )) | ((xold2+1 == b28x ) & (yold2 == b28y)) | ((xold2+2 == b28x ) & (yold2 == b28y)) | ((xold2+3 == b28x ) & (yold2 == b28y)) | ((xold2+4 == b28x ) & (yold2 == b28y)) | ((xold2+5 == b28x ) & (yold2 == b28y))) {

	draw_pacman(b28x,b28y,0,0,10);
}
else if( (( xold2 == b29x) & (yold2 == b29y)) | ( (xold2 == b29x-1) & ( yold2 == b29y )) | ( (xold2 == b29x-2) & ( yold2 == b29y )) | ( (xold2 == b29x-3) & ( yold2 == b29y )) | ( (xold2 == b29x-4) & ( yold2 == b29y )) | ( (xold2 == b29x-5) & ( yold2 == b29y )) | ( (xold2 == b29x-6) & ( yold2 == b29y )) | ((xold2+1 == b29x ) & (yold2 == b29y)) | ((xold2+2 == b29x ) & (yold2 == b29y)) | ((xold2+3 == b29x ) & (yold2 == b29y)) | ((xold2+4 == b29x ) & (yold2 == b29y)) | ((xold2+5 == b29x ) & (yold2 == b29y))) {

draw_pacman(b29x,b29y,0,0,10);
}
else if( (( xold2 == b30x) & (yold2 == b30y)) | ( (xold2 == b30x-1) & ( yold2 == b30y )) | ( (xold2 == b30x-2) & ( yold2 == b30y )) | ( (xold2 == b30x-3) & ( yold2 == b30y )) | ( (xold2 == b30x-4) & ( yold2 == b30y )) | ( (xold2 == b30x-5) & ( yold2 == b30y )) | ( (xold2 == b30x-6) & ( yold2 == b30y )) | ((xold2+1 == b30x ) & (yold2 == b30y)) | ((xold2+2 == b30x ) & (yold2 == b30y)) | ((xold2+3 == b30x ) & (yold2 == b30y)) | ((xold2+4 == b30x ) & (yold2 == b30y)) | ((xold2+5 == b30x ) & (yold2 == b30y))) {
	draw_pacman(b30x,b30y,0,0,10);
}

else if( (( xold2 == b32x) & (yold2 == b32y)) | ( (xold2 == b32x-1) & ( yold2 == b32y )) | ( (xold2 == b32x-2) & ( yold2 == b32y )) | ( (xold2 == b32x-3) & ( yold2 == b32y )) | ( (xold2 == b32x-4) & ( yold2 == b32y )) | ( (xold2 == b32x-5) & ( yold2 == b32y )) | ( (xold2 == b32x-6) & ( yold2 == b32y )) | ((xold2+1 == b32x ) & (yold2 == b32y)) | ((xold2+2 == b32x ) & (yold2 == b32y)) | ((xold2+3 == b32x ) & (yold2 == b32y)) | ((xold2+4 == b32x ) & (yold2 == b32y)) | ((xold2+5 == b32x ) & (yold2 == b32y))) {
draw_pacman(b32x,b32y,0,0,10);

}

else if( (( xold2 == b34x) & (yold2 == b34y)) | ( (xold2 == b34x-1) & ( yold2 == b34y )) | ( (xold2 == b34x-2) & ( yold2 == b34y )) | ( (xold2 == b34x-3) & ( yold2 == b34y )) | ( (xold2 == b34x-4) & ( yold2 == b34y )) | ( (xold2 == b34x-5) & ( yold2 == b34y )) | ( (xold2 == b34x-6) & ( yold2 == b34y )) | ((xold2+1 == b34x ) & (yold2 == b34y)) | ((xold2+2 == b34x ) & (yold2 == b34y)) | ((xold2+3 == b34x ) & (yold2 == b34y)) | ((xold2+4 == b34x ) & (yold2 == b34y)) | ((xold2+5 == b34x ) & (yold2 == b34y))) {
	draw_pacman(b34x,b34y,0,0,10);
}



}
}
	else if(second2 < 500){
		
			if(y2 == 0){
		xold2 = x2;
	  yold2 = y2;
    y2 = 6;
  draw_pacman(x2,y2,xold2,yold2,8);
    }
	
				
else if( y2 == 1 & x2 == 7){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 else if( y2 == 1 & x2 == 8){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 
 else if( y2 == 1 & x2 == 9){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 			
		
		
		
		
 else if( y2 == 1 & x2 == 10){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 else if( y2 == 1 & x2 == 11){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 
 else if( y2 == 1 & x2 == 12){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 else if( y2 == 1 & x2 == 13){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
	 else if( y2 == 1 & x2 == 14){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
		 else if( y2 == 1 & x2 == 15){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }
 else if( y2 == 1 & x2 == 16){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 else if( y2 == 1 & x2 == 17){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 	 
	 else if( y2 == 1 & x2 == 18){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 

 else if( ( y2 == 5 & x2 == 28) |( y2 == 5 & x2 == 29) | ( y2 == 5 & x2 == 30) | ( y2 == 5 & x2 == 31) | ( y2 == 5 & x2 == 32) | ( y2 == 5 & x2 == 33) | ( y2 == 5 & x2 == 34) | ( y2 == 5 & x2 == 35) | ( y2 == 5 & x2 == 36) | ( y2 == 5 & x2 == 37)){
	
 xold2 = x2;
 yold2 = y2;
 y2 = 5;
 draw_pacman(x2,y2,xold2,yold2,8);	
}

else if(  ( y2 == 5 & x2 == 44) | ( y2 == 5 & x2 == 45) | ( y2 == 5 & x2 == 46) | ( y2 == 5 & x2 == 47) | ( y2 == 5 & x2 == 48) | ( y2 == 5 & x2 == 49) | ( y2 == 5 & x2 == 50) | ( y2 == 5 & x2 == 51) | ( y2 == 5 & x2 == 52)){
	
 xold2 = x2;
 yold2 = y2;
 y2 = 5;
 draw_pacman(x2,y2,xold2,yold2,8);	
}

else if(  ( y2 == 3 & x2 == 44) | ( y2 == 3 & x2 == 45) | ( y2 == 3 & x2 == 46) | ( y2 == 3 & x2 == 47) | ( y2 == 3 & x2 == 48) | ( y2 == 3 & x2 == 49) | ( y2 == 3 & x2 == 50) | ( y2 == 3 & x2 == 51) | ( y2 == 3 & x2 == 52)){
	
 xold2 = x2;
 yold2 = y2;
 y2 = 3;
 draw_pacman(x2,y2,xold2,yold2,8);	
}

else if( ( y2 == 3 & x2 == 28) |( y2 == 3 & x2 == 29) | ( y2 == 3 & x2 == 30) | ( y2 == 3 & x2 == 31) | ( y2 == 3 & x2 == 32) | ( y2 == 3 & x2 == 33) | ( y2 == 3 & x2 == 34) | ( y2 == 3 & x2 == 35) | ( y2 == 3 & x2 == 36) | ( y2 == 3 & x2 == 37)){
	
 xold2 = x2;
 yold2 = y2;
 y2 = 3;
 draw_pacman(x2,y2,xold2,yold2,8);
 
 }
 
 
 
 
  else if( y2 == 4 & x2 == 7){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 else if( y2 == 4 & x2 == 8){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 
 else if( y2 == 4 & x2 == 9){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		
 else if( y2 == 4 & x2 == 10){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 else if( y2 == 4 & x2 == 11){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 
 else if( y2 == 4 & x2 == 12){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 else if( y2 == 4 & x2 == 13){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
	 else if( y2 == 4 & x2 == 14){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
		 else if( y2 == 4 & x2 == 15){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }
 else if( y2 == 4 & x2 == 16){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 else if( y2 == 4 & x2 == 17){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 	 
	 else if( y2 == 4 & x2 == 18){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 

 else if( y2 == 4 & x2 == 63){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 	 
	 else if( y2 == 4 & x2 == 64){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 else if( y2 == 4 & x2 == 65){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 
 else if( y2 == 4 & x2 == 66){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 else if( y2 == 4 & x2 == 67){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
	 else if( y2 == 4 & x2 == 68){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
		 else if( y2 == 4 & x2 == 69){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }
 else if( y2 == 4 & x2 == 70){
 xold2 = x2;
 yold2 = y2;
 y2 = 4;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 
 
 else if( y2 == 1 & x2 == 58){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 else if( y2 == 1 & x2 == 59){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
	 else if( y2 == 1 & x2 == 60){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
		 else if( y2 == 1 & x2 == 61){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }
 else if( y2 == 1 & x2 == 62){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 else if( y2 == 1 & x2 == 63){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 	 
	 else if( y2 == 1 & x2 == 64){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 else if( y2 == 1 & x2 == 65){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 
 else if( y2 == 1 & x2 == 66){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 else if( y2 == 1 & x2 == 67){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
	 else if( y2 == 1 & x2 == 68){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
		 else if( y2 == 1 & x2 == 69){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }
 else if( y2 == 1 & x2 == 70){
 xold2 = x2;
 yold2 = y2;
 y2 = 1;
 draw_pacman(x2,y2,xold2,yold2,8);
 }		 
 
		
		
		
		
		
		else
			
		{
		
	xold2 = x2;
	yold2 = y2;
	y2 -= 1;
  draw_pacman(x2,y2,xold2,yold2,8);
	
			
					if( (( xold2 == b1x) & (yold2 == b1y)) | ( (xold2 == b1x-1) & ( yold2 == b1y )) | ( (xold2 == b1x-2) & ( yold2 == b1y )) | ( (xold2 == b1x-3) & ( yold2 == b1y )) | ( (xold2 == b1x-4) & ( yold2 == b1y )) | ( (xold2 == b1x-5) & ( yold2 == b1y )) | ( (xold2 == b1x-6) & ( yold2 == b1y )) | ((xold2+1 == b1x ) & (yold2 == b1y)) | ((xold2+2 == b1x ) & (yold2 == b1y)) | ((xold2+3 == b1x ) & (yold2 == b1y)) | ((xold2+4 == b1x ) & (yold2 == b1y)) | ((xold2+5 == b1x ) & (yold2 == b1y))) {



draw_pacman(b1x,b1y,0,0,10);
	
}

		
else if( (( xold2 == b3x) & (yold2 == b3y)) | ( (xold2 == b3x-1) & ( yold2 == b3y )) | ( (xold2 == b3x-2) & ( yold2 == b3y )) | ( (xold2 == b3x-3) & ( yold2 == b3y )) | ( (xold2 == b3x-4) & ( yold2 == b3y )) | ( (xold2 == b3x-5) & ( yold2 == b3y )) | ( (xold2 == b3x-6) & ( yold2 == b3y )) | ((xold2+1 == b3x ) & (yold2 == b3y)) | ((xold2+2 == b3x ) & (yold2 == b3y)) | ((xold2+3 == b3x ) & (yold2 == b3y)) | ((xold2+4 == b3x ) & (yold2 == b3y)) | ((xold2+5 == b3x ) & (yold2 == b3y))) {

draw_pacman(b3x,b3y,0,0,10);	
}

else if( (( xold2 == b5x) & (yold2 == b5y)) | ( (xold2 == b5x-1) & ( yold2 == b5y )) | ( (xold2 == b5x-2) & ( yold2 == b5y )) | ( (xold2 == b5x-3) & ( yold2 == b5y )) | ( (xold2 == b5x-4) & ( yold2 == b5y )) | ( (xold2 == b5x-5) & ( yold2 == b5y )) | ( (xold2 == b5x-6) & ( yold2 == b5y )) | ((xold2+1 == b5x ) & (yold2 == b5y)) | ((xold2+2 == b5x ) & (yold2 == b5y)) | ((xold2+3 == b5x ) & (yold2 == b5y)) | ((xold2+4 == b5x ) & (yold2 == b5y)) | ((xold2+5 == b5x ) & (yold2 == b5y))) {

draw_pacman(b5x,b5y,0,0,10);

}

else if( (( xold2 == b7x) & (yold2 == b7y)) | ( (xold2 == b7x-1) & ( yold2 == b7y )) | ( (xold2 == b7x-2) & ( yold2 == b7y )) | ( (xold2 == b7x-3) & ( yold2 == b7y )) | ( (xold2 == b7x-4) & ( yold2 == b7y )) | ( (xold2 == b7x-5) & ( yold2 == b7y )) | ( (xold2 == b7x-6) & ( yold2 == b7y )) | ((xold2+1 == b7x ) & (yold2 == b7y)) | ((xold2+2 == b7x ) & (yold2 == b7y)) | ((xold2+3 == b7x ) & (yold2 == b7y)) | ((xold2+4 == b7x ) & (yold2 == b7y)) | ((xold2+5 == b7x ) & (yold2 == b7y))) {

draw_pacman(b7x,b7y,0,0,10);	
}

else if( (( xold2 == b9x) & (yold2 == b9y)) | ( (xold2 == b9x-1) & ( yold2 == b9y )) | ( (xold2 == b9x-2) & ( yold2 == b9y )) | ( (xold2 == b9x-3) & ( yold2 == b9y )) | ( (xold2 == b9x-4) & ( yold2 == b9y )) | ( (xold2 == b9x-5) & ( yold2 == b9y )) | ( (xold2 == b9x-6) & ( yold2 == b9y )) | ((xold2+1 == b9x ) & (yold2 == b9y)) | ((xold2+2 == b9x ) & (yold2 == b9y)) | ((xold2+3 == b9x ) & (yold2 == b9y)) | ((xold2+4 == b9x ) & (yold2 == b9y)) | ((xold2+5 == b9x ) & (yold2 == b9y))) {

draw_pacman(b9x,b9y,0,0,10);
	
}

else if( (( xold2 == b11x) & (yold2 == b11y)) | ( (xold2 == b11x-1) & ( yold2 == b11y )) | ( (xold2 == b11x-2) & ( yold2 == b11y )) | ( (xold2 == b11x-3) & ( yold2 == b11y )) | ( (xold2 == b11x-4) & ( yold2 == b11y )) | ( (xold2 == b11x-5) & ( yold2 == b11y )) | ( (xold2 == b11x-6) & ( yold2 == b11y )) | ((xold2+1 == b11x ) & (yold2 == b11y)) | ((xold2+2 == b11x ) & (yold2 == b11y)) | ((xold2+3 == b11x ) & (yold2 == b11y)) | ((xold2+4 == b11x ) & (yold2 == b11y)) | ((xold2+5 == b11x ) & (yold2 == b11y))) {

draw_pacman(b11x,b11y,0,0,10);
	
}
else if( (( xold2 == b12x) & (yold2 == b12y)) | ( (xold2 == b12x-1) & ( yold2 == b12y )) | ( (xold2 == b12x-2) & ( yold2 == b12y )) | ( (xold2 == b12x-3) & ( yold2 == b12y )) | ( (xold2 == b12x-4) & ( yold2 == b12y )) | ( (xold2 == b12x-5) & ( yold2 == b12y )) | ( (xold2 == b12x-6) & ( yold2 == b12y )) | ((xold2+1 == b12x ) & (yold2 == b12y)) | ((xold2+2 == b12x ) & (yold2 == b12y)) | ((xold2+3 == b12x ) & (yold2 == b12y)) | ((xold2+4 == b12x ) & (yold2 == b12y)) | ((xold2+5 == b12x ) & (yold2 == b12y))) {

draw_pacman(b12x,b12y,0,0,10);
	
}


else if( (( xold2 == b18x) & (yold2 == b18y)) | ( (xold2 == b18x-1) & ( yold2 == b18y )) | ( (xold2 == b18x-2) & ( yold2 == b18y )) | ( (xold2 == b18x-3) & ( yold2 == b18y )) | ( (xold2 == b18x-4) & ( yold2 == b18y )) | ( (xold2 == b18x-5) & ( yold2 == b18y )) | ( (xold2 == b18x-6) & ( yold2 == b18y )) | ((xold2+1 == b18x ) & (yold2 == b18y)) | ((xold2+2 == b18x ) & (yold2 == b18y)) | ((xold2+3 == b18x ) & (yold2 == b18y)) | ((xold2+4 == b18x ) & (yold2 == b18y)) | ((xold2+5 == b18x ) & (yold2 == b18y))) {

draw_pacman(b18x,b18y,0,0,10);
}

else if( (( xold2 == b20x) & (yold2 == b20y)) | ( (xold2 == b20x-1) & ( yold2 == b20y )) | ( (xold2 == b20x-2) & ( yold2 == b20y )) | ( (xold2 == b20x-3) & ( yold2 == b20y )) | ( (xold2 == b20x-4) & ( yold2 == b20y )) | ( (xold2 == b20x-5) & ( yold2 == b20y )) | ( (xold2 == b20x-6) & ( yold2 == b20y )) | ((xold2+1 == b20x ) & (yold2 == b20y)) | ((xold2+2 == b20x ) & (yold2 == b20y)) | ((xold2+3 == b20x ) & (yold2 == b20y)) | ((xold2+4 == b20x ) & (yold2 == b20y)) | ((xold2+5 == b20x ) & (yold2 == b20y))) {

draw_pacman(b20x,b20y,0,0,10);
	
}

else if( (( xold2 == b23x) & (yold2 == b23y)) | ( (xold2 == b23x-1) & ( yold2 == b23y )) | ( (xold2 == b23x-2) & ( yold2 == b23y )) | ( (xold2 == b23x-3) & ( yold2 == b23y )) | ( (xold2 == b23x-4) & ( yold2 == b23y )) | ( (xold2 == b23x-5) & ( yold2 == b23y )) | ( (xold2 == b23x-6) & ( yold2 == b23y )) | ((xold2+1 == b23x ) & (yold2 == b23y)) | ((xold2+2 == b23x ) & (yold2 == b23y)) | ((xold2+3 == b23x ) & (yold2 == b23y)) | ((xold2+4 == b23x ) & (yold2 == b23y)) | ((xold2+5 == b23x ) & (yold2 == b23y))) {

draw_pacman(b23x,b23y,0,0,10);
}
else if( (( xold2 == b24x) & (yold2 == b24y)) | ( (xold2 == b24x-1) & ( yold2 == b24y )) | ( (xold2 == b24x-2) & ( yold2 == b24y )) | ( (xold2 == b24x-3) & ( yold2 == b24y )) | ( (xold2 == b24x-4) & ( yold2 == b24y )) | ( (xold2 == b24x-5) & ( yold2 == b24y )) | ( (xold2 == b24x-6) & ( yold2 == b24y )) | ((xold2+1 == b24x ) & (yold2 == b24y)) | ((xold2+2 == b24x ) & (yold2 == b24y)) | ((xold2+3 == b24x ) & (yold2 == b24y)) | ((xold2+4 == b24x ) & (yold2 == b24y)) | ((xold2+5 == b24x ) & (yold2 == b24y))) {

	draw_pacman(b24x,b24y,0,0,10);
}

else if( (( xold2 == b28x) & (yold2 == b28y)) | ( (xold2 == b28x-1) & ( yold2 == b28y )) | ( (xold2 == b28x-2) & ( yold2 == b28y )) | ( (xold2 == b28x-3) & ( yold2 == b28y )) | ( (xold2 == b28x-4) & ( yold2 == b28y )) | ( (xold2 == b28x-5) & ( yold2 == b28y )) | ( (xold2 == b28x-6) & ( yold2 == b28y )) | ((xold2+1 == b28x ) & (yold2 == b28y)) | ((xold2+2 == b28x ) & (yold2 == b28y)) | ((xold2+3 == b28x ) & (yold2 == b28y)) | ((xold2+4 == b28x ) & (yold2 == b28y)) | ((xold2+5 == b28x ) & (yold2 == b28y))) {

	draw_pacman(b28x,b28y,0,0,10);
}
else if( (( xold2 == b29x) & (yold2 == b29y)) | ( (xold2 == b29x-1) & ( yold2 == b29y )) | ( (xold2 == b29x-2) & ( yold2 == b29y )) | ( (xold2 == b29x-3) & ( yold2 == b29y )) | ( (xold2 == b29x-4) & ( yold2 == b29y )) | ( (xold2 == b29x-5) & ( yold2 == b29y )) | ( (xold2 == b29x-6) & ( yold2 == b29y )) | ((xold2+1 == b29x ) & (yold2 == b29y)) | ((xold2+2 == b29x ) & (yold2 == b29y)) | ((xold2+3 == b29x ) & (yold2 == b29y)) | ((xold2+4 == b29x ) & (yold2 == b29y)) | ((xold2+5 == b29x ) & (yold2 == b29y))) {

draw_pacman(b29x,b29y,0,0,10);
}
else if( (( xold2 == b30x) & (yold2 == b30y)) | ( (xold2 == b30x-1) & ( yold2 == b30y )) | ( (xold2 == b30x-2) & ( yold2 == b30y )) | ( (xold2 == b30x-3) & ( yold2 == b30y )) | ( (xold2 == b30x-4) & ( yold2 == b30y )) | ( (xold2 == b30x-5) & ( yold2 == b30y )) | ( (xold2 == b30x-6) & ( yold2 == b30y )) | ((xold2+1 == b30x ) & (yold2 == b30y)) | ((xold2+2 == b30x ) & (yold2 == b30y)) | ((xold2+3 == b30x ) & (yold2 == b30y)) | ((xold2+4 == b30x ) & (yold2 == b30y)) | ((xold2+5 == b30x ) & (yold2 == b30y))) {
	draw_pacman(b30x,b30y,0,0,10);
}

else if( (( xold2 == b32x) & (yold2 == b32y)) | ( (xold2 == b32x-1) & ( yold2 == b32y )) | ( (xold2 == b32x-2) & ( yold2 == b32y )) | ( (xold2 == b32x-3) & ( yold2 == b32y )) | ( (xold2 == b32x-4) & ( yold2 == b32y )) | ( (xold2 == b32x-5) & ( yold2 == b32y )) | ( (xold2 == b32x-6) & ( yold2 == b32y )) | ((xold2+1 == b32x ) & (yold2 == b32y)) | ((xold2+2 == b32x ) & (yold2 == b32y)) | ((xold2+3 == b32x ) & (yold2 == b32y)) | ((xold2+4 == b32x ) & (yold2 == b32y)) | ((xold2+5 == b32x ) & (yold2 == b32y))) {
draw_pacman(b32x,b32y,0,0,10);

}

else if( (( xold2 == b34x) & (yold2 == b34y)) | ( (xold2 == b34x-1) & ( yold2 == b34y )) | ( (xold2 == b34x-2) & ( yold2 == b34y )) | ( (xold2 == b34x-3) & ( yold2 == b34y )) | ( (xold2 == b34x-4) & ( yold2 == b34y )) | ( (xold2 == b34x-5) & ( yold2 == b34y )) | ( (xold2 == b34x-6) & ( yold2 == b34y )) | ((xold2+1 == b34x ) & (yold2 == b34y)) | ((xold2+2 == b34x ) & (yold2 == b34y)) | ((xold2+3 == b34x ) & (yold2 == b34y)) | ((xold2+4 == b34x ) & (yold2 == b34y)) | ((xold2+5 == b34x ) & (yold2 == b34y))) {
	draw_pacman(b34x,b34y,0,0,10);
}

						
		}
}

/* PACMAN EATS FOOD AND SCORES POINT*/

if ( ( (x2 == food1x) & ( y2 == food1y)) | (  (x2 == food1x-4)  & ( y2 == food1y) | (  (x2 == food1x-3)  & ( y2 == food1y) | (  (x2 == food1x-2)  & ( y2 == food1y) ) | (  (x2 == food1x-1)  & ( y2 == food1y) )  |  (  (x2-6 == food1x)  & ( y2 == food1y) ) | (  (x2-5 == food1x)  & ( y2 == food1y) ) | (  (x2-4 == food1x)  & ( y2 == food1y) )| ( (x2-3 == food1x)  & ( y2 == food1y) ) | (  (x2-2 == food1x)  & ( y2 == food1y) ) | (  (x2-1 == food1x)  & ( y2 == food1y) )  )  )){

Delay(msTime);
xold2 = x2;
yold2 = y2;
x2 += 7;
draw_pacman(x2,y2,xold2,yold2,5);
draw_pacman(food1x,food1y,0,0,9);

	
	
}

else if ( ( (x2 == food2x) & ( y2 == food2y)) | (  (x2 == food2x-4)  & ( y2 == food2y) | (  (x2 == food2x-3)  & ( y2 == food2y) | (  (x2 == food2x-2)  & ( y2 == food2y) ) | (  (x2 == food2x-1)  & ( y2 == food2y) )  |  (  (x2-6 == food2x)  & ( y2 == food2y) ) | (  (x2-5 == food2x)  & ( y2 == food2y) ) | (  (x2-4 == food2x)  & ( y2 == food2y) )| ( (x2-3 == food2x)  & ( y2 == food2y) ) | (  (x2-2 == food2x)  & ( y2 == food2y) ) | (  (x2-1 == food2x)  & ( y2 == food2y) )  )  )){

Delay(msTime);
xold2 = x2;
	yold2 = y2;
	x2 += 7;
	draw_pacman(x2,y2,xold2,yold2,5);
draw_pacman(food2x,food2y,0,0,9);
	
	
	
}	





if( (( x == b1x) & (y == b1y)) | ( (x == b1x-1) & ( y == b1y )) | ( (x == b1x-2) & ( y == b1y )) | ( (x == b1x-3) & ( y == b1y )) | ( (x == b1x-4) & ( y == b1y )) | ( (x == b1x-5) & ( y == b1y )) | ( (x == b1x-6) & ( y == b1y )) | ((x+1 == b1x ) & (y == b1y)) | ((x+2 == b1x ) & (y == b1y)) | ((x+3 == b1x ) & (y == b1y)) | ((x+4 == b1x ) & (y == b1y)) | ((x+5 == b1x ) & (y == b1y))) {

skor += 1;
eat_dot();
b1x = -1;
b1y = -1;
	
}

 if( (( x == b3x) & (y == b3y)) | ( (x == b3x-1) & ( y == b3y )) | ( (x == b3x-2) & ( y == b3y )) | ( (x == b3x-3) & ( y == b3y )) | ( (x == b3x-4) & ( y == b3y )) | ( (x == b3x-5) & ( y == b3y )) | ( (x == b3x-6) & ( y == b3y )) | ((x+1 == b3x ) & (y == b3y)) | ((x+2 == b3x ) & (y == b3y)) | ((x+3 == b3x ) & (y == b3y)) | ((x+4 == b3x ) & (y == b3y)) | ((x+5 == b3x ) & (y == b3y))) {

skor += 1;
eat_dot();
b3x = -1;
b3y = -1;
	
}

if( (( x == b5x) & (y == b5y)) | ( (x == b5x-1) & ( y == b5y )) | ( (x == b5x-2) & ( y == b5y )) | ( (x == b5x-3) & ( y == b5y )) | ( (x == b5x-4) & ( y == b5y )) | ( (x == b5x-5) & ( y == b5y )) | ( (x == b5x-6) & ( y == b5y )) | ((x+1 == b5x ) & (y == b5y)) | ((x+2 == b5x ) & (y == b5y)) | ((x+3 == b5x ) & (y == b5y)) | ((x+4 == b5x ) & (y == b5y)) | ((x+5 == b5x ) & (y == b5y))) {

skor += 1;
eat_dot();
b5x = -1;
b5y = -1;
	
}

if( (( x == b7x) & (y == b7y)) | ( (x == b7x-1) & ( y == b7y )) | ( (x == b7x-2) & ( y == b7y )) | ( (x == b7x-3) & ( y == b7y )) | ( (x == b7x-4) & ( y == b7y )) | ( (x == b7x-5) & ( y == b7y )) | ( (x == b7x-6) & ( y == b7y )) | ((x+1 == b7x ) & (y == b7y)) | ((x+2 == b7x ) & (y == b7y)) | ((x+3 == b7x ) & (y == b7y)) | ((x+4 == b7x ) & (y == b7y)) | ((x+5 == b7x ) & (y == b7y))) {

skor += 1;
eat_dot();	
b7x = -1;
b7y = -1;
	
}

if( (( x == b9x) & (y == b9y)) | ( (x == b9x-1) & ( y == b9y )) | ( (x == b9x-2) & ( y == b9y )) | ( (x == b9x-3) & ( y == b9y )) | ( (x == b9x-4) & ( y == b9y )) | ( (x == b9x-5) & ( y == b9y )) | ( (x == b9x-6) & ( y == b9y )) | ((x+1 == b9x ) & (y == b9y)) | ((x+2 == b9x ) & (y == b9y)) | ((x+3 == b9x ) & (y == b9y)) | ((x+4 == b9x ) & (y == b9y)) | ((x+5 == b9x ) & (y == b9y))) {

skor += 1;
eat_dot();
b9x = -1;
b9y = -1;
	
}

if( (( x == b11x) & (y == b11y)) | ( (x == b11x-1) & ( y == b11y )) | ( (x == b11x-2) & ( y == b11y )) | ( (x == b11x-3) & ( y == b11y )) | ( (x == b11x-4) & ( y == b11y )) | ( (x == b11x-5) & ( y == b11y )) | ( (x == b11x-6) & ( y == b11y )) | ((x+1 == b11x ) & (y == b11y)) | ((x+2 == b11x ) & (y == b11y)) | ((x+3 == b11x ) & (y == b11y)) | ((x+4 == b11x ) & (y == b11y)) | ((x+5 == b11x ) & (y == b11y))) {

skor += 1;
eat_dot();
b11x = -1;
b11y = -1;
	
}
if( (( x == b12x) & (y == b12y)) | ( (x == b12x-1) & ( y == b12y )) | ( (x == b12x-2) & ( y == b12y )) | ( (x == b12x-3) & ( y == b12y )) | ( (x == b12x-4) & ( y == b12y )) | ( (x == b12x-5) & ( y == b12y )) | ( (x == b12x-6) & ( y == b12y )) | ((x+1 == b12x ) & (y == b12y)) | ((x+2 == b12x ) & (y == b12y)) | ((x+3 == b12x ) & (y == b12y)) | ((x+4 == b12x ) & (y == b12y)) | ((x+5 == b12x ) & (y == b12y))) {

skor += 1;
eat_dot();	
b12x = -1;
b12y = -1;
	
}


if( (( x == b18x) & (y == b18y)) | ( (x == b18x-1) & ( y == b18y )) | ( (x == b18x-2) & ( y == b18y )) | ( (x == b18x-3) & ( y == b18y )) | ( (x == b18x-4) & ( y == b18y )) | ( (x == b18x-5) & ( y == b18y )) | ( (x == b18x-6) & ( y == b18y )) | ((x+1 == b18x ) & (y == b18y)) | ((x+2 == b18x ) & (y == b18y)) | ((x+3 == b18x ) & (y == b18y)) | ((x+4 == b18x ) & (y == b18y)) | ((x+5 == b18x ) & (y == b18y))) {

skor += 1;
eat_dot();	
b18x = -1;
b18y = -1;
	
}

if( (( x == b20x) & (y == b20y)) | ( (x == b20x-1) & ( y == b20y )) | ( (x == b20x-2) & ( y == b20y )) | ( (x == b20x-3) & ( y == b20y )) | ( (x == b20x-4) & ( y == b20y )) | ( (x == b20x-5) & ( y == b20y )) | ( (x == b20x-6) & ( y == b20y )) | ((x+1 == b20x ) & (y == b20y)) | ((x+2 == b20x ) & (y == b20y)) | ((x+3 == b20x ) & (y == b20y)) | ((x+4 == b20x ) & (y == b20y)) | ((x+5 == b20x ) & (y == b20y))) {

skor += 1;
eat_dot();	
b20x = -1;
b20y = -1;
	
}

if( (( x == b23x) & (y == b23y)) | ( (x == b23x-1) & ( y == b23y )) | ( (x == b23x-2) & ( y == b23y )) | ( (x == b23x-3) & ( y == b23y )) | ( (x == b23x-4) & ( y == b23y )) | ( (x == b23x-5) & ( y == b23y )) | ( (x == b23x-6) & ( y == b23y )) | ((x+1 == b23x ) & (y == b23y)) | ((x+2 == b23x ) & (y == b23y)) | ((x+3 == b23x ) & (y == b23y)) | ((x+4 == b23x ) & (y == b23y)) | ((x+5 == b23x ) & (y == b23y))) {

skor += 1;
eat_dot();	
b23x = -1;
b23y = -1;	
}
 if( (( x == b24x) & (y == b24y)) | ( (x == b24x-1) & ( y == b24y )) | ( (x == b24x-2) & ( y == b24y )) | ( (x == b24x-3) & ( y == b24y )) | ( (x == b24x-4) & ( y == b24y )) | ( (x == b24x-5) & ( y == b24y )) | ( (x == b24x-6) & ( y == b24y )) | ((x+1 == b24x ) & (y == b24y)) | ((x+2 == b24x ) & (y == b24y)) | ((x+3 == b24x ) & (y == b24y)) | ((x+4 == b24x ) & (y == b24y)) | ((x+5 == b24x ) & (y == b24y))) {

skor += 1;
eat_dot();	
b24x = -1;
b24y = -1;		
}

if( (( x == b28x) & (y == b28y)) | ( (x == b28x-1) & ( y == b28y )) | ( (x == b28x-2) & ( y == b28y )) | ( (x == b28x-3) & ( y == b28y )) | ( (x == b28x-4) & ( y == b28y )) | ( (x == b28x-5) & ( y == b28y )) | ( (x == b28x-6) & ( y == b28y )) | ((x+1 == b28x ) & (y == b28y)) | ((x+2 == b28x ) & (y == b28y)) | ((x+3 == b28x ) & (y == b28y)) | ((x+4 == b28x ) & (y == b28y)) | ((x+5 == b28x ) & (y == b28y))) {

skor += 1;
eat_dot();	
b28x = -1;
b28y = -1;		
}
if( (( x == b29x) & (y == b29y)) | ( (x == b29x-1) & ( y == b29y )) | ( (x == b29x-2) & ( y == b29y )) | ( (x == b29x-3) & ( y == b29y )) | ( (x == b29x-4) & ( y == b29y )) | ( (x == b29x-5) & ( y == b29y )) | ( (x == b29x-6) & ( y == b29y )) | ((x+1 == b29x ) & (y == b29y)) | ((x+2 == b29x ) & (y == b29y)) | ((x+3 == b29x ) & (y == b29y)) | ((x+4 == b29x ) & (y == b29y)) | ((x+5 == b29x ) & (y == b29y))) {

skor += 1;
eat_dot();	
b29x = -1;
b29y = -1;		
}
 if( (( x == b30x) & (y == b30y)) | ( (x == b30x-1) & ( y == b30y )) | ( (x == b30x-2) & ( y == b30y )) | ( (x == b30x-3) & ( y == b30y )) | ( (x == b30x-4) & ( y == b30y )) | ( (x == b30x-5) & ( y == b30y )) | ( (x == b30x-6) & ( y == b30y )) | ((x+1 == b30x ) & (y == b30y)) | ((x+2 == b30x ) & (y == b30y)) | ((x+3 == b30x ) & (y == b30y)) | ((x+4 == b30x ) & (y == b30y)) | ((x+5 == b30x ) & (y == b30y))) {

skor += 1;
eat_dot();	
b30x = -1;
b30y = -1;		
}

 if( (( x == b32x) & (y == b32y)) | ( (x == b32x-1) & ( y == b32y )) | ( (x == b32x-2) & ( y == b32y )) | ( (x == b32x-3) & ( y == b32y )) | ( (x == b32x-4) & ( y == b32y )) | ( (x == b32x-5) & ( y == b32y )) | ( (x == b32x-6) & ( y == b32y )) | ((x+1 == b32x ) & (y == b32y)) | ((x+2 == b32x ) & (y == b32y)) | ((x+3 == b32x ) & (y == b32y)) | ((x+4 == b32x ) & (y == b32y)) | ((x+5 == b32x ) & (y == b32y))) {

skor += 1;
eat_dot();	
b32x = -1;
b32y = -1;		
}

if( (( x == b34x) & (y == b34y)) | ( (x == b34x-1) & ( y == b34y )) | ( (x == b34x-2) & ( y == b34y )) | ( (x == b34x-3) & ( y == b34y )) | ( (x == b34x-4) & ( y == b34y )) | ( (x == b34x-5) & ( y == b34y )) | ( (x == b34x-6) & ( y == b34y )) | ((x+1 == b34x ) & (y == b34y)) | ((x+2 == b34x ) & (y == b34y)) | ((x+3 == b34x ) & (y == b34y)) | ((x+4 == b34x ) & (y == b34y)) | ((x+5 == b34x ) & (y == b34y))) {

skor += 1;
eat_dot();	
b34x = -1;
b34y = -1;		
}




if (( (x == x2) & ( y == y2)) | (  (x == x2-5)  & ( y == y2) ) | (  (x == x2-4)  & ( y == y2) | (  (x == x2-3)  & ( y == y2) | (  (x == x2-2)  & ( y == y2) ) | (  (x == x2-1)  & ( y == y2) )  |  (  (x-6 == x2)  & ( y == y2) ) | (  (x-5 == x2)  & ( y == y2) ) | (  (x-4 == x2)  & ( y == y2) )| ( (x-3 == x2)  & ( y == y2) ) | (  (x-2 == x2)  & ( y == y2) ) | (  (x-1 == x2)  & ( y == y2) )  )  ) ){

eat_pacman();
game_over_ghost();
}	


if ( ( (x == food1x) & ( y == food1y)) | (  (x == food1x-4)  & ( y == food1y) | (  (x == food1x-3)  & ( y == food1y) | (  (x == food1x-2)  & ( y == food1y) ) | (  (x == food1x-1)  & ( y == food1y) )  |  (  (x-6 == food1x)  & ( y == food1y) ) | (  (x-5 == food1x)  & ( y == food1y) ) | (  (x-4 == food1x)  & ( y == food1y) )| ( (x-3 == food1x)  & ( y == food1y) ) | (  (x-2 == food1x)  & ( y == food1y) ) | (  (x-1 == food1x)  & ( y == food1y) )  )  )){


eat_food();
skor += 5;
	
	
	
}	


else if ( ( (x == food2x) & ( y == food2y)) |  (  (x == food2x-4)  & ( y == food2y) | (  (x == food2x-3)  & ( y == food2y) | (  (x == food2x-2)  & ( y == food2y) ) | (  (x == food2x-1)  & ( y == food2y) )  |  (  (x-6 == food2x)  & ( y == food2y) ) | (  (x-5 == food2x)  & ( y == food2y) ) | (  (x-4 == food2x)  & ( y == food2y) )| ( (x-3 == food2x)  & ( y == food2y) ) | (  (x-2 == food2x)  & ( y == food2y) ) | (  (x-1 == food2x)  & ( y == food2y) )  )  )){


eat_food();
foodamount += 1;
skor += 1;
	
	
	
}	



while( (TPM0->SC & 0x80) == 0){}
	
	if( foodamount > 0 ){
	TPM0->SC |= 0x80; // Clear TOF //
	food1x = 41;
	food1y = 3;
	draw_pacman(food1x,food1y,0,0,9);
//	draw_dot(food1x,food1y);
	
  srand(result2);

	food2x = (rand()%(65));
	food2y = (rand()%(5));
	draw_pacman(food2x,food2y,0,0,9);	
		
	TPM0->SC |= 0x08;
	foodamount -= 1;
	}

/* FINISHING CONDITION FOR PACMAN'S WIN */

if( ( b1x == -1 & b1y == -1)  & ( b3x == -1 & b3y == -1)  & ( b5x == -1 & b5y == -1 ) & ( b7x == -1 & b7y == -1 ) & ( b9x == -1 & b9y == -1) & ( b11x == -1 & b11y == -1 ) & ( b12x == -1 & b12y == -1) & ( b18x == -1 & b18y == -1)  & ( b20x == -1 & b20y == -1 ) & ( b23x == -1 & b23y == -1 ) & ( b24x == -1 & b24y == -1) & ( b28x == -1 & b28y == -1 ) & ( b29x == -1 & b29y == -1 ) & ( b30x == -1 & b30y == -1) & ( b32x == -1 & b32y == -1 )  & ( b34x == -1 & b34y == -1 )){

//GLCD_clear();
game_over_pacman();
}

	
  
}
}
	
/* FUNCTION DEFINITIONS FOR OUR LIBRARY*/
	

void draw_pacman(int x,int y,int xold, int yold,int dir)
{
	GLCD_setCursor(xold,yold);
	GLCD_putchar(1);
	GLCD_setCursor(x,y);
	Delay(msTime);
	GLCD_putchar(dir);
	

		
}
	
void GLCD_writeLetter(int c){
 
    int i;
    for (i = 0; i < 8; i++){
        GLCD_data_write(letters[c][i]);
}	
}


void GLCD_putchar(int c) {
    int i;
    for (i = 0; i < 8; i++){
        GLCD_data_write(game_table[c][i]);
}}

void GLCD_setCursor(unsigned char x, unsigned char y) {
    GLCD_command_write(0x80 | x);  /* column */
    GLCD_command_write(0x40 | y);  /* bank (8 rows per bank) */
}

/* clears the GLCD by writing zeros to the entire screen */
void GLCD_clear(void) {
    int32_t index;
    for (index = 0 ; index < (GLCD_WIDTH * GLCD_HEIGHT / 8) ; index++)
        GLCD_data_write(0x00);
    
    GLCD_setCursor(0, 0); /*After we clear the display, return to the home position */
}





void SPI0_init(void) {
    SIM->SCGC5 |= 0x1000;       /* enable clock to Port D */
    SIM->SCGC5 |= 0x0800;       /* enable clock to Port C */
    PORTD->PCR[1] = 0x200;      /* make PTD1 pin as SPI SCK */
    PORTD->PCR[2] = 0x200;      /* make PTD2 pin as SPI MOSI */
    PORTD->PCR[0] = 0x100;      /* make PTD0 pin as DC */
    PORTC->PCR[8] = 0x100;      /* make PTC8 pin as RST */
    PORTC->PCR[9] = 0x100;      /* make PTC9 pin as CE */
    PTD->PDDR |= 0x01;          /* make PTD0 as output pin for DC */
    PTC->PDDR |= 0x0200;        /* make PTC9 as output pin for /CE */
    PTC->PSOR = CE;             /* deassert /CE */
    PTC->PDDR |= 0x0100;        /* make PTC8 as output pin for RESET */
    PTC->PCOR = RESET;          /* assert reset */
    
    SIM->SCGC4 |= 0x400000;     /* enable clock to SPI0 */
    SPI0->C1 = 0x10;            /* disable SPI and make SPI0 master */
    SPI0->BR = 0x60;            /* set Baud rate to 1 MHz */
    SPI0->C1 |= 0x40;           /* Enable SPI module */
}

/* send the initialization commands to PCD8544 GLCD controller */
void GLCD_init(void) {
    SPI0_init();

    /* hardware reset of GLCD controller */
    PTC->PSOR = RESET;          /* deassert reset */

    GLCD_command_write(0x21);   /* set extended command mode */
    GLCD_command_write(0xBF);   /* set LCD Vop for contrast */
    GLCD_command_write(0x04);   /* set temp coefficient */
    GLCD_command_write(0x14);   /* set LCD bias mode 1:48 */
    GLCD_command_write(0x20);   /* set normal command mode */
    GLCD_command_write(0x0C);   /* set display normal mode */
}




/* write to GLCD controller data register */
void GLCD_data_write(unsigned char data) {
    /* select data register */
    PTD->PSOR = DC;             /* set DC */
    /* send data via SSI */
    SPI0_write(data);
}


/* write to GLCD controller command register */
void GLCD_command_write(unsigned char data) {
    /* select command register */
    PTD->PCOR = DC;             /* clear DC */
    /* send data via SSI */
    SPI0_write(data);
}

void SPI0_write(unsigned char data) {
    volatile char dummy;
    
    PTC->PCOR = CE;                 /* assert /CE */

    while(!(SPI0->S & 0x20)) { }    /* wait until tx ready */
    SPI0->D = data;                 /* send register address */
    while(!(SPI0->S & 0x80)) { }    /* wait until tx complete */
    dummy = SPI0->D;                /* clear SPRF */
    
    PTC->PSOR = CE;                 /* deasssert /CE */
} 




void ADC0_init(void)
{
	SIM->SCGC5 |= 0x2000; /* clock to PORTE */
	PORTE->PCR[20] = 0; /* PTE20 analog input */
	PORTE->PCR[21] = 0;
	PORTE->PCR[23] = 0;
	PORTE->PCR[30] = 0;
	SIM->SCGC6 |= 0x8000000; /* clock to ADC0 */
//	SIM->SOPT2 |= 0x0100000;
//	TPM0->SC = 0;
//	TPM0->MOD = 0xFFFF;
//	TPM0->SC |= 0x80;
//	TPM0->SC |= 0X08;
	ADC0->SC2 &= ~0x40; /* software trigger */
	/* clock div by 4, long sample time, single ended 12 bit, bus clock */
	ADC0->CFG1 = 0x40 | 0x10 | 0x04 | 0x00;
}

void Delay(volatile unsigned int time_del) {
  while (time_del--) 
		{
  }
}

void draw_dot(int x,int y){

GLCD_setCursor(x,y);
GLCD_data_write(0x20);
GLCD_data_write(0x20);	
	
}

void draw_string(int x,int y){

GLCD_setCursor(x,y);
GLCD_data_write(0x0FF);	
}

void draw_middlewall(int x,int y){
GLCD_setCursor(x,y);
GLCD_data_write(0x0F0);
GLCD_data_write(0x0F0);
GLCD_data_write(0x0F0);
GLCD_data_write(0x0F0);
GLCD_data_write(0x0F0);
GLCD_data_write(0x0F0);	
GLCD_data_write(0x0F0);
GLCD_data_write(0x0F0);	
GLCD_data_write(0x0F0);
GLCD_data_write(0x0F0);	
//GLCD_data_write(0x0F0);
//GLCD_data_write(0x0F0);	
}
void draw_middlewall2(int x,int y){
GLCD_setCursor(x,y);
GLCD_data_write(0x0F);
GLCD_data_write(0x0F);
GLCD_data_write(0x0F);
GLCD_data_write(0x0F);
GLCD_data_write(0x0F);
GLCD_data_write(0x0F);	
GLCD_data_write(0x0F);
GLCD_data_write(0x0F);	
GLCD_data_write(0x0F);
GLCD_data_write(0x0F);	
//GLCD_data_write(0x0F);
//GLCD_data_write(0x0F);	
}
void draw_upperstring(void){
GLCD_setCursor(7,0.5);
for(int c = 0; c < 14 ; c++){
GLCD_data_write(0x80);
GLCD_data_write(0x80);
}
}

void draw_upperstring2(void){
GLCD_setCursor(50,0.5);
for(int c = 0; c < 15 ; c++){
GLCD_data_write(0x80);
GLCD_data_write(0x80);
}
}

void draw_lowerstring(void){
GLCD_setCursor(7,5);
for(int c = 0; c < 15 ; c++){
GLCD_data_write(0x30);
GLCD_data_write(0x30);
}
}

void draw_solkenar(void){
GLCD_setCursor(32,2);
GLCD_data_write(0x0F);
GLCD_data_write(0x0F);	

for(int s = 0; s < 4; s++){
GLCD_data_write(0x03);	
}
}

void draw_solaltkenar(void){
GLCD_setCursor(32,4);
GLCD_data_write(0x0F);
GLCD_data_write(0x0F);	

for(int s = 0; s < 4; s++){
GLCD_data_write(0x0C);	
}
}

void draw_sagkenar(void){
GLCD_setCursor(50,2);
for(int s = 0; s < 4; s++){
GLCD_data_write(0x03);	
}	
	
GLCD_data_write(0x0F);
GLCD_data_write(0x0F);	


}

void draw_sagaltkenar(void){
GLCD_setCursor(50,4);
	
for(int s = 0; s < 4; s++){
GLCD_data_write(0x0C);	
}	
	
GLCD_data_write(0x0F);
GLCD_data_write(0x0F);	


}



void eat_pacman(void){

Delay(msTime*2);
GLCD_clear();
	
}


void eat_food(void){

	
	
	
	
}

void eat_dot(void){
	
	
}


void initialize_timer(void){


SIM->SCGC6 |= 0x01000000;
SIM->SOPT2 |= 0x01000000;
TPM0->SC =0;
TPM0->MOD = 0xFFFF;
TPM0->SC |= 0x80;
TPM0->SC |= 0x08;

	
}


void LCD_init(void)
{
    SIM->SCGC5 |= 0x800;       /* enable clock to Port C */ 
    PORTC->PCR[0] = 0x100;      /* make PTD0 pin as GPIO */
    PORTC->PCR[1] = 0x100;      /* make PTD1 pin as GPIO */
    PORTC->PCR[2] = 0x100;      /* make PTD2 pin as GPIO */
    PORTC->PCR[3] = 0x100;      /* make PTD3 pin as GPIO */
    PORTC->PCR[4] = 0x100;      /* make PTD4 pin as GPIO */
    PORTC->PCR[5] = 0x100;      /* make PTD5 pin as GPIO */
    PORTC->PCR[6] = 0x100;      /* make PTD6 pin as GPIO */
    PORTC->PCR[7] = 0x100;      /* make PTD7 pin as GPIO */
    PTC->PDDR = 0xFF;           /* make PTD7-0 as output pins */
    
    SIM->SCGC5 |= 0x0200;       /* enable clock to Port A */ 
    PORTA->PCR[2] = 0x100;      /* make PTA2 pin as GPIO */
    PORTA->PCR[4] = 0x100;      /* make PTA4 pin as GPIO */
    PORTA->PCR[5] = 0x100;      /* make PTA5 pin as GPIO */
    PTA->PDDR |= 0x34;          /* make PTA5, 4, 2 as output pins */
    
    LCD_command(0x38);      /* set 8-bit data, 2-line, 5x7 font */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}



void LCD_ready(void)
{
    uint32_t status;
    
    PTC->PDDR = 0x00;          /* PortD input */
    PTA->PCOR = RS;         /* RS = 0 for status */
    PTA->PSOR = RW;         /* R/W = 1, LCD output */
    
    do {    /* stay in the loop until it is not busy */
			  PTA->PCOR = EN;
			  Delay(500);
        PTA->PSOR = EN;     /* raise E */
        Delay(500);
        status = PTC->PDIR; /* read status register */
        PTA->PCOR = EN;
        Delay(500);			/* clear E */
    } while (status & 0x80UL);    /* check busy bit */
    
    PTA->PCOR = RW;         /* R/W = 0, LCD input */
    PTC->PDDR = 0xFF;       /* PortD output */
}

void LCD_command(unsigned char command)
{
    LCD_ready();			/* wait until LCD is ready */
    PTA->PCOR = RS | RW;    /* RS = 0, R/W = 0 */
    PTC->PDOR = command;
    PTA->PSOR = EN;         /* pulse E */
    Delay(500);
    PTA->PCOR = EN;
}

void LCD_data(unsigned char data)
{
    LCD_ready();			/* wait until LCD is ready */
    PTA->PSOR = RS;         /* RS = 1, R/W = 0 */
    PTA->PCOR = RW;
    PTC->PDOR = data;
    PTA->PSOR = EN;         /* pulse E */
    Delay(500);
    PTA->PCOR = EN;
}


void game_over_pacman(void){
	
GLCD_clear();
GLCD_setCursor(25,0);

GLCD_writeLetter(3);
GLCD_setCursor(35,0);
GLCD_writeLetter(0);
GLCD_setCursor(45,0);
GLCD_writeLetter(6);
GLCD_setCursor(55,0);
GLCD_writeLetter(2);

GLCD_setCursor (25,1);
GLCD_writeLetter(8);
GLCD_setCursor (35,1);
GLCD_writeLetter(13);
GLCD_setCursor (45,1);
GLCD_writeLetter(2);
GLCD_setCursor (55,1);
GLCD_writeLetter(10);


GLCD_setCursor(15,3);
GLCD_writeLetter(9);
GLCD_setCursor(25,3);
GLCD_writeLetter(0);
GLCD_setCursor(35,3);
GLCD_writeLetter(1);
GLCD_setCursor(45,3);
GLCD_writeLetter(6);
GLCD_setCursor(55,3);
GLCD_writeLetter(0);
GLCD_setCursor(65,3);
GLCD_writeLetter(7);

GLCD_setCursor(30,4);
GLCD_writeLetter(14);
GLCD_setCursor(40,4);
GLCD_writeLetter(8);
GLCD_setCursor(50,4);
GLCD_writeLetter(7);
}

void game_over_ghost(void){
	
GLCD_clear();
GLCD_setCursor(25,0);

GLCD_writeLetter(3);
GLCD_setCursor(35,0);
GLCD_writeLetter(0);
GLCD_setCursor(45,0);
GLCD_writeLetter(6);
GLCD_setCursor(55,0);
GLCD_writeLetter(2);

GLCD_setCursor (25,1);
GLCD_writeLetter(8);
GLCD_setCursor (35,1);
GLCD_writeLetter(13);
GLCD_setCursor (45,1);
GLCD_writeLetter(2);
GLCD_setCursor (55,1);
GLCD_writeLetter(10);


GLCD_setCursor(20,3);
GLCD_writeLetter(3);
GLCD_setCursor(30,3);
GLCD_writeLetter(4);
GLCD_setCursor(40,3);
GLCD_writeLetter(8);
GLCD_setCursor(50,3);
GLCD_writeLetter(11);
GLCD_setCursor(60,3);
GLCD_writeLetter(12);


GLCD_setCursor(30,4);
GLCD_writeLetter(14);
GLCD_setCursor(40,4);
GLCD_writeLetter(8);
GLCD_setCursor(50,4);
GLCD_writeLetter(7);
}
// Lab7Main.c
// Runs on LM4F120/TM4C123
// Test the functions in ST7735.c by printing basic
// patterns to the LCD.
//    16-bit color, 128 wide by 160 high LCD
// Daniel Valvano
// Last Modified: 1/17/2020
// Ramesh Yerraballi modified 3/20/2017

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2019

 Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// hardware connections
// **********ST7735 TFT and SDC*******************
// ST7735
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

// **********wide.hk ST7735R with ADXL345 accelerometer *******************
// Silkscreen Label (SDC side up; LCD side down) - Connection
// VCC  - +3.3 V
// GND  - Ground
// !SCL - PA2 Sclk SPI clock from microcontroller to TFT or SDC
// !SDA - PA5 MOSI SPI data from microcontroller to TFT or SDC
// DC   - PA6 TFT data/command
// RES  - PA7 TFT reset
// CS   - PA3 TFT_CS, active low to enable TFT
// *CS  - (NC) SDC_CS, active low to enable SDC
// MISO - (NC) MISO SPI data from SDC to microcontroller
// SDA  – (NC) I2C data for ADXL345 accelerometer
// SCL  – (NC) I2C clock for ADXL345 accelerometer
// SDO  – (NC) I2C alternate address for ADXL345 accelerometer
// Backlight + - Light, backlight connected to +3.3 V

// **********wide.hk ST7735R with ADXL335 accelerometer *******************
// Silkscreen Label (SDC side up; LCD side down) - Connection
// VCC  - +3.3 V
// GND  - Ground
// !SCL - PA2 Sclk SPI clock from microcontroller to TFT or SDC
// !SDA - PA5 MOSI SPI data from microcontroller to TFT or SDC
// DC   - PA6 TFT data/command
// RES  - PA7 TFT reset
// CS   - PA3 TFT_CS, active low to enable TFT
// *CS  - (NC) SDC_CS, active low to enable SDC
// MISO - (NC) MISO SPI data from SDC to microcontroller
// X– (NC) analog input X-axis from ADXL335 accelerometer
// Y– (NC) analog input Y-axis from ADXL335 accelerometer
// Z– (NC) analog input Z-axis from ADXL335 accelerometer
// Backlight + - Light, backlight connected to +3.3 V

// **********HiLetgo ST7735 TFT and SDC (SDC not tested)*******************
// ST7735
// LED-   (pin 16) TFT, connected to ground
// LED+   (pin 15) TFT, connected to +3.3 V
// SD_CS  (pin 14) SDC, chip select
// MOSI   (pin 13) SDC, MOSI
// MISO   (pin 12) SDC, MISO
// SCK    (pin 11) SDC, serial clock
// CS     (pin 10) TFT, PA3 (SSI0Fss)
// SCL    (pin 9)  TFT, SCK  PA2 (SSI0Clk)
// SDA    (pin 8)  TFT, MOSI PA5 (SSI0Tx)
// A0     (pin 7)  TFT, Data/Command PA6 (GPIO), high for data, low for command
// RESET  (pin 6)  TFT, to PA7 (GPIO)
// NC     (pins 3,4,5) not connected
// VCC    (pin 2)  connected to +3.3 V
// GND    (pin 1)  connected to ground


#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "PLL.h"
#include "../inc/tm4c123gh6pm.h"
#include "IO.h"
#include "Print.h"
#include "images.h"

#define SIZE 16

//*****COLORS******
/* some RGB color definitions                                                 */
#define Black           0x0000      /*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255,   0 */
#define White           0xFFFF      /* 255, 255, 255 */
#define Orange          0xFD20      /* 255, 165,   0 */
#define GreenYellow     0xAFE5      /* 173, 255,  47 */
#define Pink            0xF81F
//***********************************************************************



uint32_t const TestData[SIZE] ={
  0,7,9,10,99,100,409,654,999,1000,9999,10000,20806,65535,
  123400009,0xFFFFFFFF
};
	



//*****************************************************************************
//*****Testting*************

// test image2
// [red]   [yellow]  [green]   [blue]	[black]
// [red]   [yellow]  [green]   [blue]	[white]
// [red]   [yellow]  [green]   [blue]	[black]
// [red]   [yellow]  [green]   [blue]	[white]
// [red]   [yellow]  [green]   [blue]	[black]
// [black] [75%grey] [50%grey] [25%grey] [white]
const uint16_t Extra_Credit[] = {
	
	Black,	Black,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Black,	Black,
	Black,	Black,	Black,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Black,	Black,	Black,
	Black,	Black,	Black,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Black,	Black,	Black,	Black,
	Black,	Black,	Black,	Black,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Black,	Black,	Black,	Black,
	Black,	Black,	Black,	Black,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Yellow,	Black,	Black,	Black,	Black,
	Black,	Black,	Black,	Black,	Blue,		White,	White,	White,	White,	Blue,		Black,	Black,	Black,	Black,
	Black,	Black,	Black,	Black,	Blue,		White,	White,	White,	White,	Blue,		Black,	Black,	Black,	Black,
	Black,	Black,	Black,	Black,	Blue,		White,	White,	White,	White,	Blue,		Black,	Black,	Black,	Black,
	Green,	Green,	Blue,		Blue,		Blue,		White,	White,	White,	White,	Blue,		Blue,		Blue,		Green,	Green,
	Green,	Green,	Blue,		Blue,		Blue,		White,	White,	White,	White,	Blue,		Blue,		Blue,		Green,	Green,
	Green,	Green,	Black,	Black,	Blue,		White,	White,	White,	White,	Blue,		Black,	Black,	Green,	Green,
	Green,	Green,	Black,	Black,	Blue,		White,	White,	White,	White,	Blue,		Black,	Black,	Green,	Green,
	Green,	Green,	Black,	Black,	Blue,		White,	White,	White,	White,	Blue,		Black,	Black,	Green,	Green,
	Green,	Green,	Black,	Black,	Blue,		White,	White,	White,	White,	Blue,		Black,	Black,	Green,	Green,
	Green,	Green,	Black,	Black,	Blue,		Black,	Black,	Black,	Black,	Blue,		Black,	Black,	Green,	Green,
	Green,	Green,	Black,	Black,	Blue,		Black,	Black,	Black,	Blue,		Black,	Black,	Black,	Green,	Green,
	Green,	Green,	Black,	Black,	Black,	Blue,		Black,	Black,	Blue,		Black,	Black,	Black,	Green,	Green,
	Green,	Green,	Black,	Black,	Black,	Blue,		Black,	Blue,		Black,	Black,	Black,	Black,	Green,	Green,
	Green,	Green,	Black,	Black,	Black,	Blue,		Black,	Blue,		Black,	Black,	Black,	Black,	Green,	Green,
	Green,	Green,	Black,	Black,	Black,	Black,	Blue,		Black,	Black,	Black,	Black,	Black,	Green,	Green
};

const uint16_t imageMike[] = {
0xFFFF, 0XFFFF, 0x07E0, 0x07E0, 0x07E0, 0XFFFF, 0XFFFF,
0XFFFF, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0XFFFF,
0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0,
0x07E0, 0x07E0, 0XF800, 0xF800, 0XF800, 0x07E0, 0x07E0,
0x07E0, 0XF800, 0x07E0, 0x07E0, 0x07E0, 0xF800, 0x07E0,
0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0,
0x07E0, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0xFFFF, 0x07E0,
0x07E0, 0XFFFF, 0X001F, 0X001F, 0X001F, 0XFFFF, 0x07E0,
0x07E0, 0XFFFF, 0X001F, 0X0000, 0X001F, 0XFFFF, 0x07E0,
0x07E0, 0XFFFF, 0X001F, 0X001F, 0X001F, 0XFFFF, 0x07E0,	
0x07E0, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0x07E0,
0XFFFF, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0XFFFF,
0xFFFF, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0xFFFF,
};

//***********************************************
//*****************************************************************************

int main(void){  
  uint32_t i;
  PLL_Init(Bus80MHz);    // set system clock to 80 MHz
  IO_Init();
  
  // test DrawChar() and DrawCharS()
  ST7735_InitR(INITR_REDTAB);
  ST7735_OutString("Lab 7 Fall 2019\nWelcome to EE319K");
  IO_Touch();
	
	ST7735_OutString("\nEXTRA CREDIT");
	ST7735_DrawBitmap(64,80,Extra_Credit,14,20);
	IO_Touch();

  ST7735_FillScreen(0xFFFF);   // set screen to white
  ST7735_DrawBitmap(44, 159, Logo, 40, 160);
  IO_Touch();
  ST7735_FillScreen(0);       // set screen to black
  for(i=0;i<SIZE;i++){
    IO_HeartBeat();
    ST7735_SetCursor(0,i);
    LCD_OutDec(TestData[i]);
    ST7735_SetCursor(11,i);
    LCD_OutFix(TestData[i]);
//    IO_Touch(); // remove this line to see all test cases
  }
  while(1){
  }
}


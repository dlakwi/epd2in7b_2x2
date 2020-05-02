// epd2in7b_2x2.ino

// The EPD has 2 coloured layers, one black and one red.
// This sketch uses a 2x2 pixel where each sub-pixel can be black, red, or white.
// So, there are many ways to create a 'pixel' with intermediate colours.
// Here, 15 colours are defined using a Bayer half-tone scheme.
// The native 264x176 3-colour display becomes a 132x88 15-colour display.
// 
// If 15-colour images were to be displayed, a clever means of processing the original images
// and some code to encode the image as a loadable pixmap.
// 
// Donald Johnson
// 2020-05-02

/**
 *   based on   : 
 *  @filename   :   epd2in7b_demo.ino
 *  @brief      :   2.7inch e-paper display (B) demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     July 17 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// SPI pins SCLK, MOSI, and SS are used
// other pin assignments are in epdif.h

// the display module has 4 pushbuttons switches - a button press shorts the pin to GND
// these are available only on the RPi connector:
//   1  P5  / 21
//   2  P6  / 22
//   3  P13 / 23
//   4  P19 / 24

#define _ATMEGA_1284P 1  // for pin assignments in epdif.h

#include <SPI.h>
#include "epd2in7b.h"
#include "epdpaint.h"
#include "colours.h"
#include <neotimer.h>

// each 'pixel' is actually a 2x2 pixel, all x, y, width, and height coordinates are half of physical pixels

#define BUF_WIDTH     88  // EPD_WIDTH/2
#define BUF_HEIGHT   132  // EPD_HEIGHT/2

// Each frame buffer is 5808 bytes in size, so a uC with at least 12K RAM must be used.
// Note that EPD_WIDTH*EPD_HEIGHT = 176*264 = 46464 which is > MAXINT (signed)

#define BUF_SIZE    5808  // ((BUF_WIDTH*2 * BUF_HEIGHT*2) / 8)

unsigned char image_black[BUF_SIZE];
unsigned char image_red  [BUF_SIZE];

const int block_width  = 36;
const int block_height = 36;

// x, y, colour
int block[6][3] =
{
  {  5,  6,  0 }, { 47,  6,  0 },
  {  5, 48,  0 }, { 47, 48,  0 },
  {  5, 90,  0 }, { 47, 90,  0 }
};

bool refresh = false;  // not currently used - a button could be used to force a refresh
const unsigned long display_time    = 2UL * 60UL * 1000UL;  //  2 minutes
const unsigned long epd_redraw_time =       20UL * 1000UL;  // 20 seconds

Epd epd;  // the epd, interface, and paint methods

Paint paint( image_black, image_red, BUF_WIDTH, BUF_HEIGHT );  // width should be the multiple of 8

Neotimer timer = Neotimer( epd_redraw_time );

void setup( void )
{
  Serial.begin( 9600 );

  if ( epd.Init() != 0 )
  {
    Serial.print( "e-Paper init failed" );
    return;
  }

  Serial.println( "Bayer Blocks 2020-05-01" );

  epd.ClearFrame();  // clear the SRAM of the e-paper display

  paint.Clear( EPD2_white );

  // display the title in landscape mode with the display pushbuttons on the left
  paint.SetRotate( ROTATE_270 );
  paint.DrawStringAt( 4,  4, "Bayer Blocks", &Font12, EPD2_black );
  paint.DrawStringAt( 4, 24, "2020-05-01",   &Font8,  EPD2_red );

  epd.DisplayFrame( image_black, image_red );

  epd.WaitUntilIdle();

  paint.SetRotate( ROTATE_0 );

  epd.Sleep();

  delay( 4000 );  // display title

  timer.start();
}

void loop( void )
{
  if ( timer.waiting() )
  {
    // complete redraw for the b/r/w display is about 20 seconds
    // so, we cannot refresh sooner than that
    if ( refresh )
    {
      if ( timer.remaining() > epd_redraw_time )
      {
        timer.stop();
        timer.set( epd_redraw_time );
        timer.start();
      }
    }
  }

  if ( timer.done() )
  {
    Serial.println( "next" );

    paint.Clear( EPD2_white );
    for ( int i = 0; i < 6; i++ )
    {
      block[i][2] = colours[ random( 15 ) ];
      paint.DrawFilledRectangle2( block[i][0], block[i][1], block_width, block_height, block[i][2] );
    }

    epd.Init();  // wake up the display

    epd.DisplayFrame( image_black, image_red );

    epd.WaitUntilIdle();

    epd.Sleep();

    timer.set( display_time );
    timer.start();
  }
}

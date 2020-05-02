/**
 *  @filename   :   epdpaint.cpp
 *  @brief      :   Paint tools
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 9 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
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

#include <avr/pgmspace.h>
#include "epdpaint.h"

Paint::Paint( unsigned char* image_black, unsigned char* image_red, int width2, int height2 )
{
  this->rotate      = ROTATE_0;
  this->image_black = image_black;
  this->image_red   = image_red;
  // 1 byte = 8 pixels, so the width2 should be the multiple of 8
  this->width2      = width2 % 8 ? width2 + 8 - ( width2 % 8 ) : width2;
  this->height2     = height2;
  this->width       = this->width2 * 2;
  this->height      = this->height2 * 2;
}

Paint::~Paint()
{
}

// Getters and Setters

unsigned char* Paint::GetImage_black( void )
{
  return this->image_black;
}

unsigned char* Paint::GetImage_red( void )
{
  return this->image_red;
}

int Paint::GetWidth( void )
{
  return this->width2;
}

void Paint::SetWidth( int width2 )
{
  this->width2 = ( width2 % 8 ) ? ( width2 + 8 - ( width2 % 8 ) ) : width2;
  this->width = this->width2 * 2;
}

int Paint::GetHeight( void )
{
  return this->height2;
}

void Paint::SetHeight( int height2 )
{
  this->height2 = height2;
  this->height = this->height2 * 2;
}

int Paint::GetRotate( void )
{
  return this->rotate;
}

void Paint::SetRotate( int rotate )
{
  this->rotate = rotate;
}

// addresses a must be longs since EPD_WIDTH * EPD_HEIGHT > MAXINT

void Paint::blackPixel( byte c, unsigned int a, byte p )
{
  if ( c )
    image_black[ a ] |=  p;
  else
    image_black[ a ] &= ~p;
}

void Paint::redPixel( byte c, unsigned int a, byte p )
{
  if ( c )
    image_red[ a ] |=  p;
  else
    image_red[ a ] &= ~p;
}

// this draws a pixel by absolute coordinates.
// this function won't be affected by the rotate parameter

void Paint::DrawAbsolutePixel( int x, int y, byte colour )
{
  if ( x < 0 || x >= this->width2 || y < 0 || y >= this->height2 )
  {
    return;
  }

  unsigned int x0, x1, x2, x3;
  unsigned int y0, y1, y2, y3;
  unsigned int a0, a1, a2, a3;

  x0 = 2 * x;  x1 = x0 + 1;
  x2 = x0;     x3 = x1;
  y0 = 2 * y;  y1 = y0;
  y2 = y0 + 1; y3 = y2;

  a0 = ( x0 + y0 * this->width ) >> 3;
  a1 = ( x1 + y1 * this->width ) >> 3;
  a2 = ( x2 + y2 * this->width ) >> 3;
  a3 = ( x3 + y3 * this->width ) >> 3;

  blackPixel( ( colour & 0b01000000 ), a0, ( 0x80 >> ( x0 & 7 ) ) );
  blackPixel( ( colour & 0b00010000 ), a1, ( 0x80 >> ( x1 & 7 ) ) );
  blackPixel( ( colour & 0b00000100 ), a2, ( 0x80 >> ( x2 & 7 ) ) );
  blackPixel( ( colour & 0b00000001 ), a3, ( 0x80 >> ( x3 & 7 ) ) );

  redPixel(   ( colour & 0b10000000 ), a0, ( 0x80 >> ( x0 & 7 ) ) );
  redPixel(   ( colour & 0b00100000 ), a1, ( 0x80 >> ( x1 & 7 ) ) );
  redPixel(   ( colour & 0b00001000 ), a2, ( 0x80 >> ( x2 & 7 ) ) );
  redPixel(   ( colour & 0b00000010 ), a3, ( 0x80 >> ( x3 & 7 ) ) );
}

// clear the buffers

void Paint::Clear( byte colour )
{
  for ( int y = 0; y < this->height2; y++ )
  {
    for ( int x = 0; x < this->width2; x++ )
    {
      DrawAbsolutePixel( x, y, colour );
    }
  }
}

// this draws a pixel with potentially rotated coordinates
// rotations are clockwise

void Paint::DrawPixel( int x, int y, byte colour )
{
  int point_temp;
  if ( this->rotate == ROTATE_0 )
  {
  }
  else if ( this->rotate == ROTATE_90 )
  {
    point_temp = x; x = this->width2 - y; y = point_temp;
  }
  else if ( this->rotate == ROTATE_180 )
  {
    x = this->width2 - x; y = this->height2 - y;
  }
  else if ( this->rotate == ROTATE_270 )
  {
    point_temp = x; x = y; y = this->height2 - point_temp;
  }

  if ( x < 0 || x >= this->width2 || y < 0 || y >= this->height2 ) { return; }
  DrawAbsolutePixel( x, y, colour );
}

// this draws a charactor on the frame buffer but not refresh

void Paint::DrawCharAt( int x, int y, char ascii_char, sFONT* font, byte colour )
{
                 int   i, j;
        unsigned int   char_offset = ( ascii_char - ' ' ) * font->Height * ( font->Width / 8 + ( font->Width % 8 ? 1 : 0 ) );
  const unsigned char* ptr         = &font->table[char_offset];

  for ( j = 0; j < font->Height; j++ )
  {
    for ( i = 0; i < font->Width; i++ )
    {
      if ( pgm_read_byte( ptr ) & ( 0x80 >> ( i % 8 ) ) )
      {
        DrawPixel( x + i, y + j, colour );
      }
      if ( i % 8 == 7 )
      {
        ptr++;
      }
    }
    if ( font->Width % 8 != 0 )
    {
      ptr++;
    }
  }
}

// this displays a string on the frame buffer but not refresh

void Paint::DrawStringAt( int x, int y, const char* text, sFONT* font, byte colour )
{
  const char*        p_text    = text;
        unsigned int counter   = 0;
                 int refcolumn = x;

  // Send the string character by character on EPD
  while ( *p_text != 0 )
  {
    DrawCharAt( refcolumn, y, *p_text, font, colour );  // Display one character on EPD
    refcolumn += font->Width;                           // Decrement the column position by 16
    p_text++;                                           // Point on the next character
    counter++;
  }
}

// this draws a line on the frame bufferv

void Paint::DrawLine( int x0, int y0, int x1, int y1, byte colour )
{
  // Bresenham algorithm
  int dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
  int sx = x0 < x1 ? 1 : -1;
  int dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
  int sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;
  while ( ( x0 != x1 ) && ( y0 != y1 ) )
  {
    DrawPixel( x0, y0, colour );
    if ( 2 * err >= dy )
    {
      err += dy;
      x0 += sx;
    }
    if ( 2 * err <= dx )
    {
      err += dx;
      y0 += sy;
    }
  }
}

// this draws a horizontal line on the frame buffer

void Paint::DrawHorizontalLine( int x, int y, int line_width, byte colour )
{
  int i;
  for ( i = x; i < x + line_width; i++ )
  {
    DrawPixel( i, y, colour );
  }
}

// this draws a vertical line on the frame buffer

void Paint::DrawVerticalLine( int x, int y, int line_height, byte colour )
{
  int i;
  for ( i = y; i < y + line_height; i++ )
  {
    DrawPixel( x, i, colour );
  }
}

// this draws a rectangle

void Paint::DrawRectangle( int x0, int y0, int x1, int y1, byte colour )
{
  int min_x, min_y, max_x, max_y;
  min_x = x1 > x0 ? x0 : x1;
  max_x = x1 > x0 ? x1 : x0;
  min_y = y1 > y0 ? y0 : y1;
  max_y = y1 > y0 ? y1 : y0;
  DrawHorizontalLine( min_x, min_y, max_x - min_x + 1, colour );
  DrawHorizontalLine( min_x, max_y, max_x - min_x + 1, colour );
  DrawVerticalLine  ( min_x, min_y, max_y - min_y + 1, colour );
  DrawVerticalLine  ( max_x, min_y, max_y - min_y + 1, colour );
}

// this draws a filled rectangle

void Paint::DrawFilledRectangle( int x0, int y0, int x1, int y1, byte colour )
{
  int min_x, min_y, max_x, max_y;
  int i;
  min_x = x1 > x0 ? x0 : x1;
  max_x = x1 > x0 ? x1 : x0;
  min_y = y1 > y0 ? y0 : y1;
  max_y = y1 > y0 ? y1 : y0;
  for ( i = min_x; i <= max_x; i++ )
  {
    DrawVerticalLine( i, min_y, max_y - min_y + 1, colour );
  }
}

// this draws a filled rectangle, specifying the upper-right corner and the width and height

void Paint::DrawFilledRectangle2( int x, int y, int w, int h, byte colour )
{
  if ( w < 1 || h < 1 ) return;
  for ( int i = x; i < x+w; i++ )
  {
    DrawVerticalLine( i, y, h, colour );
  }
}

// this draws a circle

void Paint::DrawCircle( int x, int y, int radius, byte colour )
{
  // Bresenham algorithm
  int x_pos = -radius;
  int y_pos = 0;
  int err = 2 - 2 * radius;
  int e2;
  do
  {
    DrawPixel( x - x_pos, y + y_pos, colour );
    DrawPixel( x + x_pos, y + y_pos, colour );
    DrawPixel( x + x_pos, y - y_pos, colour );
    DrawPixel( x - x_pos, y - y_pos, colour );
    e2 = err;
    if ( e2 <= y_pos )
    {
      err += ++y_pos * 2 + 1;
      if ( -x_pos == y_pos && e2 <= x_pos )
      {
        e2 = 0;
      }
    }
    if ( e2 > x_pos )
    {
      err += ++x_pos * 2 + 1;
    }
  } while ( x_pos <= 0 );
}

// this draws a filled circle

void Paint::DrawFilledCircle( int x, int y, int radius, byte colour )
{
  // Bresenham algorithm
  int x_pos = -radius;
  int y_pos = 0;
  int err = 2 - 2 * radius;
  int e2;
  do
  {
    DrawPixel( x - x_pos, y + y_pos, colour );
    DrawPixel( x + x_pos, y + y_pos, colour );
    DrawPixel( x + x_pos, y - y_pos, colour );
    DrawPixel( x - x_pos, y - y_pos, colour );
    DrawHorizontalLine( x + x_pos, y + y_pos, 2 * ( -x_pos ) + 1, colour );
    DrawHorizontalLine( x + x_pos, y - y_pos, 2 * ( -x_pos ) + 1, colour );
    e2 = err;
    if ( e2 <= y_pos )
    {
      err += ++y_pos * 2 + 1;
      if ( -x_pos == y_pos && e2 <= x_pos )
      {
        e2 = 0;
      }
    }
    if ( e2 > x_pos )
    {
      err += ++x_pos * 2 + 1;
    }
  } while ( x_pos <= 0 );
}

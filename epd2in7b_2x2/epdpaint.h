/**
 *  @filename   :   epdpaint.h
 *  @brief      :   Header file for epdpaint.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     July 28 2017
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

#ifndef EPDPAINT_H
#define EPDPAINT_H

// Display orientation
#define ROTATE_0            0
#define ROTATE_90           1
#define ROTATE_180          2
#define ROTATE_270          3

// Color inverse. 1 or 0 = set or reset a bit if set a colored pixel
#define IF_INVERT_COLOR     1

#include "fonts.h"

typedef uint8_t byte;

class Paint
{
public:
  Paint( unsigned char* image_black, unsigned char* image_red, int width, int height );
 ~Paint();

  void Clear     ( byte colour );
  int  GetWidth  ( void );
  void SetWidth  ( int width );
  int  GetHeight ( void );
  void SetHeight ( int height );
  int  GetRotate ( void );
  void SetRotate ( int rotate );

  unsigned char* GetImage_black( void );
  unsigned char* GetImage_red( void );

  void blackPixel          ( byte c, unsigned int a, byte p );
  void redPixel            ( byte c, unsigned int a, byte p );
  void DrawAbsolutePixel   ( int x,  int y,                                                 byte colour );
  void DrawPixel           ( int x,  int y,                                                 byte colour );
  void DrawCharAt          ( int x,  int y,                  char ascii_char,  sFONT* font, byte colour );
  void DrawStringAt        ( int x,  int y,                  const char* text, sFONT* font, byte colour );
  void DrawLine            ( int x0, int y0, int x1, int y1,                                byte colour );
  void DrawHorizontalLine  ( int x,  int y,                  int width,                     byte colour );
  void DrawVerticalLine    ( int x,  int y,                  int height,                    byte colour );
  void DrawRectangle       ( int x0, int y0, int x1, int y1,                                byte colour );
  void DrawFilledRectangle ( int x0, int y0, int x1, int y1,                                byte colour );
  void DrawFilledRectangle2( int x,  int y,  int w,  int h,                                 byte colour );
  void DrawCircle          ( int x,  int y,                  int radius,                    byte colour );
  void DrawFilledCircle    ( int x,  int y,                  int radius,                    byte colour );

private:
  unsigned char* image_black;
  unsigned char* image_red;
  int            width;
  int            height;
  int            width2;   // width  / 2
  int            height2;  // height / 2
  int            rotate;
};

#endif

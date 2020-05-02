// colours.h

// 15 'colour' r/b/w display
// 132 x 88  2x2 pixels
// 
// index = b + r << 4
// 
//  0 1
//  2 3
//                                                                                     0 1 2 3
// colour   r  b  w    red   black    index      h   d        pixels          0123  0bBBBBBBBB  0xXX
// -------  -  -  -   -----  -----   --------   --  --   ----------------     ----  ----------  ----
// white    0  0  4     0      0     00000000   00   0               0123     0000  0b00000000  0x00
// black    0  4  0     0      4     00000100   04   4         0123           1111  0b01010101  0x55
// red      4  0  0     4      0     01000000   40  64   0123                 2222  0b10101010  0xAA
//                                                       .     .     .                              
// gray1    0  1  3     0      1     00000001   01   1         0      123     1000  0b01000000  0x40
// gray2    0  2  2     0      2     00000010   02   2         0  3   12      1001  0b01000001  0x41
// gray3    0  3  1     0      3     00000011   03   3         012      3     1110  0b01010100  0x54
//                                                       .     .     .                              
// pink1    1  0  3     1      0     00010000   10  16   0            123     2000  0b10000000  0x80
// pink2    2  0  2     2      0     00100000   20  32   0  3         12      2002  0b10000010  0x82
// pink3    3  0  1     3      0     00110000   30  48   012            3     2220  0b10101000  0xA8
//                                                       .     .     .                              
// red1     1  3  0     1      3     00010011   13  19   0      123           2111  0b10010101  0x95
// red2     2  2  0     2      2     00100010   22  34   0  3   12            2112  0b10010110  0x96
// red3     3  1  0     3      1     00110001   31  49   012      3           2221  0b10101001  0xA9
//                                                       .     .     .                              
// rust1    1  1  2     1      1     00010001   11  17   0        3   12      2001  0b10000001  0x81
// rust2    1  2  1     1      2     00010010   12  18   0      12      3     2110  0b10010100  0x94
// rust3    2  1  1     2      1     00100001   21  33    12   0        3     1220  0b01101000  0x68
//                                                       .     .     .                              

// white  0x00
// gray1  0x40
// gray2  0x41
// gray3  0x54
// black  0x55
// 
// white  0x00
// pink1  0x80
// pink2  0x82
// pink3  0xA8
// red    0xAA
// 
// black  0x55
// red1   0x95
// red2   0x96
// red3   0xA9
// red    0xAA
// 
// rust1  0x81
// rust2  0x94
// rust3  0x68
// 

enum
{
  EPD2_white = 0x00,
  EPD2_black = 0x55,
  EPD2_red   = 0xAA,
  EPD2_gray1 = 0x40,
  EPD2_gray2 = 0x41,
  EPD2_gray3 = 0x54,
  EPD2_pink1 = 0x80,
  EPD2_pink2 = 0x82,
  EPD2_pink3 = 0xA8,
  EPD2_red1  = 0x95,
  EPD2_red2  = 0x96,
  EPD2_red3  = 0xA9,
  EPD2_rust1 = 0x81,
  EPD2_rust2 = 0x94,
  EPD2_rust3 = 0x68
};

byte colours[] =
{
  EPD2_white, EPD2_black, EPD2_red,
  EPD2_gray1, EPD2_gray2, EPD2_gray3,
  EPD2_pink1, EPD2_pink2, EPD2_pink3,
  EPD2_red1,  EPD2_red2,  EPD2_red3,
  EPD2_rust1, EPD2_rust2, EPD2_rust3
};

byte EPD2_white_to_black[] = { EPD2_white, EPD2_gray1, EPD2_gray2, EPD2_gray3, EPD2_black };
byte EPD2_white_to_red[]   = { EPD2_white, EPD2_pink1, EPD2_pink2, EPD2_pink3, EPD2_red   };
byte EPD2_black_to_red[]   = { EPD2_black, EPD2_red1,  EPD2_red2,  EPD2_red3,  EPD2_red   };
byte EPD2_rusts[]          = {             EPD2_rust1, EPD2_rust2, EPD2_rust3             };  // 2w, 2b, 2r

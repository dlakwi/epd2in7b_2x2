# epd2in7b_2x2
Waveshare 2.7'' B/W/R e-Paper display with 15 colour, 2x2 pixel mode

Waveshare product page: https://www.waveshare.com/product/displays/e-paper/epaper-2/2.7inch-e-paper-hat-b.htm

Wiki page: https://www.waveshare.com/wiki/2.7inch_e-Paper_HAT_(B)

This sketch is based on the Waveshare Arduino demo. Two full 5808 byte buffers are used, so an ATmega 1284P uC is used.

The board is the Lakduino Dwee 1284 Mini Pro - ATmega 1284P by Aptinex available on Tindie https://www.tindie.com/products/chathura/lakduino-dwee-1284-mini-pro-atmega-1284p/ or directly from Aptinex https://aptinex.com

To produce 15 colours, Bayer half-tone 2x2 pixels are used. See colours.h for the definitions and DrawAbsolutePixel() in epdpaint.cpp for the rendering.

![image](https://github.com/dlakwi/epd2in7b_2x2/blob/master/epd_bwr_2sm.jpg)


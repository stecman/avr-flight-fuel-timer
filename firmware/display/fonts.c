#include "fonts.h"

/*
  Fontname: -ctrld-fixed-medium-r-normal--16-100-96-96-c-80-ISO10646-1
  Copyright:
  Capital A Height: 0, '1' Height: 9
  Calculated Max Values w= 6 h= 9 x= 2 y= 0 dx= 8 dy= 0 ascent= 9 len= 9
  Font Bounding box     w= 8 h=16 x= 0 y=-4
  Calculated Min Values           x= 0 y= 0 dx= 0 dy= 0
  Pure Font   ascent = 9 descent= 0
  X Font      ascent = 9 descent= 0
  Max Font    ascent = 9 descent= 0

  Generated with:
    ./bdf2u8g -b 48 -e 57 ctrld-fixed-16r.bdf font_ctrld

  Sourced from https://github.com/Tecate/bitmap-fonts
*/
const u8g_fntpgm_uint8_t font_ctrld[167] U8G_FONT_SECTION("font_ctrld") = {
    0,8,16,0,252,9,0,0,0,0,48,57,0,9,0,9,
    0,6,9,9,8,1,0,120,132,140,148,164,196,132,132,120,
    5,9,9,8,2,0,32,96,160,32,32,32,32,32,248,6,
    9,9,8,1,0,120,132,4,8,16,32,64,128,252,6,9,
    9,8,1,0,120,132,4,4,56,4,4,132,120,6,9,9,
    8,1,0,8,24,40,72,136,252,8,8,8,6,9,9,8,
    1,0,252,128,128,128,248,4,4,132,120,6,9,9,8,1,
    0,56,64,128,128,248,132,132,132,120,6,9,9,8,1,0,
    252,4,4,8,8,16,16,32,32,6,9,9,8,1,0,120,
    132,132,132,120,132,132,132,120,6,9,9,8,1,0,120,132,
    132,132,124,4,4,8,112};

/*
  Fontname: -nerdypepper-scientifica-medium-r-normal--11-80-100-100-C-50-ISO10646-1
  Copyright: Copyright (c) 2016, romeovs
  Capital A Height: 7, '1' Height: 7
  Calculated Max Values w= 5 h= 9 x= 4 y= 4 dx= 5 dy= 0 ascent= 8 len= 9
  Font Bounding box     w= 5 h=12 x= 0 y=-2
  Calculated Min Values           x= 0 y=-2 dx= 0 dy= 0
  Pure Font   ascent = 7 descent=-2
  X Font      ascent = 8 descent=-2
  Max Font    ascent = 8 descent=-2

  Generated with:
    ./bdf2u8g -b 32 -e 125 scientifica-11.bdf font_scientifica

  Sourced from https://github.com/Tecate/bitmap-fonts
*/
const u8g_fntpgm_uint8_t font_scientifica[1169] U8G_FONT_SECTION("font_scientifica") = {
    0,5,12,0,254,7,1,145,3,42,32,125,254,8,254,8,
    254,0,0,0,5,4,1,1,7,7,5,2,0,128,128,128,
    128,128,0,128,3,3,3,5,1,4,160,160,160,5,5,5,
    5,0,0,80,248,80,248,80,3,9,9,5,1,255,64,64,
    224,128,224,32,224,64,64,4,4,4,5,0,1,144,32,64,
    144,5,7,7,5,0,0,96,128,144,120,144,144,104,1,3,
    3,5,2,4,128,128,128,4,9,9,5,0,255,48,64,128,
    128,128,128,128,64,48,4,9,9,5,0,255,192,32,16,16,
    16,16,16,32,192,5,5,5,5,0,0,80,32,248,32,80,
    5,5,5,5,0,0,32,32,248,32,32,2,3,3,5,1,
    255,64,64,128,4,1,1,5,0,2,240,1,1,1,5,2,
    0,128,4,8,8,5,0,255,16,16,32,32,64,64,128,128,
    4,7,7,5,0,0,96,144,176,208,144,144,96,3,7,7,
    5,1,0,64,192,64,64,64,64,224,4,7,7,5,0,0,
    96,144,16,32,64,128,240,4,7,7,5,0,0,240,16,32,
    80,16,144,96,4,7,7,5,0,0,128,128,160,160,240,32,
    32,4,7,7,5,0,0,240,128,224,16,16,144,96,4,7,
    7,5,0,0,96,144,128,224,144,144,96,4,7,7,5,0,
    0,240,16,16,32,32,64,64,4,7,7,5,0,0,96,144,
    144,96,144,144,96,4,7,7,5,0,0,96,144,144,112,16,
    144,96,1,4,4,5,2,1,128,0,0,128,2,6,6,5,
    1,255,64,0,0,64,64,128,3,5,5,5,0,0,32,64,
    128,64,32,4,3,3,5,0,1,240,0,240,3,5,5,5,
    1,0,128,64,32,64,128,4,7,7,5,0,0,96,144,16,
    32,64,0,64,4,7,7,5,0,0,96,144,176,176,176,128,
    112,4,7,7,5,0,0,96,144,144,240,144,144,144,4,7,
    7,5,0,0,224,144,144,224,144,144,224,4,7,7,5,0,
    0,96,144,128,128,128,144,96,4,7,7,5,0,0,224,144,
    144,144,144,144,224,4,7,7,5,0,0,240,128,128,224,128,
    128,240,4,7,7,5,0,0,240,128,128,224,128,128,128,4,
    7,7,5,0,0,96,144,128,176,144,144,96,4,7,7,5,
    0,0,144,144,144,240,144,144,144,3,7,7,5,1,0,224,
    64,64,64,64,64,224,4,7,7,5,0,0,112,16,16,16,
    16,144,112,4,7,7,5,0,0,144,160,192,160,144,144,144,
    4,7,7,5,0,0,128,128,128,128,128,128,240,4,7,7,
    5,0,0,144,240,240,144,144,144,144,4,7,7,5,0,0,
    144,208,208,176,176,144,144,4,7,7,5,0,0,96,144,144,
    144,144,144,96,4,7,7,5,0,0,224,144,144,224,128,128,
    128,4,9,9,5,0,254,96,144,144,144,144,144,96,32,16,
    4,7,7,5,0,0,224,144,144,224,144,144,144,4,7,7,
    5,0,0,96,144,128,96,16,144,96,5,7,7,5,0,0,
    248,32,32,32,32,32,32,4,7,7,5,0,0,144,144,144,
    144,144,144,112,4,7,7,5,0,0,144,144,144,144,144,96,
    96,4,7,7,5,0,0,144,144,144,144,240,240,144,4,7,
    7,5,0,0,144,144,144,96,144,144,144,4,7,7,5,0,
    0,144,144,144,112,16,144,112,4,7,7,5,0,0,240,16,
    32,64,128,128,240,3,9,9,5,1,255,224,128,128,128,128,
    128,128,128,224,4,8,8,5,0,255,128,128,64,64,32,32,
    16,16,3,9,9,5,1,255,224,32,32,32,32,32,32,32,
    224,5,3,3,5,0,4,32,80,136,4,1,1,5,0,0,
    240,3,3,3,5,1,4,128,64,32,4,5,5,5,0,0,
    112,144,144,176,80,4,7,7,5,0,0,128,128,224,144,144,
    144,224,4,5,5,5,0,0,96,144,128,144,96,4,7,7,
    5,0,0,16,16,112,144,144,144,112,4,5,5,5,0,0,
    96,144,240,128,112,4,7,7,5,0,0,48,64,64,224,64,
    64,64,4,7,7,5,0,254,112,144,144,144,112,16,96,4,
    7,7,5,0,0,128,128,224,144,144,144,144,4,7,7,5,
    0,0,64,0,192,64,64,64,48,3,9,9,5,0,254,32,
    0,96,32,32,32,32,32,192,4,7,7,5,0,0,128,128,
    144,160,192,160,144,4,7,7,5,0,0,192,64,64,64,64,
    64,48,4,5,5,5,0,0,144,240,144,144,144,4,5,5,
    5,0,0,224,144,144,144,144,4,5,5,5,0,0,96,144,
    144,144,96,4,7,7,5,0,254,224,144,144,144,224,128,128,
    4,7,7,5,0,254,112,144,144,144,112,16,16,4,5,5,
    5,0,0,224,144,128,128,128,4,5,5,5,0,0,112,128,
    96,16,224,4,7,7,5,0,0,64,64,240,64,64,64,48,
    4,5,5,5,0,0,144,144,144,144,112,4,5,5,5,0,
    0,144,144,144,96,96,4,5,5,5,0,0,144,144,144,240,
    144,4,5,5,5,0,0,144,144,96,144,144,4,7,7,5,
    0,254,144,144,144,144,112,16,96,4,5,5,5,0,0,240,
    32,64,128,240,4,9,9,5,0,255,16,32,32,32,192,32,
    32,32,16,1,9,9,5,2,255,128,128,128,128,128,128,128,
    128,128,4,9,9,5,1,255,128,64,64,64,48,64,64,64,
    128};

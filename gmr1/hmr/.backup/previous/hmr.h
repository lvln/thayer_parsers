#pragma once

#include <stdint.h>

#define ACCEPT_ACTION 32767
#define ERROR_ACTION 0
#define T_END 0
#define T_UNDEFINED 16

/*
 * symbols:
 *   0:$end 1:'(' 2:')' 3:';' 4:' ' 5:',' 6:'a' 7:C 8:H 9:D 
 *   10:I 11:E 12:A 13:F 14:B 15:G 16:$undefined 17:$accept
 */
static uint16_t tokens[256] = {
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
  1,2,16,16,5,16,16,16,16,16,16,16,16,16,16,16,16,16,16,3,
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,6,16,16,
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
  4,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16
};

/*
 * lhsnames:
 *   0:C 1:D 2:H 3:D 4:I 5:C 6:E 7:F 8:A 9:F 
 *   10:B 11:G 12:G
 */
static uint16_t lhs[13] = {
  7,9,8,9,10,7,11,13,12,13,14,15,15
};

static uint16_t rhs[13] = {
  1,1,3,3,1,1,1,1,1,3,2,1,1
};

static int16_t table[18][16] = {
  {0,6,0,0,0,0,1,2,3,4,0,5,32767,7,9,8},
  {-11,-11,-11,-11,-11,-11,-11,0,0,0,0,0,0,0,0,0},
  {17,0,0,13,0,0,0,0,0,0,0,0,0,0,0,0},
  {-12,-12,-12,-12,-12,-12,-12,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {-1,-1,-1,-1,-1,15,-1,0,0,0,0,0,0,0,0,0},
  {-5,6,-5,-5,-5,-5,1,10,3,4,11,5,0,7,0,8},
  {-6,-6,-6,-6,-6,-6,-6,0,0,0,0,0,0,0,0,0},
  {-7,-7,-7,-7,-7,-7,-7,0,0,0,0,0,0,0,0,0},
  {-8,-8,-8,-8,-8,-8,-8,0,0,0,0,0,0,0,0,0},
  {-4,-4,-4,13,-4,-4,-4,0,0,0,0,0,0,0,0,0},
  {0,0,12,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {-2,-2,-2,-2,-2,-2,-2,0,0,0,0,0,0,0,0,0},
  {0,6,0,0,0,0,1,0,3,0,0,14,0,7,0,8},
  {-3,-3,-3,-3,-3,15,-3,0,0,0,0,0,0,0,0,0},
  {0,6,0,0,0,0,1,0,3,0,0,0,0,0,0,16},
  {-9,-9,-9,-9,-9,-9,-9,0,0,0,0,0,0,0,0,0},
  {-10,-10,-10,-10,-10,-10,-10,0,0,0,0,0,0,0,0,0}
};

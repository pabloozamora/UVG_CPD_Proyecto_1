#pragma once
#include <cstdint>
#include "color.h"


struct Fragment {
  uint16_t x;      
  uint16_t y;
  int z;
  Color color; 
};

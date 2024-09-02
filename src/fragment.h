#pragma once
#include <cstdint>
#include "color.h"


struct Fragment {
  uint16_t x;      
  uint16_t y;
  double z;
  Color color; 
};

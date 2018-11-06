#pragma once
#include "stdafx.h"

// Global Constants
enum class parserStates {
  INITIAL,
  CONTINUING,
  FINISHED,
  OVERSTEP,
  SWITCH_TO_COMMENT_DODUBLE_SLASH,
  SWITCH_TO_COMMENT_SLASH_STAR
};

// Structs
typedef struct ID {
  int token;
  union {
    double numValue;
    std::string* strValue;
  } unionValue;
  int line;
  int offset;
} ID, *PID;



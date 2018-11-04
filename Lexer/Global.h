#pragma once
#include "stdafx.h"

// Global Constants
enum parserStates {
  INITIAL,
  CONTINUING,
  FINISHED,
  SWITCH_TO_PREPROCESSING,
  SWITCH_TO_COMMENT_DODUBLE_SLASH,
  SWITCH_TO_COMMENT_SLASH_STAR
};

// Structs
typedef struct ID {
  int token;
  union unionV {
    double numValue;
    char* strValue;
  } unionValue;
  int lineNum;
  int lineOffset;
} ID, *PID;



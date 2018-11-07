#pragma once
#include "targetver.h"
#include <cstring>
#include <cctype>
#include <cstdio>
#include <string>

#include "ErrorNotifier.h"
#include "BasicParser.h"

// Global Constants
enum class GPS { // Stands for GlobalParserStates
  INITIAL,
  CONTINUING,
  FINISHED,
  OVERSTEP,
  ERROR,
  SWITCH_TO_COMMENT_DODUBLE_SLASH,
  SWITCH_TO_COMMENT_SLASH_STAR
};

enum class GlobalError { NO_INPUT, NO_TRANSTABLE };

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



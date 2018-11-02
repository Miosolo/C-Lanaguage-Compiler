#pragma once

#include "stdafx.h"
#include <cctype>
#include <string>

// Structs
struct ID {
  int token;
  union unionValue {
    double numValue;
    std::string strValue;
  };
  int lineNum;
  int lineOffset;
};



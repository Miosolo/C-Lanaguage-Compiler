#pragma once
#include "BasicParser.h"
class StringParser :
  public BasicParser {
public:
  StringParser (int lineNum, int offset);
  ~StringParser ();
};


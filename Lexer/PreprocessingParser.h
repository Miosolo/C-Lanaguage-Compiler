#pragma once
#include "StringParser.h"
class PreprocessingParser :
  public StringParser {
public:
  PreprocessingParser (int lineNum, int offset);
  ~PreprocessingParser ();
};


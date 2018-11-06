#pragma once
#include "StringParser.h"
class PreprocessingParser :
  public BasicParser {
private:
  std::string tempStr;

public:
  parserStates feedChar (char feed);
  PreprocessingParser (int lineNum, int offset);
  ~PreprocessingParser ();
};


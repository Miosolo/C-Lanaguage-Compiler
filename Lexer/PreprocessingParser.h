#pragma once
#include "Global.h"
#include "BasicParser.h"

class PreprocessingParser :
  public BasicParser {
private:
  std::string tempStr;

public:
  GPS feedChar (char feed);
  PreprocessingParser (int lineNum, int offset);
  ~PreprocessingParser ();
};


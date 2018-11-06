#pragma once
#include "BasicParser.h"
class IdentifierParser :
  public BasicParser {
private:
  std::string tempName;

public:
  IdentifierParser (int lineNum, int offset);
  ~IdentifierParser ();
  parserStates feedChar (char feed);
};


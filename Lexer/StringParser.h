#pragma once
#include "BasicParser.h"
class StringParser :
  public BasicParser {
private:
  enum class SS{INIT, RUNNING, TERM} state; //Abbr. of StringStates

protected:
  std::string tempStr;

public:
  parserStates feedChar (char feed);
  StringParser (int lineNum, int offset);
  ~StringParser ();
};


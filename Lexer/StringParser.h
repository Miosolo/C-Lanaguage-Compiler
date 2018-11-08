#pragma once
#include "Global.h"
#include "BasicParser.h"

class StringParser :
  public BasicParser {
private:
  enum class SS{INIT, RUNNING, TERM} state; //Abbr. of StringStates

protected:
  std::string tempStr;

public:
  GPS feedChar (char feed);
  StringParser (int lineNum, int offset);
  ~StringParser ();
};


#pragma once
#include "BasicParser.h"
#include <map>

class IdentifierParser :
  public BasicParser {
private:
  std::string tempName;
  static const std::map<std::string, int> keywordDict;

private:
  void checkKeyword ();

public:
  IdentifierParser (int lineNum, int offset);
  ~IdentifierParser ();
  GPS feedChar (char feed);
};


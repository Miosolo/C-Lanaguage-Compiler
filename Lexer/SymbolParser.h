#pragma once
#include "BasicParser.h"
class SymbolParser :
  public BasicParser {
public:
  SymbolParser (int lineNum, int offset);
  ~SymbolParser ();
};


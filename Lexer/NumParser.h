#pragma once
#include "BasicParser.h"

class NumParser :
  public BasicParser {
private:
  int intAcc;
  double devAcc;

private:
  void setState ();

public:
  NumParser (int lineNum, int offset);
  ~NumParser ();
};


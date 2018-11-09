#pragma once
#include "Global.h"

class BasicParser {
protected:
  PID thisID;

public: //virtual funs
  virtual GPS feedChar (char feed) = 0;

  BasicParser (int lineNum, int lineOffset);
  BasicParser ();
  virtual ~BasicParser ();
  PID getPID ();
};
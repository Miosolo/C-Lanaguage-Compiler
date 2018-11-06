#pragma once
#include "Global.h"

class BasicParser {
protected:
  PID thisID;

private:
  virtual void setState () = 0;

public:
  virtual parserStates feedChar (char feed) = 0;
  PID returnPID ();
  BasicParser (int lineNum, int lineOffset);
  virtual ~BasicParser ();
};

BasicParser::BasicParser (int lineNum, int lineOffset) {
  thisID = new struct ID;
  thisID->line = lineNum;
  thisID->offset = lineOffset;
}

BasicParser::~BasicParser () {}

PID BasicParser::returnPID () {
  return thisID;
}


#pragma once
#include "Global.h"

class BasicParser {
private:
  PID thisID;
  int lineNum;
  int lineOffset;

private:
  virtual void craeteID ();
  virtual void setState ();
  virtual bool checkValid ();

public:
  BasicParser ();
  virtual enum returnStates feedChar (char thisChar);
  PID returnPID ();
  virtual ~BasicParser ();
};



BasicParser::BasicParser () {}


BasicParser::~BasicParser () {}

PID BasicParser::returnPID () {
  return thisID;
}


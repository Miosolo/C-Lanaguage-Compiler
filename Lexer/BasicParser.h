#pragma once
#include "Global.h"

class BasicParser {
private:
  struct ID thisID;
  enum returnStates { 
    FINISHED, 
    CONTINUE,
  };

private:
  virtual void craeteID ();
  virtual void setState ();
  virtual bool checkValid ();

public:
  BasicParser ();
  virtual enum returnStates feedChar (char thisChar);
  virtual ~BasicParser ();
};



BasicParser::BasicParser () {}


BasicParser::~BasicParser () {}


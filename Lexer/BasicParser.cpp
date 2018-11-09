#include "BasicParser.h"

BasicParser::BasicParser (int lineNum, int lineOffset) {
  thisID = new ID;
  thisID->line = lineNum;
  thisID->offset = lineOffset;
}

inline BasicParser::BasicParser () {}

inline BasicParser::~BasicParser () {}

PID BasicParser::getPID () {
  return thisID;
}
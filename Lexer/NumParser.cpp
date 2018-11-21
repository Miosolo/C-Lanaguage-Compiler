#include "NumParser.h"


NumParser::NumParser (int lineNum, int lineOffset) 
  : BasicParser(lineNum, lineOffset) {
  intAcc = 0;
  decAcc = 0;
  dec10Pow = -1;
  state = NS::INIT;
}

NumParser::~NumParser () {}

void NumParser::setState (char feed) {
  // Choose the row & column at the transTable
  int col, row;
  switch (state) {
  case NumParser::NS::INIT: col = 0;  break;
  case NumParser::NS::INT:  col = 1;  break;
  case NumParser::NS::DEC:  col = 2;  break;
  case NumParser::NS::TERM: default:  break;
  }

  if (isdigit (feed)) {
    row = 0;
  } else if (feed == '.') {
    row = 1;
  } else {
    row = 2;
  }

  state = transTable[row][col].nextState;
  if (transTable[row][col].nextState == NS::TERM) thisID->token = transTable[row][col].finalToken;
}

GPS NumParser::feedChar (char feed) {
  setState (feed);
  switch (state) {
  case NumParser::NS::INT: 
    intAcc = intAcc * 10 + (feed - '0'); //char-to-int
    return GPS::CONTINUING;
    break;
  case NumParser::NS::DEC:
    if (feed != '.') {
      decAcc += (feed - '0') * pow (10, dec10Pow);
      dec10Pow--;
    }
    return GPS::CONTINUING;
    break;
  case NumParser::NS::TERM:
    thisID->unionValue.numValue = intAcc + decAcc;
    return GPS::OVERSTEP;
    break;
  case NS::INIT: default: // NeverHappen
    return GPS::ERROR;
    break;
  }
}

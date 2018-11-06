#include "stdafx.h"
#include "NumParser.h"


NumParser::NumParser (int lineNum, int lineOffset) 
  : BasicParser(lineNum, lineOffset) {
  intAcc = 0;
  decAcc = 0;
  dec10Pow = 0;
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
    col = 0;
  } else if (feed == '.') {
    col = 1;
  } else {
    col = 2;
  }

  if (transTable[col][row].nextState != NS::TERM) {
    state = transTable[col][row].nextState;
  } else {
    thisID->token = transTable[row][col].finalToken;
  }
}

parserStates NumParser::feedChar (char feed) {
  setState (feed);
  switch (state) {
  case NumParser::NS::INT: 
    intAcc = intAcc * 10 + (feed - '0'); //char-to-int
  case NumParser::NS::DEC:
    if (feed != '.') {
      decAcc += (feed - '0') * pow (10, dec10Pow);
      dec10Pow--;
    }
    return parserStates::CONTINUING;
    break;
  case NumParser::NS::TERM:
    thisID->unionValue.numValue = intAcc + decAcc;
    return parserStates::FINISHED;
    break;
  default:
    break;
  }
}

#include "stdafx.h"
#include "StringParser.h"

StringParser::StringParser (int lineNum, int lineOffset) 
  : BasicParser(lineNum, lineOffset) {
  tempStr = "";
  state = SS::INIT;
}

StringParser::~StringParser () {}

parserStates StringParser::feedChar (char feed) {
  switch (state) {
  case StringParser::SS::INIT:
    state = SS::RUNNING;
    tempStr += feed; 
    break;
  case StringParser::SS::RUNNING:
    if (feed != '\"') {
      tempStr += feed;
      return parserStates::CONTINUING;
    } else {
      tempStr += feed;
      state = SS::TERM;
      thisID->unionValue.strValue = new std::string (tempStr);
      return parserStates::FINISHED;
    }
    break;
  case StringParser::SS::TERM: default: //Never happen.
    break;
  }
}

#include "StringParser.h"

StringParser::StringParser (int lineNum, int lineOffset) 
  : BasicParser(lineNum, lineOffset) {
  tempStr = "\"";
  thisID->token = 500; // token of strings
  state = SS::INIT;
}

StringParser::~StringParser () {}

GPS StringParser::feedChar (char feed) {
  switch (state) {
  case StringParser::SS::INIT:
    state = SS::RUNNING;
    tempStr += feed; 
    break;
  case StringParser::SS::RUNNING:
    if (feed != '\"') {
      tempStr += feed;
      return GPS::CONTINUING;
    } else { // encounter " (end)
      tempStr += feed;
      state = SS::TERM;
      thisID->unionValue.strValue = new std::string (tempStr);
      return GPS::FINISHED;
    }
    break;
  case StringParser::SS::TERM: default: //Never happen.
    break;
  }
}

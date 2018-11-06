#include "stdafx.h"
#include "PreprocessingParser.h"

PreprocessingParser::PreprocessingParser (int lineNum, int lineOffset)
  : BasicParser(lineNum, lineOffset) {
  tempStr = "";
}

PreprocessingParser::~PreprocessingParser () {}

parserStates PreprocessingParser::feedChar (char feed) {
  if (feed != '\n') {
    tempStr += feed;
    return parserStates::CONTINUING;
  } else {
    thisID->unionValue.strValue = new std::string (tempStr);
    return parserStates::FINISHED;
  }
}

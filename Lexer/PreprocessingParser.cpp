#include "stdafx.h"
#include "PreprocessingParser.h"

PreprocessingParser::PreprocessingParser (int lineNum, int lineOffset)
  : BasicParser(lineNum, lineOffset) {
  thisID->token = 700; // token of preprocessing token
  tempStr = "";
}

PreprocessingParser::~PreprocessingParser () {}

GPS PreprocessingParser::feedChar (char feed) {
  if (feed != '\n') {
    tempStr += feed;
    return GPS::CONTINUING;
  } else {
    thisID->unionValue.strValue = new std::string (tempStr);
    return GPS::FINISHED;
  }
}

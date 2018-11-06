#include "stdafx.h"
#include "IdentifierParser.h"

IdentifierParser::IdentifierParser (int lineNum, int lineOffset) 
  : BasicParser(lineNum, lineOffset)
{
  tempName = "";
}

parserStates IdentifierParser::feedChar (char feed) {
  if (isalnum (feed) || feed == '_') {
    tempName += feed;
    return parserStates::CONTINUING;
  } else {
    thisID->unionValue.strValue = new std::string (tempName);
    thisID->token = 0; //TO-DO: token?
    return parserStates::FINISHED;
  }
}

IdentifierParser::~IdentifierParser () {}

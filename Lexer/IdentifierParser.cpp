#include "IdentifierParser.h"

const std::map<std::string, int> IdentifierParser::keywordDict = {
    {"char", 201},   {"double", 202},   {"enum", 203},
    {"float", 204},  {"int", 205},      {"long", 206},
    {"short", 207},  {"signed", 208},   {"struct", 209},
    {"union", 210},  {"unsigned", 211}, {"void", 212},
    {"for", 213},    {"do", 214},       {"while", 215},
    {"break", 216},  {"continue", 217}, {"if", 218},
    {"else", 219},   {"goto", 220},     {"switch", 221},
    {"case", 222},   {"default", 223},  {"return", 224},
    {"auto", 225},   {"extern", 226},   {"register", 227},
    {"static", 228}, {"const", 229},    {"sizeof", 230},
    {"typedef", 231},{"volatile", 232}
};

IdentifierParser::IdentifierParser (int lineNum, int lineOffset) 
  : BasicParser(lineNum, lineOffset)
{
  tempName = "";
}

GPS IdentifierParser::feedChar (char feed) {
  if (isalnum (feed) || feed == '_') {
    tempName += feed;
    return GPS::CONTINUING;
  } else {
    thisID->unionValue.strValue = new std::string (tempName);
    checkKeyword ();
    return GPS::OVERSTEP;
  }
}

IdentifierParser::~IdentifierParser () {}

void IdentifierParser::checkKeyword () {
  auto result = keywordDict.find (*(thisID->unionValue.strValue));
  if (result == keywordDict.end ()) {
    thisID->token = 300; //token of non-keywords identifier
  } else {
    thisID->token = result->second;
  }
}

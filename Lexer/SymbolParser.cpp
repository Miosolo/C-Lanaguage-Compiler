#include "stdafx.h"
#include "SymbolParser.h"

const std::map<char, int> SymbolParser::offsetMap = {
    {'I', 11}, // 'I' Stands for initial
    {'+', 0},  {'-', 1},  {'*', 2},  {'/', 3},
    {'<', 4},  {'>', 5},  {'!', 6},  {'=', 7},
    {'&', 8},  {'^', 9},  {'|', 10}, {'.', 11},
    {';', 12}, {'\'', 13},{'(', 14}, {')', 15},
    {'[', 16}, {']', 17}, {'{', 18 },{'}', 19},
    {'?', 20}, {',', 21}, {':', 22}, {'~', 23},
};

SymbolParser::SymbolParser (int lineNum, int offset) 
  : BasicParser(lineNum, offset) {
  state = SS::INIT;
  thisOffset = offsetMap.find ('I')->second; //Set the X-axis value of the table
  readTable ();
}

SymbolParser::~SymbolParser () {}

void SymbolParser::readTable () {
  FILE *csv = fopen (tableAddress, "r");
  if (csv == NULL) {
    //TO-DO: Error: no transTable found.
    state = SS::ERROR;
    return;
  } else {
    int row, col;
    fscanf (csv, "%d,%d", &row, &col); // Read Row & Col defined in the head of .csv
    while (fgetc (csv) != '\n'); //Jump to the end of first line (no data)
    transTable.resize (row); 

    for (auto rowIt = transTable.begin (); rowIt != transTable.end (); rowIt++) {
      (*rowIt).resize (col);
      for (auto colIt = (*rowIt).begin (); colIt != (*rowIt).end (); colIt++) {
        int tempState, tempInt;
        fscanf (csv, "%d,%d", &tempState, &tempInt);
        (*colIt).nextOrToken = tempInt;
        switch (tempState) {
        case 0:
          (*colIt).nextState = SS::FIN;
          break;
        case 1:
          (*colIt).nextState = SS::OVER;
          break;
        case 2:
          (*colIt).nextState = SS::PEND;
          break;
        case 3:
          (*colIt).nextState = SS::SS; //slash-star
          break;
        case 4:
          (*colIt).nextState = SS::DS; //double-slash
          break;
        default:
          break;
        }
        fgetc (csv); //Skip the following ',' of '\n'
      }
    }
  }
}

void SymbolParser::setState (char feed) {
  auto next = offsetMap.find (feed);
  int nextOffset;

  if (next == offsetMap.end ()) {
    nextOffset = transTable.end () - transTable.begin (); // 在transTable中，others位于最后一行
  } else {
    nextOffset = next->second;
  }

  state = transTable[thisOffset][nextOffset].nextState;
  switch (state) {
  case SymbolParser::SS::PEND:
    thisOffset = transTable[thisOffset][nextOffset].nextOrToken;
    break;
  case SymbolParser::SS::FIN: case SymbolParser::SS::OVER:
    thisID->token = transTable[thisOffset][nextOffset].nextOrToken;
    break;
  default:
    break;
  }
}

GPS SymbolParser::feedChar (char feed) {
  setState (feed);
  switch (state) {
  case SymbolParser::SS::PEND:
    tempSymbol.push_back (feed);
    return GPS::CONTINUING;
    break;
  case SymbolParser::SS::FIN: case SymbolParser::SS::OVER:
    tempSymbol.push_back (feed);
    thisID->unionValue.strValue = new std::string (tempSymbol);
    return state == SS::FIN ? GPS::FINISHED : GPS::OVERSTEP;
    break;
  case SS::DS:
    return GPS::SWITCH_TO_COMMENT_DODUBLE_SLASH;
    break;
  case SS::SS:
    return GPS::SWITCH_TO_COMMENT_SLASH_STAR;
  case SymbolParser::SS::ERROR:
    //TO-DO: Report an error;
    return GPS::ERROR;
    break;
  default:
    break;
  }
}

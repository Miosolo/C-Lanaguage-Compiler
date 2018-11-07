#include "stdafx.h"
#include "SymbolParser.h"


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
    //Error: no transTable found.
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
        default:
          break;
        }
        fgetc (csv); //Skip the following ',' of '\n'
      }
    }
  }
}

void SymbolParser::setState (char feed) {
  int nextOffset = offsetMap.find (feed)->second;
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

parserStates SymbolParser::feedChar (char feed) {
  setState (feed);
  switch (state) {
  case SymbolParser::SS::PEND:
    tempSymbol.push_back (feed);
    return parserStates::CONTINUING;
    break;
  case SymbolParser::SS::FIN: case SymbolParser::SS::OVER:
    tempSymbol.push_back (feed);
    thisID->unionValue.strValue = new std::string (tempSymbol);
    return state == SS::FIN ? parserStates::FINISHED : parserStates::OVERSTEP;
    break;
  case SymbolParser::SS::ERROR:
    //TO-DO: Report an error;
    return parserStates::ERROR;
    break;
  default:
    break;
  }
}

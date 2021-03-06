#include "SymbolParser.h"

std::vector<std::vector<SymbolParser::trans> > SymbolParser::transTable{ {} };

const std::map<char, int> SymbolParser::offsetMap = {
    //{'I', 14}, // 'I' Stands for initial, only use once
    {'+', 0},  {'-', 1},  {'*', 2},  {'/', 3},
    {'<', 4},  {'>', 5},  {'!', 6},  {'=', 7},
    {'&', 8},  {'^', 9},  {'|', 10}, {'%', 11}, // "<<"-12 and ">>"-13 will never occur
    {'.', 14}, {';', 15}, {'\'', 16},{'(', 17}, 
    {')', 18}, {'[', 19}, {']', 20}, {'{', 21 },
    {'}', 22}, {'?', 23}, {',', 24}, {':', 25}, 
    {'~', 26},
};

SymbolParser::SymbolParser (int lineNum, int offset) 
  : BasicParser(lineNum, offset) {
  state = SS::INIT;
  thisOffset = 14; //Set the X-axis value of the table, 14 for 'initial'
  if (!readTable ()) SymbolParser::~SymbolParser();
}

SymbolParser::~SymbolParser () {}

bool SymbolParser::readTable () {
  if (!transTable[0].empty()) { 
    return true;
  } else { //__init__ == false
    FILE *csv = fopen (GLOBAL_ABSOLUTE_TRANSTABLE, "r");
    if (csv == NULL) {
      ErrorNotifier::showError (GlobalError::NO_TRANSTABLE);
      return false;
    } else { // csv open secceeded.
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
      fclose (csv);
      return true;
    }
  }  
}

void SymbolParser::setState (char feed) {
  auto next = offsetMap.find (feed);
  int nextOffset;

  if (next == offsetMap.end ()) {
    nextOffset = transTable.end () - transTable.begin () - 1; // 在transTable中，others位于最后一行
  } else {
    nextOffset = next->second;
  }

  state = transTable[nextOffset][thisOffset].nextState;
  switch (state) {
  case SymbolParser::SS::PEND:
    thisOffset = transTable[nextOffset][thisOffset].nextOrToken;
    break;
  case SymbolParser::SS::FIN: case SymbolParser::SS::OVER:
    thisID->token = transTable[nextOffset][thisOffset].nextOrToken;
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
  case SymbolParser::SS::FIN: 
    tempSymbol.push_back (feed);
    // Intentional FALL-DOWN
  case SymbolParser::SS::OVER:
    thisID->unionValue.strValue = new std::string (tempSymbol);
    return state == SS::FIN ? GPS::FINISHED : GPS::OVERSTEP;
    break;
  case SS::DS:
    return GPS::SWITCH_TO_COMMENT_DODUBLE_SLASH;
    break;
  case SS::SS:
    return GPS::SWITCH_TO_COMMENT_SLASH_STAR;
  case SymbolParser::SS::ERROR:
    return GPS::ERROR;
    break;
  default:
    break;
  }
}

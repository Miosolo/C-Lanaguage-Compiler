#pragma once
#include "Global.h"
#include "BasicParser.h"

class NumParser :
  public BasicParser {
private:
  enum class NS {INIT, INT, DEC, TERM}; //Abbr. of NumStates
  struct trans {
    NS nextState; // if has-next
    int finalToken; // if !has-next, token = 0
  };
  //
  struct trans transTable[3][3] = {
    /*|Event \ State|    Initial     |   Integer          |      Decimal      |*/
    /*|Digit*/      {{NS::INT, 0},     {NS::INT, 0},         {NS::DEC, 0}},
    /*| "." */      {{NS::DEC, 0},     {NS::DEC, 0},         {NS::TERM, 402}},
    /*|Others*/     {{NS::TERM, 0},    {NS::TERM, 401},      {NS::TERM, 402}}
    /*               ^never happen^ */
  };

private:
  int intAcc;
  double decAcc;
  int dec10Pow;
  NS state;

private:
  void setState (char feed);

public:
  GPS feedChar (char feed);

public:
  NumParser (int lineNum, int offset);
  ~NumParser ();
};


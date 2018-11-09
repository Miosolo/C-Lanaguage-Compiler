#include "stdafx.h"

#include "Global.h"
#include "BasicParser.h"
#include "IdentifierParser.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LexerUnitTest {
  TEST_CLASS (TestIdentifierParser) {
public:

  TEST_METHOD (TestInit) {
    IdentifierParser idp1 (1, 23);
    PID p1 = idp1.getPID ();
    Assert::AreEqual (23, p1->offset);
  }

  TEST_METHOD (BlackNonKeyword) {
    IdentifierParser idp2 (2, 20);
    PID p2 = idp2.getPID ();
    std::string inStr = "__Main__2;";

    for each (char c in inStr) {
      idp2.feedChar (c);
    }

    Assert::AreEqual (300, p2->token);
    inStr.pop_back ();
    Assert::AreEqual (inStr, *(p2->unionValue.strValue));
  }
  };
}
#include "CppUnitTest.h"
#include "stdafx.h"

#include "BasicParser.h"
#include "Global.h"
#include "SymbolParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ParserUnitTest {
  TEST_CLASS (TestSymParser) {
public:

  TEST_METHOD (TestAssert) {
    SymbolParser smp1 (2, 23);
    PID p1 = smp1.getPID ();

    smp1.feedChar ('=');
    smp1.feedChar ('2');

    Assert::AreEqual (p1->token, 111);
    }  // namespace UnitTest

  TEST_METHOD (TestNotEqual) {
    SymbolParser smp2 (2, 23);
    PID p2 = smp2.getPID ();

    smp2.feedChar ('!');
    smp2.feedChar ('=');

    Assert::AreEqual ("!=", p2->unionValue.strValue->c_str ());
    Assert::AreEqual (109, p2->token);
  }

  };
}
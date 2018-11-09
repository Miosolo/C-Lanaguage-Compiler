#include "CppUnitTest.h"
#include "stdafx.h"

#include "BasicParser.h"
#include "Global.h"
#include "SymbolParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
  TEST_CLASS (TestSymParser) {
public:

  TEST_METHOD (TestAssert) {
    SymbolParser smp1 (2, 23);
    PID p1 = smp1.getPID ();

    smp1.feedChar ('=');
    smp1.feedChar ('2');

    Assert::AreEqual (p1->token, 111);
    }  // namespace UnitTest
  };
}
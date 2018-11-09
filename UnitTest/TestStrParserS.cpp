#include "CppUnitTest.h"
#include "stdafx.h"

#include "BasicParser.h"
#include "Global.h"
#include "StringParser.h"
#include "CommentParser.h"
#include "PreprocessingParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ParserUnitTest {
  TEST_CLASS (TestStrParser) {
public:

  TEST_METHOD (TestSimple) {
    StringParser strp (2, 23);
    PID p1 = strp.getPID ();

    std::string testStr = "test\"";
    for each (char c in testStr) {
      strp.feedChar (c);
    }

    Assert::AreEqual (p1->token, 500);
    Assert::AreEqual ("\"test\"", p1->unionValue.strValue->c_str());
  }  // namespace UnitTest

  TEST_METHOD (TestCut) {
    StringParser strp (2, 23);
    PID p1 = strp.getPID ();

    std::string testStr = "FYHEOSDFHL\"fsaldkfjl;s";
    for each (char c in testStr) {
      strp.feedChar (c);
    }

    Assert::AreEqual (p1->token, 500);
    Assert::AreEqual ("\"FYHEOSDFHL\"", p1->unionValue.strValue->c_str());
  }

  };

  TEST_CLASS (TestPrepParser) {
public:

  TEST_METHOD (BlackTest) {
    PreprocessingParser pp(2, 1);
    PID p = pp.getPID ();

    std::string prepStr = "#include <stdio.h>\n";
    for each (char c in prepStr) {
      pp.feedChar (c);
    }

    Assert::AreEqual (700, p->token);
    Assert::AreEqual ("#include <stdio.h>", p->unionValue.strValue->c_str ());
  }
  };

  TEST_CLASS (TestComParser) {
public:

  TEST_METHOD (TestInline) {
    CommentParser inp (2, 3 - 2, GPS::SWITCH_TO_COMMENT_DODUBLE_SLASH);
    PID p = inp.getPID ();
    Assert::AreEqual (1, p->offset);

    std::string com = "test comment\t\n";
    for each (char c in com) {
      inp.feedChar (c);
    }
    Assert::AreEqual ("//test comment\t", p->unionValue.strValue->c_str ());
  }

  TEST_METHOD (TestMultiLine) {
    CommentParser mup (2, 3 - 2, GPS::SWITCH_TO_COMMENT_SLASH_STAR);
    PID p = mup.getPID ();
    Assert::AreEqual (1, p->offset);

    std::string com = "line1\n line2\n\t line3*/";
    for each (char c in com) {
      mup.feedChar (c);
    }
    Assert::AreEqual ("/*line1\n line2\n\t line3*/", p->unionValue.strValue->c_str ());
  }
  };
}


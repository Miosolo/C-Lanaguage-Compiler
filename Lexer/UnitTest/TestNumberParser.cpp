#include "stdafx.h"
#include "CppUnitTest.h"

#include "Global.h"
#include "BasicParser.h"
#include "NumParser.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LexerUnitTest
{		
	TEST_CLASS(TestNumberParser)
	{
	public:

    TEST_METHOD (TestGlobalVar) {
      ID id = { 401, 123, 1, 1 };
      Assert::AreEqual (401, id.token);
    }
		
		TEST_METHOD(BlackTestInt) {
      NumParser num1 (1, 21);
      std::string numTest = "123n";
      /*for each (char c in numTest) {
        num1.feedChar (c);
      }*/

      /*PID resultID = num1.getPID ();

      Assert::AreEqual (resultID->line, 1);
      Assert::AreEqual (resultID->offset, 21);
      Assert::AreEqual (resultID->unionValue.numValue, 123.0);
      Assert::AreEqual (resultID->token, 401);*/
		}

	};
}
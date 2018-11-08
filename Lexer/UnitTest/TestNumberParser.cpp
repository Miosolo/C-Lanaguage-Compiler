#include "stdafx.h"
#include "CppUnitTest.h"

#include "Global.h"
#include "BasicParser.h"
#include "NumParser.h"

#include "Numparser.cpp"
#include "BasicParser.cpp"

#include <cstdlib>
#include <ctime>


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
      for each (char c in numTest) {
        num1.feedChar (c);
      }

      PID resultID = num1.getPID ();

      Assert::AreEqual (resultID->line, 1);
      Assert::AreEqual (resultID->offset, 21);
      Assert::AreEqual ((int)resultID->unionValue.numValue, 123);
      Assert::AreEqual (resultID->token, 401);
		}

    TEST_METHOD (BlackTestDouble) {
      NumParser num2 (1, 21);
      std::string numTest = "52342.222.";
      for each (char c in numTest) {
        num2.feedChar (c);
      }

      PID resultID = num2.getPID ();

      Assert::AreEqual (52342.222, resultID->unionValue.numValue);
      Assert::AreEqual (402, resultID->token);
    }

    TEST_METHOD (BlackExtreme) {
      int kase = 100;
      srand ((unsigned)time(NULL));
      double magic = 1941.244;
      while (kase--) {
        std::string msg = "Case " + std::to_string (100 - kase);
        Logger::WriteMessage (msg.c_str ());
        
        NumParser num3 (1, 1);
        PID resultID = num3.getPID ();

        double random = (double)(rand() * magic / RAND_MAX);

        std::string tempNum = std::to_string (random);
        Logger::WriteMessage (tempNum.c_str ());
        tempNum.push_back ('\n');

        for each (char c in tempNum) {
          num3.feedChar (c);
        }
        tempNum.pop_back ();

        Assert::AreEqual (tempNum, std::to_string(resultID->unionValue.numValue));
      }
    }

	};
}
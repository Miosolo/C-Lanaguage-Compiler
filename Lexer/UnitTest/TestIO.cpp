#include "stdafx.h"

#include "Global.h"
#include "BasicParser.h"
#include "Reader.h"
#include "Writer.h"
#include "Lexer.h"
#include "ErrorNotifier.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ParserUnitTest {
  TEST_CLASS (TestReader) {
public:
  TEST_METHOD (TestRead) {
    //Toggle Reader.class to full-public
    Reader aReader ("../../UnitTest/TestFile/testRead.c", NULL);
    Assert::IsTrue (aReader.openInputFile ("../../UnitTest/TestFile/testRead.c"));
    std::string fileContent = "123\n";
    
    for each (char c in fileContent) {
      char r = aReader.readChar ();
      Assert::AreEqual (c, r);
    }
  }
  };

  TEST_CLASS (TestLexerCtrl) {
public:
  TEST_METHOD (TestReaderStates) {
    Lexer lex ("../../UnitTest/TestFile/testIO.c", "../../UnitTest/TestFile/testIO.csv");
    //Toggle Lexer.class to full public
    Reader aReader = *(lex.aReader);

    Assert::IsTrue (aReader.openInputFile ("../../UnitTest/TestFile/testIO.c"));
    std::string fileContent = "1\na\n#include\n";

    aReader.step ();//read "1"
    PID p1 = aReader.parser->getPID ();
    aReader.step ();//read "\n"
    Assert::AreEqual (1, p1->offset);
    Assert::AreEqual (1.0, p1->unionValue.numValue);
    Assert::IsTrue (aReader.traceBackFlag);

    aReader.step (); //traceback, read '\n'
    aReader.step (); //read 'a'
    PID p2 = aReader.parser->getPID ();
    aReader.step ();//read "\n"
    Assert::AreEqual ("a", p2->unionValue.strValue->c_str ());

    aReader.step (); //traceback;
    aReader.step (); //read "#"
    PID p3 = aReader.parser->getPID ();
    Assert::AreEqual (3, p3->line);
    Assert::AreEqual (1, p3->offset);
    Assert::AreEqual (700, p3->token);
  }

  TEST_METHOD (TestOutputRaw) {
    Lexer lex ("../../UnitTest/TestFile/testIO.c", "");
    Writer* w = lex.aWriter;
    Reader* r = lex.aReader;

    r->run ();
    w;

    //Toggle Writer.class to full public
    Assert::AreEqual (1.0, w->symbolList[0]->unionValue.numValue);
    Assert::AreEqual ("a", w->symbolList[1]->unionValue.strValue->c_str ());
    Assert::AreEqual ("#include", w->symbolList[2]->unionValue.strValue->c_str ());  
  }

  TEST_METHOD (BlackBoxTest) {
    Lexer lex ("../../UnitTest/TestFile/testIO.c", "../../UnitTest/TestFile/testIO.csv");
    lex.run ();
    Assert::IsNotNull (fopen ("../../UnitTest/TestFile/testIO.csv", "r"));
  }
  };
}
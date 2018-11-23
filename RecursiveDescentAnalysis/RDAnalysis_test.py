from RDAnalysis import Parser
from nose.tools import raises

class Test_RDA(object):
  def test_init(self):
    p = Parser('RecursiveDescentAnalysis/TestFile/source.csv')
    p.step()
    assert p.sym == {'token': 401, 'line': 1, 'offset': 2, 'content':'1'}
  
  @raises(IOError)
  def test_wrong_file(self):
    p = Parser('RecursiveDescentAnalysis/TestFile/wrong.csv')

  def test_simple_return(self):
    p = Parser('RecursiveDescentAnalysis/TestFile/test_bare.csv')
    assert p.parse() == True

  def test_run_source(self):
    p = Parser('RecursiveDescentAnalysis/TestFile/source.csv')
    assert p.parse() == True

  def test_run_source_1(self):
    p = Parser('RecursiveDescentAnalysis/TestFile/source1.csv')
    assert p.parse() == True

  def test_run_source_2(self):
    p = Parser('RecursiveDescentAnalysis/TestFile/source2.csv')
    assert p.parse() == True

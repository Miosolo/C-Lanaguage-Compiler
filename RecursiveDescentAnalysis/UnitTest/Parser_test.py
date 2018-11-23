from TheoreticalParser import Parser

class TestSuite1(object):
  def test_init(self):
    p = Parser('source.txt')
    assert p.index == 0
    assert p.source == '1+3*(2*7+5)+i'

  def test_step(self):
    p = Parser('test_EOF.txt')
    p.step()
    assert p.sym == '#'
  
  def test_parse_bare(self):
    p = Parser('test_bare.txt')
    assert p.parse() == True
  
  def test_parse_fault(self):
    p = Parser('test_simpleFault.txt')
    assert p.parse() == False

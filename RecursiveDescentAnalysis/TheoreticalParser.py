
class Parser(object):
  V_N = ['E', 'e', 'T', 't', 'F']

  first = {
    'E': ['(', 'i'],
    'e': ['+', '-'],
    'T': ['(', 'i'],
    't': ['*', '/'],
    'F': ['(', 'i']
  }

  follow = {
    'E': [')', '#'],
    'e': [')', '#'],
    'T': ['+', '-', '#', ')'],
    't': ['+', '-', '#', ')'],
    'F': ['+', '-', '*', '/', ')', '#']
  }

  producer = {
    'E': [['T', 'e']],
    'e': [['+', 'T', 'e'], ['-', 'T', 'e']],
    'T': [['F', 't']],
    't': [['*', 'F', 't'], ['/', 'F', 't']],
    'F': [['(', 'E', ')'], ['i']]
  }
 
  def __init__(self, inputFileLocation):
    try:
      with open(inputFileLocation, 'r') as f:
        self.source = f.read()
        self.index = 0
    except IOError:
      print("Open Source File Failed.")

  def step(self):
    try:
      self.sym = self.source[self.index]
      self.index += 1
    except IndexError:
      self.sym = '#'

  def implement(self, seed):
    if self.sym in self.first[seed]:
        prods = [p for p in filter(lambda p: p[0] == self.sym or\
                                   (p[0] in self.V_N and self.sym in self.first[p[0]]), self.producer[seed])]
        # then prods should be a len(1) list
        prod = prods[0]

        for v in prod:
          if v in self.V_N:
            if self.implement(v) == True: # 表达式可解析
              pass
            else:
              return False
          
          else: # v in V_T
            if v == self.sym:
              self.step()
            else: # V_T not match
              return False
        else:
          return True

    elif self.sym in self.follow[seed]:
      return True

    else: # this char in neither FIRST() nor FOLLOW()
      return False

  def parse(self):
    self.step()
    return self.implement('E') and self.sym == '#'

    

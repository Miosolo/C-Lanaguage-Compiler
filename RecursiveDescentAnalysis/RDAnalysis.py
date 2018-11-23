import csv

class Parser(object):
  V_N = ['E', 'e', 'T', 't', 'F', 'i']
  V_T = [101, 102, 103, 104, 137, 138, 300, 401, 405]

  first = {
    'E': [137, 'i'],
    'e': [101, 102],
    'T': [137, 'i'],
    't': [103, 104],
    'F': [137, 'i'],
    'i': [401, 405, 300]
  }

  follow = {
    'E': [138, '#'],
    'e': [138, '#'],
    'T': [101, 102, '#'],
    't': [101, 102, '#'],
    'F': [101, 102, 103, 104, '#']
  }

  producer = {
    'E': [['T', 'e']],
    'e': [[101, 'T', 'e'], [102, 'T', 'e']],
    'T': [['F', 't']],
    't': [[103, 'F', 't'], [104, 'F', 't']],
    'F': [[137, 'E', 138], ['i']]
  }

  def __init__(self, inputFileLocation):
    with open(inputFileLocation) as f:
      self.f_csv = csv.reader(f)
      self.step()

  def step(self):
    try:
      line = next(self.f_csv) # is the list of elements in a line
      while len(line) == 0 or str.isdigit(line[0]) == False: #读入的是空行，或是标题行
        line = next(self.f_csv)
      # 现在line是一个有效行

      self.sym = {'token': int(line[0]), 'line': int(line[2]), 'offset': int(line[3])}

      if self.sym['token'] // 100 == 2: # token = 2xx <=> keywords
        # TODO:识别关键字
        self.step()

    except EOFError:
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
              return True
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
    return self.implement('E')
import csv

class Parser(object):
  V_N = ['E', 'E\'', 'T', 'T\'', 'F', 'I']
  V_T = [101, 102, 103, 104, 137, 138, 300, 401, 405]

  first = {
    'E': [137, 300, 401, 405], # (, int, float, id
    'E\'': [101, 102],           # +, -
    'T': [137, 300, 401, 405], # (, int, float, id
    'T\'': [103, 104],           # *, /
    'F': [137, 300, 401, 405], # (, int, float, id
    'I': [401, 405, 300]       # int, float, id
  }

  follow = {
    'E': [138, '#'],          # ), #
    'E\'': [138, '#'],          # ) #
    'T': [101, 102, 138, '#'],     # +, -, ), #
    'T\'': [101, 102, 138, '#'],     # +, -, ), #
    'F': [101, 102, 103, 104, 138, '#'], # +, -, *, /, ), #
    'I': [101, 102, 103, 104, 138, '#'] # +, -, *, /, ), #
  }

  producer = {
    'E': [['T', 'E\'']],
    'E\'': [[101, 'T', 'E\''], [102, 'T', 'E\'']],
    'T': [['F', 'T\'']],
    'T\'': [[103, 'F', 'T\''], [104, 'F', 'T\'']],
    'F': [[137, 'E', 138], ['I']],
    'I': [[300], [401], [405]]
  }

  def __init__(self, inputFileLocation):
    try:
      f = open(inputFileLocation, 'r')
      self.f_csv = csv.reader(f)
      self.parsedList = []
      self.sym = None
    except IOError:
      print('Error: cannot find the specific symbol table.')
      raise IOError

  def step(self):
    if self.sym != None:
      print('='*20)
      print('Using Producer: ' + self.usingProd['L'] + ' -> ' + str(self.usingProd['R']))
      print('Parsed Expression: ' + str(self.parsedList))
      print('Current symbol: ' + self.sym['content'])
      self.parsedList.append(self.sym['content'])
    else: # self.sym 不存在 <=> 第一次step
      pass

    try:
      line = next(self.f_csv) # is the list of elements in a line
      while len(line) == 0 or str.isdigit(line[0]) == False: #读入的是空行，或是标题行
        line = next(self.f_csv)
      # 现在line是一个有效行

      self.sym = {'token': int(line[0]), 'content': line[1], 'line': int(line[2]), 'offset': int(line[3])}

      if self.sym['token'] not in self.V_N and self.sym['token'] not in self.V_T:
        raise RuntimeError('Unsuppoted Symbol.')

    except StopIteration:
      self.sym['token'] = self.sym['content'] = '#'

  def implement(self, seed):
    if self.sym['token'] in self.first[seed]:
        prods = [p for p in filter(lambda p: p[0] == self.sym['token'] or\
                                   (p[0] in self.V_N and self.sym['token'] in self.first[p[0]]), self.producer[seed])]
        # then prods should be a len(1) list
        prod = prods[0]
        self.usingProd = {'L': seed, 'R': prod}

        for v in prod:
          if v in self.V_N:
            if self.implement(v) == True: # 表达式可解析
              pass
            else:
              return False
          
          else: # v in V_T
            if v == self.sym['token']:
              self.step()
            else: # V_T not match
              return False
        else:
          return True

    elif self.sym['token'] in self.follow[seed]:
      return True

    else: # this char in neither FIRST() nor FOLLOW()
      return False

  def parse(self):
    self.step()
    result = self.implement('E')
    self.step()

    if result == True:
      print('No error occurred, valid expression!')
    else:
      with open('error.txt' ,'w') as f:
        f.write('An error occurred at line' + self.sym['line'] + ', ' + self.sym['offset'])
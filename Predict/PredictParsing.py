import FirstCollection
import FollowCollection
from functools import reduce

def getPredictTable(prodDict, V_N, V_T):
  firstCollection = FirstCollection.getFirstCollection(prodDict, V_N, V_T)
  followCollection = FollowCollection.getFollowCollection(prodDict, V_N, V_T, firstCollection)

  getFirst = lambda v: firstCollection[v] if v in V_N else set([v])

  def getSequenceFirst(prod):
    first = set()
    for V in prod:
      thisFirst = getFirst(V)
      if 'eps' in thisFirst:
        first |= thisFirst if V == prod[len(prod)-1] else thisFirst - {['eps']}
        continue
      else: # 不含eps
        first |= thisFirst
        break
    return first

  predictTable = dict()
  for vn in V_N:
    predictTable[vn] = dict()  # 初始化分析表为全空

  for vn in prodDict.keys():
    for prod in prodDict[vn]:
      seqFirst = getSequenceFirst(prod)
      for vt in seqFirst:
        if vt != 'eps':
          predictTable[vn][vt] = prod
        else: #eps
          for vnext in followCollection[vn]:
            predictTable[vn][vnext] = prod
  
  return predictTable

def predictParsing(grammer, sentence):
  '''
  Input: 
    grammer = {
      'S': V_N
      'V_N': set
      'V_T': set
      prodDict: dict
    }
    sentence: string
  Output:
    AC(True) / REJ(False)
  '''
  def error():
    print('Unmatched symbol ' + sentence[i] + ' found at ' + str(i) + '.')
    return False
  list2str = lambda l: reduce(lambda ele, substr: ('ε' if ele == 'eps' else ele) + substr, l)

  predictTable = getPredictTable(grammer['prodDict'], grammer['V_N'], grammer['V_T'])
  symbolStack = ['#', grammer['S']]
  step = 0
  i = 0
  sentenceLen = len(list2str(sentence)) + 4
  output = lambda outList : print('{:^6}  {:<10}  {:<{slen}}  {:<15}'.format(*outList, slen = sentenceLen))

  output(['Step', 'Stack', 'Input', 'Production'])

  while i < len(sentence):
    X = symbolStack[len(symbolStack) - 1]
    c = sentence[i]

    if X in grammer['V_T']:
      if c == X:
        output([str(step), list2str(symbolStack), list2str(sentence[i:]), c+' is matched'])
        if X != '#':
          symbolStack.pop()
          step += 1
          i += 1
        else: # c == X and X == '#':
          return True
      else: # c != X
        return error()
    elif X in grammer['V_N']:
      try:
        symbolStack.pop()
        prod = predictTable[X][c]
        output([str(step), list2str(symbolStack+[X]), list2str(sentence[i:]), X+'->'+list2str(['ε'] if prod == ['eps'] else prod)])
        if prod != ['eps']:
          symbolStack += prod[::-1]
        step += 1
      except KeyError:
        return error()
    else:
      raise RuntimeError(X + ' cannot be classified, check the grammer again.')
  pass
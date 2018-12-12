import FirstCollection
import FollowCollection
from functools import reduce

def getPredictTable(prodDict, V_N, V_T):
  firstCollection = FirstCollection.getFirstCollection(prodDict, V_N, V_T)
  followCollection = FollowCollection.getFollowCollection(prodDict, V_N, V_T, firstCollection)

  getFirst = lambda v: firstCollection[v] if v in V_N else set([v]) # 方便计算，定义终结符的FIRST集为自身

  def getSequenceFirst(prod): # 对于一个字符序列，求FIRST集
    first = set()
    for V in prod:
      thisFirst = getFirst(V)
      if 'eps' in thisFirst: # 与FirstCollection中的思路相同，对eps的存在与否进行分类
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
      seqFirst = getSequenceFirst(prod) # 对于每个表达式求序列FIRST集
      for vt in seqFirst:
        if vt != 'eps':
          predictTable[vn][vt] = prod # 终结符非eps的情况：直接填入
        else: #eps
          for vnext in followCollection[vn]: # 填入FOLLOW集中的元素对应的产生式
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
  list2str = lambda l: reduce(lambda ele, substr: ele + substr, l) # 实现从数组到字符串的拼接转化

  predictTable = getPredictTable(grammer['prodDict'], grammer['V_N'], grammer['V_T'])
  symbolStack = ['#', grammer['S']] # 初始化下推栈
  step = 0
  i = 0
  sentenceLen = len(list2str(sentence)) + 4 # 定义表格格式化参数
  output = lambda outList : print('{:^6}  {:<10}  {:<{slen}}  {:<15}'.format(*outList, slen = sentenceLen)) # 定义表格格式化函数
  
  print()
  output(['Step', 'Stack', 'Input', 'Production']) # 打印表头

  while i < len(sentence): # 在读取完字符串之前
    X = symbolStack[len(symbolStack) - 1] # 栈顶符号
    c = sentence[i] # 当前读取字符

    if X in grammer['V_T']: # 栈顶符号为终结符
      if c == X: # 与读取符号相同
        output([str(step), list2str(symbolStack), list2str(sentence[i:]), c+' is matched'])
        if X != '#':
          symbolStack.pop() # 直接逐出符号栈，读取成功
          step += 1
          i += 1
        else: # c == X and X == '#' <=> 成功结束
          return True
      else: # c != X
        return error()
    elif X in grammer['V_N']: # 栈顶符号为非终结符
      try:
        symbolStack.pop()
        prod = predictTable[X][c]
        output([str(step), list2str(symbolStack+[X]), list2str(sentence[i:]), X+'->'+list2str(['ε'] if prod == ['eps'] else prod)]) # 写表
        if prod != ['eps']:
          symbolStack += prod[::-1] # 将预测表中的产生式倒序输入
        step += 1
      except KeyError: # 预测表（实际上为字典）中不存在该项 <=> 约定的错误处理
        return error()
    else:
      raise RuntimeError(X + ' cannot be classified, check the grammer again.')
  pass
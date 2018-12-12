def getFirstCollection(prodDict, V_N, V_T):
  '''
  Input: 
  prodDict = {
    'A': [['a', 'b', 'T'], [...]] # 约定一个V_N的产生式均在此处，不出现左方V_N重复的情况
    ...
  }
  V_N = set(V_N)
  V_T = set(V_T)

  Output:
  firstCollection = {
    'A': {'a', 'b', ...},
    ...
  }
  '''
  firstCollection = dict()

  def getFirst(V):
    if V in firstCollection.keys():
      return firstCollection[V] # 动态规划，避免重复计算
    elif V in V_T:
      return set([V]) # 方便计算，定义终结符的FIRST集为自身
    else: # V为未确定FIRST集的V_N，需要计算
      VNFirst = set()
      for prod in prodDict[V]:
        for rightV in prod:
          thisFirst = getFirst(rightV) # 对于当前的符号求FIRST集
          if 'eps' in thisFirst:
            VNFirst |= thisFirst if rightV == prod[len(prod)-1] else thisFirst - {['eps']} # 含eps的情况，分结尾和非结尾进行考虑：1.结尾处不应当删去可能存在的eps；2.非结尾应当删去可能存在的eps
            continue # 当前FIRST集中有eps，对后续符号进行考察
          else: # 不含eps
            VNFirst |= thisFirst # FIRST集合可以确定，并且不必继续后探
            break
      firstCollection[V] = VNFirst # 记录
      return VNFirst
  
  for vn in prodDict.keys():
    if vn not in firstCollection.keys():
      getFirst(vn)
  
  return firstCollection
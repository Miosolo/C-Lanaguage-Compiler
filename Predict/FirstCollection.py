import queue

def getFirstCollection(prodDict, V_N, V_T):
  '''
  Input: 
  prodDict = {
    'V_N': [['a', 'b', 'T'], [...]] # 约定一个V_N的产生式均在此处，不出现左方V_N重复的情况
  }
  V_N = set(V_N)
  V_T = set(V_T)

  Output:
  FIRST_COLLECTION = {
    'V_N': {'a', 'b', ...}
  }
  '''
  firstCollection = dict()

  def getFirst(V):
    if V in firstCollection.keys():
      return firstCollection[V]
    elif V in V_T:
      return set([V])
    else: # V为未确定FIRST集的V_N，需要计算
      VNFirst = set()
      for prod in prodDict[V]:
        for rightV in prod:
          thisFirst = getFirst(rightV)
          if 'eps' in thisFirst:
            VNFirst |= thisFirst if rightV == prod[len(prod)-1] else thisFirst - {'eps'}
            continue
          else: # 不含eps
            VNFirst |= thisFirst
            break
      firstCollection[V] = VNFirst
      return VNFirst
  
  for vn in prodDict.keys():
    if vn not in firstCollection.keys():
      getFirst(vn)
  
  return firstCollection
import FirstCollection
import FollowCollection

def getPredictTable(prodDict, V_T, V_N):
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


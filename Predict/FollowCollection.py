def getFollowCollection(prodDict, V_N, V_T, firstCollection):
  '''
  Input: 
  prodDict = {
    'A': [['a', 'b', 'T'], [...]] # 约定一个V_N的产生式均在此处，不出现左方V_N重复的情况
    ...
  }
  V_N = set(V_N)
  V_T = set(V_T)
  firstCollection = {
    'A': ['a', 'b', 'c',...],
    ...
  }

  Output:
  followCollection = {
    'A': {'#', 'a', ...},
    ...
  }
  '''
  followCollection = dict()

  getFirst = lambda v: firstCollection[v] if v in V_N else set(v)

  def getFollow(leftV):
    if leftV in followCollection.keys():
      return followCollection[leftV] # 动态规划，避免重复计算

    else:
      thisFollow = set('#')
      # 查找其在prodDict中的出现位置 -> (V_N, 产生式的序号，产生式内下标)
      posis = []
      for vn, prods in prodDict.items():
        for prod in prods:
          posis += [(vn, prods.index(prod), i) for i in range(len(prod)) if prod[i] == leftV] # 对每一个单个产生式，查找V出现的位置
      
      for vn, prodIndex, innerIndex in posis:
        if innerIndex != len(prodDict[vn][prodIndex]) - 1: #该符号不在一个表达式的结尾 <=> 有后续符号
          nextSym = prodDict[vn][prodIndex][innerIndex + 1]
          thisFollow |= (getFirst(nextSym) - set(['eps'])) # 规则1，下个符号的FIRST集 - eps
          if 'eps' in getFirst(nextSym):
            thisFollow |= getFollow(nextSym) # 规则1，下个符号的FIRST集中有eps <=> 递归计算其FOLLOW集
        elif vn != leftV: # 该符号在产生式尾部，且与左部不同（防止E->TE类的无限递归）
          thisFollow |= getFollow(vn) # 规则2
      
      followCollection[leftV] = thisFollow # 存表中
      return thisFollow

  for vn in prodDict.keys():
    if vn not in followCollection.keys():
      getFollow(vn)
  
  return followCollection

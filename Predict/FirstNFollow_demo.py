from functools import reduce
import FirstCollection
import FollowCollection

if __name__ == "__main__":
  prodDict = {
    'S': [['T', 'B']],
    'B': [['b', 'T', 'B'], ['eps']],
    'T': [['F', 'T\'']],
    'T\'': [['a', 'F', 'T\''], ['eps']],
    'F': [['n', 'F'], ['(', 'S', ')'], ['t'], ['f']],
  }
  V_N = {'S', 'T', 'B', 'T\'', 'F'}
  V_T = {'eps', '#', 'a', 'n', 't', 'f', '(', ')', 'b'}
  grammer = {'prodDict': prodDict, 'V_N': V_N, 'V_T': V_T, 'S': 'S'}
  demo(grammer)


  
def demo(grammer):
  list2str = lambda l: reduce(lambda ele, substr: ele + substr, l)
  prodStrize = lambda vn, prods: vn + '->' + \
               reduce(lambda ele, substr: ele + '|' + substr, map(lambda p: 'ε' if p == ['eps'] else list2str(p), prods))
  
  prodDict = grammer['prodDict']
  V_N = grammer['V_N']
  V_T = grammer['V_T']

  print('\n文法：')
  for vn, prods in prodDict.items():
    print(prodStrize(vn, prods))

  print('\nFIRST集：')
  firstCollection = FirstCollection.getFirstCollection(prodDict, V_N, V_T)
  for vn, first in firstCollection.items():
    print(vn + ': ' + str(set(map(lambda vt: 'ε' if vt == 'eps' else vt, first))))
  
  print('\nFOLLOW集：')
  followCollection = FollowCollection.getFollowCollection(prodDict, V_N, V_T, firstCollection)
  for vn, follow in followCollection.items():
    print(vn + ': ' + str(follow))
  

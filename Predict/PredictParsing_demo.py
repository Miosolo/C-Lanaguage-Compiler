import PredictParsing
import FirstNFollow_demo
from functools import reduce

list2str = lambda l: reduce(lambda ele, substr: ele + substr, l) # 实现从数组到字符串的拼接转化

def showTable(preDict, V_N, V_T):
  colLabels = V_T - {'eps'}

  print('{:^5}'.format(' '), end = ' ')
  for vt in colLabels:
    print('{:^10}'.format(vt), end = ' ')
  print()
  for vn in V_N:
    print('{:<5}'.format(vn), end = ' ')
    for vt in colLabels:
      try:
        prod = preDict[vn][vt]
        print('{:^10}'.format(vn + '->' + list2str(['ε'] if prod == ['eps'] else prod)), end = ' ')
      except KeyError:
        print('{:^10}'.format(' '), end = ' ')
    print()

def demo(grammer, sentence):
  FirstNFollow_demo.demo(grammer)
  prodDict = grammer['prodDict']
  V_T = grammer['V_T']
  V_N = grammer['V_N']

  print('\n预测分析表：')
  showTable(PredictParsing.getPredictTable(prodDict, V_N, V_T), V_N, V_T)
  
  print('\n对\'' + list2str(sentence) + '\'进行分析:')
  if PredictParsing.predictParsing(grammer, sentence) == True:
    print('\nSucess :D')
  else:
    print('\nFail :(')

if __name__ == "__main__":
  prodDict = {
    'S': [['T', 'B']],
    'B': [['b', 'T', 'B'], ['eps']],
    'T': [['F', 'T\'']],
    'T\'': [['a', 'F', 'T\''], ['eps']],
    'F': [['n', 'F'], ['(', 'S', ')'], ['t'], ['f']],
  }
  V_N = {'S', 'T', 'B', 'T\'', 'F'}
  V_T = {'eps', '#', 'a', 'n', 't', 'f', '(', ')', 'b', '#'}
  grammer1 = {'prodDict': prodDict, 'V_N': V_N, 'V_T': V_T, 'S': 'S'}
  demo(grammer1, ['f', 'b', 'n', 'f', 'a', 't', '#'])

  V_T = {'+', '*', '(', ')', 'eps', 'i', '#'}
  V_N = {'E', 'E\'', 'F', 'T', 'T\''}
  prodDict = {
    'E': [['T', 'E\'']],
    'E\'': [['+', 'T', 'E\''], ['eps']],
    'T': [['F', 'T\'']],
    'T\'': [['*', 'F', 'T\''], ['eps']],
    'F': [['(', 'E', ')'], ['i']]
  }
  grammer2 = {'prodDict': prodDict, 'V_N': V_N, 'V_T': V_T, 'S': 'E'}
  demo(grammer2, ['i', '*', 'i', '+', 'i', '#'])

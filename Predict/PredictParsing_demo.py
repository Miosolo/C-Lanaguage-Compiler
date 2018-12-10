import PredictParsing
import FirstNFollow_demo
from functools import reduce

def showTable(preDict, V_N, V_T):
  colLabels = V_T - {'eps'}
  list2str = lambda l: reduce(lambda ele, substr: ele + substr, l)

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

  FirstNFollow_demo.demo(grammer)

  print('\n预测分析表：')
  showTable(PredictParsing.getPredictTable(prodDict, V_N, V_T), V_N, V_T)
  
  print('\n对\'fbnfat\'进行分析:')
  if PredictParsing.predictParsing(grammer, ['f', 'b', 'n', 'f', 'a', 't', '#']) == True:
    print('\nSucess :D')
  else:
    print('\nFail :(')

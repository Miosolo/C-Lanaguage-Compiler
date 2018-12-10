import PredictParsing

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
  if PredictParsing.predictParsing(grammer, ['f', 'b', 'n', 'f', 'a', 't', '#']) == True:
    print('Sucess!')
  else:
    print('Fail!')
